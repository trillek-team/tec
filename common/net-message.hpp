#pragma once

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <google/protobuf/io/zero_copy_stream.h>
#include <list>
#include <memory>

#include "tec-types.hpp"

namespace tec {
namespace networking {
enum MessageType : int {
	SYNC,
	MULTI_PART,
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

class ServerConnection;
class ClientConnection;

class NetMessage {
public:
	typedef std::unique_ptr<NetMessage> ptr_type;
	typedef const NetMessage* cptr_type;
	typedef std::shared_ptr<NetMessage> shared_type;
	enum { header_length = 16 };
	enum { max_body_length = 256 };

	NetMessage() : body_length(0), message_type(MessageType::CHAT_MESSAGE) {}

	const uint8_t* GetDataPTR() const { return data; }

	uint8_t* GetDataPTR() { return data; }

	std::size_t length() const { return header_length + body_length; }

	const char* GetBodyPTR() const { return reinterpret_cast<const char*>(data + header_length); }

	char* GetBodyPTR() { return reinterpret_cast<char*>(data + header_length); }

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
		if (body_length > max_body_length) {
			body_length = 0;
			return false;
		}
		return true;
	}

	void encode_header() {
		encode_32(&data[0], static_cast<uint32_t>(body_length));
		encode_32(&data[4], message_type);
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
	// clang-format on

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

	uint8_t data[header_length + max_body_length]{0};
	std::size_t body_length;
	MessageType message_type;
};

class MessagePool {
public:
	static std::unique_ptr<NetMessage> make_unique() { return std::make_unique<NetMessage>(); }
	static std::shared_ptr<NetMessage> make_shared() { return std::make_shared<NetMessage>(); }
};

class MessageOutStream : public google::protobuf::io::ZeroCopyOutputStream {
	friend ServerConnection;
	friend ClientConnection;

public:
	MessageOutStream() : message_type(MessageType::CHAT_MESSAGE), payload_written(0) {}
	MessageOutStream(MessageType msg_type) : message_type(msg_type), payload_written(0) {}

	// generate messages from a fixed buffer
	void FromBuffer(const void* body, size_t length);

	// generate messages from an existing string
	void FromString(const std::string& body) { FromBuffer(body.data(), body.size()); }

	void SetMessageType(MessageType value) { this->message_type = value; }

	MessageType GetMessageType() const { return this->message_type; }

	bool IsEmpty() const { return message_list.empty(); }

	// ZeroCopyOutputStream interface
	virtual bool Next(void** data, int* size);
	virtual void BackUp(int count);
	virtual int64_t ByteCount() const { return payload_written; }
	virtual bool WriteAliasedRaw(const void* /*data*/, int /*size*/) { return false; }
	virtual bool AllowsAliasing() const { return false; }

private:
	MessageType message_type;
	int64_t payload_written;
	std::list<std::unique_ptr<NetMessage>> message_list;
};

class MessageInStream : public google::protobuf::io::ZeroCopyInputStream {
	friend ServerConnection;
	friend ClientConnection;

public:
	MessageInStream() : message_type(MessageType::CHAT_MESSAGE), payload_read(0), read_offset(0) {}
	MessageInStream(MessageType msg_type) : message_type(msg_type), payload_read(0), read_offset(0) {}

	// helper to copy to a fixed buffer, output will be truncated if buffer isn't large enough
	void ToBuffer(void* body, size_t length);

	// read body into a string
	void AsString(std::string& body);

	void SetMessageType(MessageType value) { this->message_type = value; }

	MessageType GetMessageType() const { return this->message_type; }

	bool IsEmpty() const { return message_list.empty(); }

	size_t GetSize() const;

	// ZeroCopyInputStream interface
	virtual bool Next(const void** data, int* size);
	virtual void BackUp(int count);
	virtual bool Skip(int count);
	// this returns the number of bytes currently read, not the total available
	virtual int64_t ByteCount() const { return payload_read + read_offset; }

private:
	MessageType message_type;
	int64_t payload_read;
	size_t read_offset;

	typedef std::list<std::unique_ptr<NetMessage>> list_type;
	list_type message_list;
	list_type::const_iterator read_index;
};

} // end namespace networking
} // end of namespace tec
