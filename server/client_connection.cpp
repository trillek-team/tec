#include "server/client_connection.hpp"
#include "proto/game_state.pb.h"
#include "server/server.hpp"
#include "simulation.hpp"
#include <iostream>
#include <thread>

namespace tec {
	namespace networking {
		std::mutex ClientConnection::write_msg_mutex;
		void ClientConnection::StartRead() {
			read_header();
		}

		void ClientConnection::QueueWrite(const ServerMessage& msg) {
			bool write_in_progress;
			{
				std::lock_guard<std::mutex> lock(write_msg_mutex);
				write_in_progress = !write_msgs_.empty();
				write_msgs_.push_back(msg);
			}
			if (!write_in_progress) {
				do_write();
			}
		}
		void ClientConnection::DoJoin() {
			this->simulation.SetEntityState(this->entity);
		}

		void ClientConnection::read_header() {
			auto self(shared_from_this());
			asio::async_read(socket,
				asio::buffer(current_read_msg.GetDataPTR(), ServerMessage::header_length),
				[this, self] (std::error_code error, std::size_t /*length*/) {
				if (!error && current_read_msg.decode_header()) {
					read_body();
				}
				else {
					server->Leave(shared_from_this());
				}
			});
		}

		void ClientConnection::read_body() {
			auto self(shared_from_this());
			asio::async_read(socket,
				asio::buffer(current_read_msg.GetBodyPTR(), current_read_msg.GetBodyLength()),
				[this, self] (std::error_code error, std::size_t /*length*/) {
				if (!error) {
					if (current_read_msg.GetMessageType() == CHAT_MESSAGE) {
						server->Deliver(current_read_msg);
						std::cout.write(current_read_msg.GetBodyPTR(),
							current_read_msg.GetBodyLength());
						std::cout << std::endl;
					}
					else if (current_read_msg.GetMessageType() == SYNC) {
						QueueWrite(current_read_msg);
					}
					else if (current_read_msg.GetMessageType() == ENTITY_UPDATE) {
						this->entity.ParseFromArray(current_read_msg.GetBodyPTR(), current_read_msg.GetBodyLength());
						this->last_confirmed_state_id = current_read_msg.GetStateID();
						this->simulation.SetEntityState(this->entity);
						//std::cout << "Client ID: " << this->id << " ACKD state: " << this->last_confirmed_state_id << std::endl;
					}
					read_header();
				}
				else {
					server->Leave(shared_from_this());
				}
			});
		}

		void ClientConnection::do_write() {
			auto self(shared_from_this());
			std::lock_guard<std::mutex> lock(write_msg_mutex);
			asio::async_write(socket,
				asio::buffer(write_msgs_.front().GetDataPTR(),
				write_msgs_.front().length()),
				[this, self] (std::error_code error, std::size_t /*length*/) {
				if (!error) {
					bool more_to_write = false;
					{
						std::lock_guard<std::mutex> lock(write_msg_mutex);
						write_msgs_.pop_front();
						more_to_write = !write_msgs_.empty();
					}
					if (more_to_write) {
						do_write();
					}
				}
				else {
					server->Leave(shared_from_this());
				}
			});
		}

		void ClientConnection::UpdateGameState(std::set<eid> updated_entities, const GameState& full_state) {
			for (eid entity : updated_entities)  {
				if (full_state.positions.find(entity) != full_state.positions.end()) {
					this->state_changes_since_confirmed.positions[entity] = full_state.positions.at(entity);
				}
				if (full_state.orientations.find(entity) != full_state.orientations.end()) {
					this->state_changes_since_confirmed.orientations[entity] = full_state.orientations.at(entity);
				}
				if (full_state.velocties.find(entity) != full_state.velocties.end()) {
					this->state_changes_since_confirmed.velocties[entity] = full_state.velocties.at(entity);
				}
			}
		}

		tec::networking::ServerMessage ClientConnection::PrepareGameStateUpdateMessage(state_id_t current_state_id) {
			tec::proto::GameStateUpdate gsu_msg;
			gsu_msg.set_state_id(current_state_id);
			for (auto pos : this->state_changes_since_confirmed.positions) {
				tec::proto::Entity* entity = gsu_msg.add_entity();
				entity->set_id(pos.first);
				pos.second.Out(entity->add_components());
				if (this->state_changes_since_confirmed.orientations.find(pos.first) != this->state_changes_since_confirmed.orientations.end()) {
					tec::Orientation ori = this->state_changes_since_confirmed.orientations.at(pos.first);
					ori.Out(entity->add_components());
				}
				if (this->state_changes_since_confirmed.velocties.find(pos.first) != this->state_changes_since_confirmed.velocties.end()) {
					tec::Velocity vel = this->state_changes_since_confirmed.velocties.at(pos.first);
					vel.Out(entity->add_components());
				}
			}
			tec::networking::ServerMessage update_message;
			update_message.SetMessageType(tec::networking::GAME_STATE_UPDATE);
			update_message.SetBodyLength(gsu_msg.ByteSize());
			gsu_msg.SerializeToArray(update_message.GetBodyPTR(), update_message.GetBodyLength());
			update_message.encode_header();
			return std::move(update_message);
		}
	}
}
