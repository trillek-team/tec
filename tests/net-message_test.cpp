
#include "net-message.hpp"

#include <asio/buffer.hpp>
#include <gtest/gtest.h>
#include <new>

namespace tec {
namespace networking {

// clang-format off
// Warning: these are picky about whitespace, the s has to go right on the end to make a literal
#define _STRINGIFY(a) #a
#define CASE_LABEL(t) case t: return _STRINGIFY(t)s
// clang-format on
std::string GetNamedMessageType(MessageType mt) {
	using namespace std::string_literals;
	switch (mt) {
		CASE_LABEL(MessageType::SYNC);
		CASE_LABEL(MessageType::MULTI_PART);
		CASE_LABEL(MessageType::CLIENT_COMMAND);
		CASE_LABEL(MessageType::ENTITY_CREATE);
		CASE_LABEL(MessageType::ENTITY_DESTROY);
		CASE_LABEL(MessageType::CLIENT_JOIN);
		CASE_LABEL(MessageType::CLIENT_ID);
		CASE_LABEL(MessageType::CLIENT_LEAVE);
		CASE_LABEL(MessageType::GAME_STATE_UPDATE);
		CASE_LABEL(MessageType::CHAT_MESSAGE);
	default: return "MessageType::#"s + std::to_string(mt);
	}
}

std::ostream& operator<<(std::ostream& os, const MessageType mt) { return os << GetNamedMessageType(mt); }

bool HeadersEqual(const Message& a, const Message& b) {
	EXPECT_NE(&a, &b); // the same object twice? probably don't want that
	return (a.GetMessageType() == b.GetMessageType()) && (a.GetMessageID() == b.GetMessageID())
		   && (a.GetSequence() == b.GetSequence()) && (a.GetBodyLength() == b.GetBodyLength());
}
void PrintTo(const Message& msg, std::ostream* os) {
	auto old_flags = os->flags();
	*os << "Message{len=" << msg.GetBodyLength() << ", type=" << msg.GetMessageType() << ", id=" << std::hex
		<< msg.GetMessageID() << std::dec << ", seq=" << msg.GetSequence() << "}";
	os->setf(old_flags);
}
// make something sufficiently large to require multiple messages
static std::string GetLongTestString(size_t num_messages, bool variant = false) {
	using namespace std::string_literals;
	std::string testdata{"TEST0123456789abcdefghijklmnopqrstuvwxyz"s};
	if (variant) {
		testdata = "TEST0123456789ZYXWVUTSRQPONMLKJIHGFEDCBA"s;
	}
	std::string teststring;
	teststring.append(testdata);
	while (teststring.size() < (Message::max_body_length * num_messages)) {
		teststring.append(testdata);
	}
	return std::move(teststring);
}

static void MessageListContentEqual(MessagePool::list_type& list_a, MessagePool::list_type& list_b) {
	EXPECT_NE(&list_a, &list_b);
	auto iterator_a = list_a.begin();
	auto iterator_b = list_b.begin();
	while (iterator_a != list_a.end() && iterator_b != list_b.end()) {
		auto& item_a = *iterator_a;
		auto& item_b = *iterator_b;
		EXPECT_PRED2(HeadersEqual, *item_a, *item_b);
		std::string value_a(item_a->GetBodyPTR(), item_a->GetBodyLength());
		std::string value_b(item_b->GetBodyPTR(), item_b->GetBodyLength());
		EXPECT_EQ(value_a, value_b);
		iterator_a++;
		iterator_b++;
	}
	bool lists_have_same_number_items = iterator_a == list_a.end() && iterator_b == list_b.end();
	EXPECT_TRUE(lists_have_same_number_items);
}

static void ReceiveAsIs(size_t, MessagePool::ptr_type&) {}

static void FakeSendReceive(
		MessagePool::list_type& source, MessageIn* msgin, std::function<void(size_t, MessagePool::ptr_type&)> modifer) {
	size_t index = 0;
	for (auto msg_itr = source.begin(); msg_itr != source.end(); index++) {
		auto& msg = *msg_itr;
		auto new_msg = MessagePool::get();
		memcpy(new_msg->GetDataPTR(), msg->GetDataPTR(), msg->length());
		EXPECT_TRUE(new_msg->decode_header());
		auto mod_msg = new_msg;
		modifer(index, mod_msg);
		if (msgin && mod_msg) {
			EXPECT_TRUE(msgin->PushMessage(new_msg));
			if (mod_msg != new_msg) {
				// modifier generated a message, loop around again
				continue;
			}
		}
		msg_itr++;
	}
}

// helpers to find uninitialized variables
template <typename T> struct test_deallocate {
	void operator()(T* p) { free(p); }
};
template <typename T, typename... Args> std::unique_ptr<T, test_deallocate<T>> make_test_unique(Args... args) {
	uint8_t* p = reinterpret_cast<uint8_t*>(malloc(sizeof(T)));
	for (size_t i = 0; i < sizeof(T); i++) {
		p[i] = static_cast<uint8_t>(rand()); // filling with rand :)
	}
	return std::unique_ptr<T, test_deallocate<T>>(new (reinterpret_cast<T*>(p)) T(args...));
}
// a test class to make sure our tests are able to detect uninitialized values
struct init_test_class {
	init_test_class(int* p) { (this->observe = p)++; }
	~init_test_class() { (*observe)--; } // makes sure dtor gets called
	int value; // ctor doesn't initialize value, so it should be undefined data
	int* observe;
	bool operator!=(const init_test_class& other) const { return this->value != other.value; }
};

TEST(Message, initial) {
	// sanity check
	int leaked_objects = 0;
	for (int i = 0; i < 1000; i++) {
		ASSERT_NE(
				*make_test_unique<init_test_class>(&leaked_objects),
				*make_test_unique<init_test_class>(&leaked_objects));
		ASSERT_EQ(leaked_objects, 0);
	}
	// do the actual test on Message
	EXPECT_PRED2(HeadersEqual, *make_test_unique<Message>(), *make_test_unique<Message>());
}

TEST(Message, limits) {
	Message msg;
	auto teststring = GetLongTestString(2);
	// ensure that our test string requires more than one message (it should)
	EXPECT_GT(teststring.size(), Message::max_body_length);
	// does it start empty?
	EXPECT_EQ(msg.GetBodyLength(), 0);
	msg.SetBodyLength(Message::max_body_length);
	EXPECT_EQ(msg.GetBodyLength(), Message::max_body_length);
	// can it get too big?
	EXPECT_THROW(msg.SetBodyLength(Message::max_body_length + 1), std::length_error);
	EXPECT_EQ(msg.GetBodyLength(), Message::max_body_length);
	EXPECT_THROW(msg.SetBodyLength(Message::max_body_length * 2), std::length_error);
	EXPECT_EQ(msg.GetBodyLength(), Message::max_body_length);
	// unsigned weirdness?
	EXPECT_THROW(msg.SetBodyLength(~0), std::length_error);
	EXPECT_EQ(msg.GetBodyLength(), Message::max_body_length);
	// can we shrink?
	msg.SetBodyLength(0);
	EXPECT_EQ(msg.GetBodyLength(), 0);
}
TEST(Message, reflexive) {
	Message msg_a;
	Message msg_b;
	msg_a.SetMessageType(MessageType::ENTITY_CREATE);
	msg_a.SetBodyLength(4);
	std::string test{"TEST"};
	memcpy(msg_a.GetBodyPTR(), test.data(), test.size());
	msg_a.encode_header();
	auto buf = asio::buffer(msg_a.GetDataPTR(), msg_a.length());
	memcpy(msg_b.GetDataPTR(), buf.data(), buf.size());
	msg_b.decode_header();
	EXPECT_PRED2(HeadersEqual, msg_a, msg_b);
	std::string result{msg_b.GetBodyPTR(), msg_b.GetBodyLength()};
	EXPECT_EQ(test, result);
}
TEST(MessageOut, initial) {
	auto msg_ptr = make_test_unique<MessageOut>();
	auto& msg = *msg_ptr;
	EXPECT_TRUE(msg.IsEmpty());
	EXPECT_EQ(msg.ByteCount(), 0);
	EXPECT_EQ(msg.GetMessageType(), CHAT_MESSAGE);
	EXPECT_EQ(msg.GetMessageID(), 0);
}
TEST(MessageOut, allocate) {
	MessageOut msg{MessageType::CLIENT_LEAVE};
	int size = 0;
	void* buffer = &size;
	// should be able to allocate buffers for output
	EXPECT_TRUE(msg.Next(&buffer, &size));
	EXPECT_NE(buffer, &size);
	EXPECT_GT(size, 0);
	EXPECT_EQ(size, msg.ByteCount());
	msg.BackUp(7);
	size -= 7;
	EXPECT_EQ(size, msg.ByteCount());
	MessagePool::list_type msgs = msg.GetMessages();
	EXPECT_EQ(msgs.size(), 1);
	msgs.back()->decode_header();
	EXPECT_EQ(msgs.back()->GetMessageType(), msg.GetMessageType());
	EXPECT_EQ(size, msgs.back()->GetBodyLength());
}
TEST(MessageOut, FromString) {
	MessageOut msg{MessageType::CLIENT_LEAVE};

	auto teststring = GetLongTestString(5);
	msg.FromString(teststring);
	EXPECT_EQ(teststring.size(), msg.ByteCount());
	MessagePool::list_type msgs = msg.GetMessages();
	EXPECT_GE(msgs.size(), 5);
	size_t fragment_offset = 0;
	uint32_t expected_sequence = 0;
	size_t last_message_count = msgs.size() - 1;
	for (auto msg_ptr : msgs) {
		// did our large message get divided up and assigned correctly?
		std::string fragment{msg_ptr->GetBodyPTR(), msg_ptr->GetBodyLength()};
		size_t fragment_size = msg_ptr->GetBodyLength();
		EXPECT_EQ(expected_sequence, msg_ptr->GetSequence());
		if (expected_sequence == last_message_count) {
			EXPECT_EQ(msg_ptr->GetMessageType(), MessageType::CLIENT_LEAVE);
		}
		else {
			EXPECT_EQ(msg_ptr->GetMessageType(), MessageType::MULTI_PART);
		}
		EXPECT_EQ(teststring.substr(fragment_offset, fragment_size), fragment);
		fragment_offset += fragment_size;
		expected_sequence++;
	}
	EXPECT_EQ(fragment_offset, teststring.size());

	// this is more to do with the internal structure
	EXPECT_TRUE(msgs.back()->decode_header());
	EXPECT_EQ(msgs.back()->GetMessageType(), msg.GetMessageType());
}

TEST(MessageOut, Reuse) {
	MessageOut msg{MessageType::CLIENT_LEAVE};

	auto teststring = GetLongTestString(2);
	msg.FromString(teststring);
	EXPECT_EQ(teststring.size(), msg.ByteCount());
	MessagePool::list_type msgs1 = msg.GetMessages();
	MessagePool::list_type msgs2 = msg.GetMessages();
	EXPECT_GE(msgs1.size(), 2);
	EXPECT_GE(msgs2.size(), 2);
	ASSERT_EQ(msgs1.size(), msgs2.size());
	size_t fragment_offset = 0;
	for (auto msg_ptr : msgs1) {
		// did our large message get divided up and assigned correctly?
		std::string fragment{msg_ptr->GetBodyPTR(), msg_ptr->GetBodyLength()};
		size_t fragment_size = msg_ptr->GetBodyLength();
		EXPECT_EQ(teststring.substr(fragment_offset, fragment_size), fragment);
		fragment_offset += fragment_size;
	}
	EXPECT_EQ(fragment_offset, teststring.size());
	fragment_offset = 0;
	for (auto msg_ptr : msgs2) {
		// is the second one the same?
		std::string fragment{msg_ptr->GetBodyPTR(), msg_ptr->GetBodyLength()};
		size_t fragment_size = msg_ptr->GetBodyLength();
		EXPECT_EQ(teststring.substr(fragment_offset, fragment_size), fragment);
		fragment_offset += fragment_size;
	}
	EXPECT_EQ(fragment_offset, teststring.size());
}

TEST(MessageIn, initial) {
	auto msg_ptr = make_test_unique<MessageIn>();
	MessageIn& msg = *msg_ptr;
	EXPECT_TRUE(msg.IsEmpty());
	EXPECT_EQ(msg.ByteCount(), 0);
	EXPECT_EQ(msg.GetSize(), 0);
	EXPECT_EQ(msg.GetMessageType(), CHAT_MESSAGE);
	EXPECT_EQ(msg.GetMessageID(), 0);
	int size = 77;
	const void* buffer = &size;
	// an empty stream should hit EOF and return false
	EXPECT_FALSE(msg.Next(&buffer, &size));
	EXPECT_EQ(buffer, &size); // these values should remain unchanged if Next was false
	EXPECT_EQ(size, 77);
}
TEST(MessageIn, ReadingAndReset) {
	MessageOut msgout{MessageType::CLIENT_LEAVE};
	auto teststring = GetLongTestString(2);
	msgout.FromString(teststring);
	MessagePool::list_type msgs = msgout.GetMessages();
	MessageIn msgin;
	FakeSendReceive(msgs, &msgin, ReceiveAsIs);
	EXPECT_TRUE(msgin.DecodeMessages());
	{
		std::string result_ReadString;
		msgin.ReadString(result_ReadString);
		EXPECT_EQ(msgin.GetMessageType(), msgout.GetMessageType()); // did the message type get through?
		EXPECT_EQ(msgin.ByteCount(), msgout.ByteCount()); // everything read successfully?
		ASSERT_EQ(teststring.size(), result_ReadString.size());
		EXPECT_EQ(teststring, result_ReadString); // did the message payload get through?
	}
	{
		std::string result_after_Reset;
		msgin.Reset();
		msgin.ReadString(result_after_Reset);
		// do reads after Reset() still return the same?
		ASSERT_EQ(teststring.size(), result_after_Reset.size());
		EXPECT_EQ(teststring, result_after_Reset);
	}
	{
		std::string teststring2x = GetLongTestString(1, true); // get a different test string
		std::string result_Append = teststring2x; // assume we have a string
		teststring2x.append(teststring); // and want to add a message to it
		msgin.Reset();
		msgin.ReadString(result_Append); // does ReadString do that?
		ASSERT_EQ(teststring2x.size(), result_Append.size());
		EXPECT_EQ(teststring2x, result_Append);
	}
	{
		msgin.Reset();
		std::string result_ToString = msgin.ToString();
		// did ToString() work the same way as ReadString()?
		ASSERT_EQ(teststring.size(), result_ToString.size());
		EXPECT_EQ(teststring, result_ToString);
	}
	{
		std::string result_ReadBuffer;
		msgin.Reset();
		result_ReadBuffer.resize(msgin.GetSize());
		msgin.ReadBuffer(result_ReadBuffer.data(), result_ReadBuffer.size());
		// did ReadBuffer() work?
		EXPECT_EQ(teststring.size(), msgin.GetSize());
		EXPECT_EQ(teststring, result_ReadBuffer);
	}
}
TEST(MessageIn, BrokenStream) {
	MessageOut msgout{MessageType::ENTITY_CREATE};
	auto teststring = GetLongTestString(3); // let's say it takes a few fragments
	msgout.FromString(teststring);
	MessagePool::list_type msgs = msgout.GetMessages();
	MessageIn msgin;
	FakeSendReceive(msgs, &msgin, [](size_t index, MessagePool::ptr_type& ptr) {
		if (index == 1) {
			ptr = MessagePool::get();
			ptr->SetMessageType(MessageType::SYNC); // let's say something gets misplaced
			ptr->SetBodyLength(5);
		}
	});
	ASSERT_GE(msgs.size(), 2); // we applied the conditions correctly?
	EXPECT_FALSE(msgin.DecodeMessages()); // if so, we should have a broken stream
}
TEST(MessageIn, NoTypeMsg) {
	MessageOut msgout{MessageType::ENTITY_CREATE};
	auto teststring = GetLongTestString(3); // let's say it takes a few fragments
	msgout.FromString(teststring);
	MessagePool::list_type msgs = msgout.GetMessages();
	MessageIn msgin;
	FakeSendReceive(msgs, &msgin, [](size_t index, MessagePool::ptr_type& ptr) {
		ptr->SetMessageType(MULTI_PART); // only multi part messages
	});
	EXPECT_FALSE(msgin.DecodeMessages()); // we should have an "incomplete" stream
}
TEST(MessageIn, MissingSequence) {
	MessageOut msgout{MessageType::ENTITY_CREATE};
	auto teststring = GetLongTestString(5); // let's say it takes a few fragments
	msgout.FromString(teststring);
	MessagePool::list_type msgs = msgout.GetMessages();
	MessageIn msgin;
	FakeSendReceive(msgs, &msgin, [](size_t index, MessagePool::ptr_type& ptr) {
		if (index == 1) { // let's just skip one
			ptr.reset();
		}
	});
	EXPECT_FALSE(msgin.DecodeMessages()); // we should have an "incomplete" sequence because of the missing one
}

TEST(MessageIn, ToOut) {
	MessageOut msgout{MessageType::ENTITY_CREATE};
	auto teststring = GetLongTestString(3); // let's say it takes a few fragments
	msgout.FromString(teststring);
	MessagePool::list_type msgs = msgout.GetMessages();
	MessageIn msgin;
	FakeSendReceive(msgs, &msgin, ReceiveAsIs); // fake send/receive
	EXPECT_TRUE(msgin.DecodeMessages());
	MessageOut msgclone = msgin.ToOut();
	EXPECT_EQ(msgout.ByteCount(), msgclone.ByteCount());
	MessageListContentEqual(msgs, msgclone.GetMessages());
}

} // namespace networking
} // namespace tec
