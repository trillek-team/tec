#pragma once

#include <asio/buffer.hpp>
#include <cinttypes>
#include <cstdlib>
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
	CHAT_COMMAND,
	LOGIN,
	WORLD_SENT,
	CLIENT_READY_TO_RECEIVE
};

class ServerConnection;
class ClientConnection;

class Message {
public:
	typedef const Message* cptr_type;
	enum { header_length = 16 };
	enum { max_body_length = 1024 };

	Message() : body_length(0), sequence_number(0), message_type(MessageType::CHAT_MESSAGE), message_id(0) {}

	const uint8_t* GetDataPTR() const { return data; }

	uint8_t* GetDataPTR() { return data; }

	std::size_t length() const { return header_length + body_length; }

	asio::mutable_buffer buffer_header() { return asio::buffer(data, header_length); }
	asio::mutable_buffer buffer_body() { return asio::buffer(GetBodyPTR(), GetBodyLength()); }
	asio::const_buffer buffer() const { return asio::buffer(data, length()); }

	const char* GetBodyPTR() const { return reinterpret_cast<const char*>(data + header_length); }

	char* GetBodyPTR() { return reinterpret_cast<char*>(data + header_length); }

	std::size_t GetBodyLength() const { return body_length; }

	void SetSequence(uint32_t value) { this->sequence_number = value; }
	uint32_t GetSequence() const { return this->sequence_number; }

	void SetMessageType(MessageType value) { this->message_type = value; }
	MessageType GetMessageType() const { return this->message_type; }

	void SetMessageID(uint32_t value) { this->message_id = value; }
	uint32_t GetMessageID() const { return this->message_id; }

	void SetBodyLength(std::size_t new_length) {
		body_length = static_cast<uint32_t>(new_length);
		if (new_length > max_body_length) {
			body_length = max_body_length;
			using namespace std::string_literals;
			throw std::length_error("Message body length exceeded by "s + std::to_string(new_length - max_body_length));
		}
	}

	bool decode_header() {
		body_length = decode_32(&data[0]);
		message_type = static_cast<MessageType>(decode_32(&data[4]));
		message_id = decode_32(&data[8]);
		sequence_number = decode_32(&data[12]);
		if (body_length > max_body_length) {
			body_length = 0;
			return false;
		}
		return true;
	}

	void encode_header() {
		encode_32(&data[0], static_cast<uint32_t>(body_length));
		encode_32(&data[4], message_type);
		encode_32(&data[8], message_id);
		encode_32(&data[12], sequence_number);
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
	uint32_t body_length;
	uint32_t sequence_number;
	MessageType message_type;
	uint32_t message_id;
};

class MessagePool {
public:
	typedef std::shared_ptr<Message> ptr_type;
	typedef std::list<ptr_type> list_type;
	// using heap allocation for now
	// possible improvements would be to pool allocate and possibly deduplicate common messages
	static ptr_type get() { return std::make_shared<Message>(); }
};

class MessageIn;

class MessageOut : public google::protobuf::io::ZeroCopyOutputStream {
	friend ServerConnection;
	friend ClientConnection;
	friend MessageIn;

public:
	MessageOut() : message_type(MessageType::CHAT_MESSAGE), message_id(0), current_sequence(0), payload_written(0) {}
	MessageOut(MessageType msg_type) : message_type(msg_type), message_id(0), current_sequence(0), payload_written(0) {}
	MessageOut(const MessageOut& other) :
			message_type(other.message_type), message_id(other.message_id), current_sequence(other.current_sequence),
			payload_written(other.payload_written), message_list(other.message_list) {}
	MessageOut(MessageOut&& other) :
			message_type(other.message_type), message_id(other.message_id), current_sequence(other.current_sequence),
			payload_written(other.payload_written), message_list(std::move(other.message_list)) {
		other.payload_written = 0;
		other.current_sequence = 0;
	}

	// generate messages from a fixed buffer
	void FromBuffer(const void* body, size_t length);

	// generate messages from an existing string
	void FromString(const std::string& body) { FromBuffer(body.data(), body.size()); }

	void SetMessageType(MessageType value) { this->message_type = value; }
	void SetMessageID(uint32_t value) { this->message_id = value; }

	MessageType GetMessageType() const { return this->message_type; }
	uint32_t GetMessageID() const { return this->message_id; }

	bool IsEmpty() const { return message_list.empty(); }

	MessagePool::list_type GetMessages();

	// ZeroCopyOutputStream interface
	virtual bool Next(void** data, int* size);
	virtual void BackUp(int count);
	virtual int64_t ByteCount() const { return payload_written; }
	virtual bool WriteAliasedRaw(const void* /*data*/, int /*size*/) { return false; }
	virtual bool AllowsAliasing() const { return false; }

private:
	MessageType message_type;
	uint32_t message_id;
	uint32_t current_sequence;
	int64_t payload_written;
	MessagePool::list_type message_list;
};

class MessageIn : public google::protobuf::io::ZeroCopyInputStream {
	friend ServerConnection;
	friend ClientConnection;

public:
	MessageIn() : message_type(MessageType::CHAT_MESSAGE), message_id(0), payload_read(0), read_offset(0) {}
	MessageIn(MessageType msg_type) : message_type(msg_type), message_id(0), payload_read(0), read_offset(0) {}

	// helper to copy to a fixed buffer, output will be truncated if buffer isn't large enough
	void ReadBuffer(void* body, size_t length);

	// read and append body data into a string
	void ReadString(std::string& body);

	// return body as a new string
	std::string ToString() {
		std::string body;
		body.reserve(GetSize());
		ReadString(body);
		return body;
	}

	void SetMessageType(MessageType value) { this->message_type = value; }
	void SetMessageID(uint32_t value) { this->message_id = value; }

	MessageType GetMessageType() const { return this->message_type; }
	uint32_t GetMessageID() const { return this->message_id; }

	bool IsEmpty() const { return message_list.empty(); }

	size_t GetSize() const;

	bool PushMessage(MessagePool::ptr_type msg);
	bool DecodeMessages();
	bool AssignMessages(MessagePool::list_type msgs);

	MessageOut ToOut() const;

	void Reset() {
		this->payload_read = 0;
		this->read_offset = 0;
	}

	// ZeroCopyInputStream interface
	virtual bool Next(const void** data, int* size);
	virtual void BackUp(int count);
	virtual bool Skip(int count);
	// this returns the number of bytes currently read, not the total available
	virtual int64_t ByteCount() const { return payload_read + read_offset; }

private:
	MessageType message_type;
	uint32_t message_id;
	int64_t payload_read;
	size_t read_offset;

	MessagePool::list_type message_list;
	MessagePool::list_type::const_iterator read_index;
};

} // end namespace networking
} // end of namespace tec
