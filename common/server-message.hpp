// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#ifndef TRILLEK_COMMON_SERVER_MESSAGE_HPP
#define TRILLEK_COMMON_SERVER_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cinttypes>

#include "types.hpp"

namespace tec {
	namespace networking {
		enum MessageType : int {
			SYNC,
			CLIENT_COMMAND,
			ENTITY_CREATE,
			ENTITY_DESTROY,
			CLIENT_JOIN,
			CLIENT_ID,
			CLIENT_LEAVE,
			GAME_STATE_UPDATE,
			CHAT_MESSAGE
		};

		class ServerMessage {
		public:
			enum { header_length = 16 };
			enum { max_body_length = 512 };

			ServerMessage() : body_length(0), last_recv_state_id(0),
				message_type(MessageType::CHAT_MESSAGE) { }

			const char* GetDataPTR() const {
				return data;
			}

			char* GetDataPTR() {
				return data;
			}

			std::size_t length() const {
				return header_length + body_length;
			}

			const char* GetBodyPTR() const {
				return data + header_length;
			}

			char* GetBodyPTR() {
				return data + header_length;
			}

			state_id_t GetStateID() const {
				return this->last_recv_state_id;
			}

			void SetStateID(state_id_t state_id) {
				this->last_recv_state_id = state_id;
			}

			std::size_t GetBodyLength() const {
				return body_length;
			}

			void SetMessageType(MessageType value) {
				this->message_type = value;
			}

			MessageType GetMessageType() {
				return this->message_type;
			}

			void SetBodyLength(std::size_t new_length) {
				body_length = new_length;
				if (body_length > max_body_length) {
					body_length = max_body_length;
				}
			}

			bool decode_header() {
				char length_header[5] = "";
				std::strncat(length_header, data, 4);
				body_length = std::atoi(length_header);
				char type_header[5] = "";
				std::strncat(type_header, data + 4, 4);
				message_type = static_cast<MessageType>(std::atoi(type_header));
				char state_id_header[9] = "";
				std::strncat(state_id_header, data + 8, 8);
				last_recv_state_id = static_cast<state_id_t>(std::atoi(state_id_header));
				if (body_length > max_body_length) {
					body_length = 0;
					return false;
				}
				return true;
			}

			void encode_header() {
				char header[header_length + 1] = "";
				std::sprintf(header, "%4zu%4d%" PRI_STATE_ID_T, body_length, message_type, last_recv_state_id);
				std::memcpy(data, header, header_length);
			}

		private:
			char data[header_length + max_body_length]{ 0 };
			std::size_t body_length;
			state_id_t last_recv_state_id;
			MessageType message_type;
		};
	}
}

#endif
