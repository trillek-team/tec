
#include <spdlog/spdlog.h>

#include "net-message.hpp"

namespace tec {
namespace networking {

void MessageOutStream::FromBuffer(const void* body, size_t length) {
	size_t remain = length;
	size_t offset = 0;
	const char* body_ptr = reinterpret_cast<const char*>(body);
	while (remain > 0) {
		void* buffer;
		int buffer_size;
		if (!Next(&buffer, &buffer_size)) {
			spdlog::get("console_log")->warn("MessageStream::FromString failed to request buffer");
			return;
		}
		if (!buffer_size) {
			continue;
		}
		if (buffer_size <= remain) {
			memcpy(buffer, body_ptr + offset, buffer_size);
			remain -= buffer_size;
			offset += buffer_size;
		}
		else { // buffer is larger than what we need (or have left)
			memcpy(buffer, body_ptr + offset, remain);
			BackUp(static_cast<int>(buffer_size - remain)); // return the extra
			return;
		}
	}
}

bool MessageOutStream::Next(void** data, int* size) {
	// using heap allocation for now
	// possible improvements would be to pool allocate and possibly deduplicate common messages
	message_list.emplace_back(std::make_unique<NetMessage>());
	NetMessage* msg = message_list.back().get();
	msg->SetMessageType(this->message_type);
	msg->SetBodyLength(NetMessage::max_body_length);
	size_t body_size = msg->GetBodyLength();
	*data = msg->GetBodyPTR();
	*size = static_cast<int>(body_size);
	payload_written += body_size;
	return true;
}

void MessageOutStream::BackUp(int count) {
	if (message_list.empty() || (count < 1)) {
		return;
	}
	NetMessage* msg = message_list.back().get();
	if (count > msg->GetBodyLength()) {
		return;
	}
	msg->SetBodyLength(msg->GetBodyLength() - count);
	payload_written -= count;
}

void MessageInStream::ToBuffer(void* body, size_t length) {
	size_t offset = 0;
	char* body_ptr = reinterpret_cast<char*>(body);
	while (offset < length) {
		const void* buffer;
		int buffer_size;
		if (!Next(&buffer, &buffer_size)) {
			// error or end of stream
			return;
		}
		if (!buffer_size) {
			continue;
		}
		if (offset + buffer_size <= length) {
			memcpy(body_ptr + offset, buffer, buffer_size);
			offset += buffer_size;
		}
		else {
			// trucation (provided buffer too short)
			memcpy(body_ptr + offset, buffer, length - offset);
			buffer_size -= static_cast<int>(length - offset);
			BackUp(buffer_size); // in case someone feels like trying again
			// done, no more room to write
			return;
		}
	}
}

void MessageInStream::AsString(std::string& body) {
	body.resize(GetSize());
	ToBuffer(body.data(), body.size());
}

size_t MessageInStream::GetSize() const {
	size_t total = 0;
	for (auto& ptr : message_list) {
		total += ptr->GetBodyLength();
	}
	return total;
}

bool MessageInStream::Next(const void** data, int* size) {
	if (!payload_read) {
		// potentially didn't read anything yet
		read_index = message_list.cbegin();
	}
	size_t read_length = 0;
	while (read_length == 0) {
		if (read_index == message_list.cend()) {
			// end of stream
			return false;
		}
		read_length = (*read_index)->GetBodyLength();
		if (read_offset == read_length) {
			// we already returned all of this message, get the next one
			payload_read += read_offset;
			read_offset = read_length = 0;
			read_index++;
		}
	}
	if (!read_offset) {
		// we haven't returned the message at the current index
		read_offset = read_length;
		*size = static_cast<int>(read_offset);
		*data = (*read_index)->GetBodyPTR();
		return true;
	}
	if (read_offset < read_length) {
		// more to return, probably from backing up
		*data = (*read_index)->GetBodyPTR() + read_offset;
		*size = static_cast<int>(read_length - read_offset);
		read_offset = read_length;
		return true;
	}
	// this is unexpected
	return false;
}

void MessageInStream::BackUp(int count) {
	if (count <= read_offset) {
		read_offset -= count;
	}
}

bool MessageInStream::Skip(int count) {
	const void* ignore;
	int size;
	while (count > 0) {
		if (!Next(&ignore, &size)) {
			return false; // Next didn't like that
		}
		if (size <= count) { // more to skip
			count -= size;
		} else {
			size -= count;
			BackUp(size);
			return true;
		}
	}
	return true;
}

} // namespace networking
} // namespace tec
