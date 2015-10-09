#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace tec {
	namespace networking {
		class ServerMessage {
		public:
			enum { header_length = 5 };
			enum { max_body_length = 512 };

			ServerMessage() : body_length(0), entity_update_message(false) { }

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

			std::size_t GetBodyLength() const {
				return body_length;
			}

			void EntityUpdateMessage(bool value) {
				this->entity_update_message = value;
			}

			bool EntityUpdateMessage() {
				return this->entity_update_message;
			}

			void SetBodyLength(std::size_t new_length) {
				body_length = new_length;
				if (body_length > max_body_length) {
					body_length = max_body_length;
				}
			}

			bool decode_header() {
				char header[header_length + 1] = "";
				std::strncat(header, data, header_length - 1);
				body_length = std::atoi(header);
				if (body_length > max_body_length) {
					body_length = 0;
					return false;
				}
				if (data[header_length - 1] == 't') {
					this->entity_update_message = true;
				}
				else {
					this->entity_update_message = false;
				}
				return true;
			}

			void encode_header() {
				char header[header_length + 1] = "";
				std::sprintf(header, "%4d%c", body_length, (entity_update_message ? 't' : 'f'));
				std::memcpy(data, header, header_length);
			}

		private:
			char data[header_length + max_body_length];
			std::size_t body_length;
			bool entity_update_message;
		};
	}
}
