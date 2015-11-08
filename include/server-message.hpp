#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace tec {
	namespace networking {
		enum MessageType {
			SYNC,
			ENTITY_UPDATE,
			ENTITY_CREATE,
			CLIENT_ID,
			CHAT_MESSAGE
		};

		class ServerMessage {
		public:
			enum { header_length = 8 };
			enum { max_body_length = 512 };

			ServerMessage() : body_length(0), message_type(CHAT_MESSAGE) { }

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
				if (body_length > max_body_length) {
					body_length = 0;
					return false;
				}
				return true;
			}

			void encode_header() {
				char header[header_length + 1] = "";
				std::sprintf(header, "%4d%4d", body_length, message_type);
				std::memcpy(data, header, header_length);
			}

		private:
			char data[header_length + max_body_length];
			std::size_t body_length;
			MessageType message_type;
		};
	}
}
