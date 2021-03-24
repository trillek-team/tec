#include <gtest/gtest.h>
#include <user.hpp>

using namespace tec;

const uid user_id = "TEST-1";
const uid user_name = "TestUser";
const eid entity_id = 1234;

TEST(UserClassTest, Constructor) { User user(entity_id); }

TEST(UserClassTest, SetUserId) {
	User user(entity_id);
	user.SetUserId(user_id);
	EXPECT_EQ(user_id, user.GetUserId());
}

TEST(UserClassTest, SetUsername) {
	User user(entity_id);
	user.SetUsername(user_name);
	EXPECT_EQ(user_name, user.GetUsername());
}

TEST(UserClassTest, Deserialize) {
	proto::User proto_user;
	proto_user.set_id(user_id);
	proto_user.set_username(user_name);
	User user(entity_id);
	user.In(proto_user);
	EXPECT_EQ(user_id, user.GetUserId());
	EXPECT_EQ(user_name, user.GetUsername());
}

TEST(UserClassTest, Serialize) {
	User user(entity_id);
	user.SetUserId(user_id);
	user.SetUsername(user_name);
	proto::User proto_user;
	user.Out(&proto_user);
	EXPECT_EQ(user_id, proto_user.id());
	EXPECT_EQ(user_name, proto_user.username());
}
