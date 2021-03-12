
#include "net-message.hpp"

namespace tec {
namespace networking {

bool MessageStream::Next(void** data, int* size) {
	message_list.emplace_back(std::make_unique<ServerMessage>());
	ServerMessage* msg = message_list.back().get();
	msg->SetMessageType(this->message_type);
	msg->SetBodyLength(ServerMessage::max_body_length);
	size_t body_size = msg->GetBodyLength();
	*data = msg->GetBodyPTR();
	*size = static_cast<int>(body_size);
	payload_size += body_size;
	return true;
}

void MessageStream::BackUp(int count) {
	if (message_list.empty() || (count < 1)) {
		return;
	}
	ServerMessage* msg = message_list.back().get();
	if (count > msg->GetBodyLength()) {
		return;
	}
	msg->SetBodyLength(msg->GetBodyLength() - count);
	payload_size -= count;
}

} // namespace networking
} // namespace tec
