#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include "save-game.hpp"

using namespace tec;

const FilePath save_directory = FilePath::GetAssetPath("save_test_123456");
const FilePath save_file_path = save_directory + "/save.json";
const std::string
		contents("{\"users\":[{\"id\":\"user-1\",\"username\":\"John\"}], \"world\":{\"entityFileList\":[]}}");
const eid entity_id = 1234;

class SaveGameTest : public ::testing::Test {
protected:
	void SetUp() override {
		ASSERT_TRUE(FilePath::MkDir(save_directory));
		std::fstream output(save_file_path.GetNativePath(), std::ios::out);
		ASSERT_TRUE(output.good());
		output.write(contents.c_str(), contents.length());
		output.close();
	}

	void TearDown() override {
		try {
			std::filesystem::remove_all(save_directory.toString());
		}
		catch (std::filesystem::filesystem_error err) {
			std::cout << "Failed to clean up - " << err.code() << ":" << err.what() << std::endl;
		}
	}
};
TEST_F(SaveGameTest, Constructor) { SaveGame save_game; }

TEST_F(SaveGameTest, DiskIO) {
	// Sanity check
	ASSERT_TRUE(save_directory.DirExists());

	uid user2_id = "user-2";
	{
		SaveGame save_game;
		EXPECT_NO_THROW(save_game.Load(save_file_path));
		User user2;
		user2.SetUserId(user2_id);
		user2.SetUsername(user2_id);
		save_game.GetUserList()->AddUser(user2);
		EXPECT_NO_THROW(save_game.Save());
	}
	{
		SaveGame save_game;
		save_game.Load(save_file_path);
		EXPECT_TRUE(save_game.GetUserList()->HasUser(user2_id));
	}
}

TEST_F(SaveGameTest, State) {
	SaveGame save_game;
	save_game.Load(save_file_path);

	EXPECT_EQ(save_game.GetUserList()->GetUsers()->size(), 1);
}

TEST(UserList_class_test, Constructor) {
	UserList user_list;

	EXPECT_EQ(user_list.GetUsers()->size(), 0);
}

TEST(UserList_class_test, AddUser) {
	UserList user_list;
	auto users = user_list.GetUsers();
	User user;
	user.SetUserId("user-1");

	user_list.AddUser(user);
	EXPECT_EQ(users->size(), 1);
}

TEST(UserList_class_test, UserExists) {
	UserList user_list;
	auto users = user_list.GetUsers();
	uid user_id = "user-1";
	User user;
	user.SetUserId(user_id);

	user_list.AddUser(user);
	EXPECT_TRUE(user_list.HasUser(user_id));

	uid invalid_user_id = "user-2";
	EXPECT_FALSE(user_list.HasUser(invalid_user_id));
}

TEST(UserList_class_test, RemoveUser) {
	UserList user_list;
	auto users = user_list.GetUsers();
	uid user_id = "user-1";
	User user;
	user.SetUserId(user_id);

	user_list.AddUser(user);
	EXPECT_EQ(users->size(), 1);

	uid invalid_user_id = "user-2";
	user_list.RemoveUser(invalid_user_id);
	EXPECT_EQ(users->size(), 1);

	user_list.RemoveUser(user_id);
	EXPECT_EQ(users->size(), 0);
}

TEST(UserList_class_test, UpdateUser) {
	UserList user_list;
	uid user_id = "user-1";
	auto users = user_list.GetUsers();
	{
		User user;
		user.SetUserId(user_id);
		user_list.AddUser(user);
	}

	auto user = user_list.GetUser(user_id);
	uid user_id2 = "user-2";
	user->SetUserId(user_id2);

	EXPECT_FALSE(user_list.HasUser(user_id));
	EXPECT_TRUE(user_list.HasUser(user_id2));
}
