#pragma once

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "tec-types.hpp"

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
	CHAT_MESSAGE,
	CHAT_COMMAND
};

class ServerMessage {
public:
	enum { header_length = 16 };
	enum { max_body_length = 256 };

	ServerMessage() : body_length(0), last_recv_state_id(0), message_type(MessageType::CHAT_MESSAGE) {}

	const uint8_t* GetDataPTR() const { return data; }

	uint8_t* GetDataPTR() { return data; }

	std::size_t length() const { return header_length + body_length; }

	const char* GetBodyPTR() const { return reinterpret_cast<const char*>(data + header_length); }

	char* GetBodyPTR() { return reinterpret_cast<char*>(data + header_length); }

	state_id_t GetStateID() const { return this->last_recv_state_id; }

	void SetStateID(state_id_t state_id) { this->last_recv_state_id = state_id; }

	std::size_t GetBodyLength() const { return body_length; }

	void SetMessageType(MessageType value) { this->message_type = value; }

	MessageType GetMessageType() const { return this->message_type; }

	void SetBodyLength(std::size_t new_length) {
		body_length = new_length;
		if (body_length > max_body_length) {
			printf("Message len %zd truncated by %d bytes\n",
				   body_length,
				   static_cast<int>(body_length - max_body_length));
			body_length = max_body_length;
		}
	}

	bool decode_header() {
		body_length = decode_32(&data[0]);
		message_type = static_cast<MessageType>(decode_32(&data[4]));
		last_recv_state_id = static_cast<state_id_t>(decode_64(&data[8]));
		if (body_length > max_body_length) {
			body_length = 0;
			return false;
		}
		return true;
	}

	void encode_header() {
		encode_32(&data[0], static_cast<uint32_t>(body_length));
		encode_32(&data[4], message_type);
		encode_64(&data[8], last_recv_state_id);
	}

private:
	// clang-format off
	static uint32_t decode_32(const uint8_t* p) {
		return static_cast<uint32_t>(p[0])
			| (static_cast<uint32_t>(p[1]) << 8)
			| (static_cast<uint32_t>(p[2]) << 16)
			| (static_cast<uint32_t>(p[3]) << 24);
	}

	static uint64_t decode_64(const uint8_t* p) {
		return static_cast<uint64_t>(p[0])
			| (static_cast<uint64_t>(p[1]) << 8)
			| (static_cast<uint64_t>(p[2]) << 16)
			| (static_cast<uint64_t>(p[3]) << 24)
			| (static_cast<uint64_t>(p[4]) << 32)
			| (static_cast<uint64_t>(p[5]) << 40)
			| (static_cast<uint64_t>(p[6]) << 48)
			| (static_cast<uint64_t>(p[7]) << 56);
	}

	static void encode_32(uint8_t* p, uint32_t value) {
		p[0] = 0xff & (value);
		p[1] = 0xff & (value >> 8);
		p[2] = 0xff & (value >> 16);
		p[3] = 0xff & (value >> 24);
	}

	static void encode_64(uint8_t* p, uint64_t value) {
		p[0] = 0xff & (value);
		p[1] = 0xff & (value >> 8);
		p[2] = 0xff & (value >> 16);
		p[3] = 0xff & (value >> 24);
		p[4] = 0xff & (value >> 32);
		p[5] = 0xff & (value >> 40);
		p[6] = 0xff & (value >> 48);
		p[7] = 0xff & (value >> 56);
	}
	// clang-format on

	uint8_t data[header_length + max_body_length]{0};
	std::size_t body_length;
	state_id_t last_recv_state_id;
	MessageType message_type;
};
} // end namespace networking
} // end of namespace tec
