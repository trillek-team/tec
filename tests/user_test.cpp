#include <gtest/gtest.h>
#include <user/user.hpp>

using namespace tec;
using namespace tec::user;

const uid user_id = "TEST-1";
const uid user_name = "TestUser";

TEST(UserClassTest, Constructor) { User user; }

TEST(UserClassTest, SetUserId) {
	User user;
	user.SetUserId(user_id);
	EXPECT_EQ(user_id, user.GetUserId());
}

TEST(UserClassTest, SetUsername) {
	User user;
	user.SetUsername(user_name);
	EXPECT_EQ(user_name, user.GetUsername());
}

TEST(UserClassTest, Deserialize) {
	proto::User proto_user;
	proto_user.set_id(user_id);
	proto_user.set_username(user_name);
	User user;
	user.In(proto_user);
	EXPECT_EQ(user_id, user.GetUserId());
	EXPECT_EQ(user_name, user.GetUsername());
}

TEST(UserClassTest, Serialize) {
	User user;
	user.SetUserId(user_id);
	user.SetUsername(user_name);
	proto::User proto_user;
	user.Out(&proto_user);
	EXPECT_EQ(user_id, proto_user.id());
	EXPECT_EQ(user_name, proto_user.username());
}
