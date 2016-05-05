/**
* Unit tests of TEC - FileSystem
*/

#include "filesystem.hpp"

#include <gtest/gtest.h>

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>

TEST(FilePath_class_test, Constructor) {
	using namespace tec;
	// Construction from strings
	std::string bad_path = u8"c:\\usr/share\\MyApp\\foo/bar.png";
	FilePath fp1(bad_path);
#if defined(WIN32)
	ASSERT_STREQ(u8"c:\\usr\\share\\MyApp\\foo\\bar.png", fp1.toString().c_str() );
#else
	ASSERT_STREQ(u8"/usr/share/MyApp/foo/bar.png", fp1.toString().c_str() );
#endif
	FilePath fp2("c:/usr/local/share/");
#if defined(WIN32)
	ASSERT_STREQ(u8"c:\\usr\\local\\share\\", fp2.toString().c_str() );
#else
	ASSERT_STREQ(u8"/usr/local/share/", fp2.toString().c_str() );
#endif

#if defined(WIN32)
	FilePath fp3 = std::wstring(L"c:/usr/local/share/\U20AC");
	ASSERT_STREQ("c:\\usr\\local\\share\\\U20AC", fp3.toString().c_str() );
	ASSERT_STREQ("c:/usr/local/share/\U20AC", fp3.toGenericString().c_str() );
#else
	FilePath fp3 = std::wstring(L"c:/usr/local/share/\u20AC");
	ASSERT_STREQ(u8"/usr/local/share/€", fp3.toString().c_str() );
	ASSERT_STREQ(u8"/usr/local/share/€", fp3.toGenericString().c_str() );
#endif

}

TEST(FilePath_class_test, Operators) {
	using namespace tec;
	FilePath fp1(u8"/usr/");
	fp1 = u8"c:\\windows\\";
#if defined(WIN32)
	ASSERT_EQ(0, fp1.toString().compare(u8"c:\\windows\\"));
#else
	ASSERT_EQ(0, fp1.toString().compare(u8"/windows/"));
#endif

	fp1 = FilePath(u8"/usr/") + "local/";
#if defined(WIN32)
	ASSERT_EQ(0, fp1.toString().compare(u8"\\usr\\local\\"));
#else
	ASSERT_EQ(0, fp1.toString().compare(u8"/usr/local/"));
#endif

	fp1 += "share";
#if defined(WIN32)
	ASSERT_EQ(0, fp1.toString().compare(u8"\\usr\\local\\share"));
#else
	ASSERT_EQ(0, fp1.toString().compare(u8"/usr/local/share"));
#endif

	fp1 /= "trillek";
#if defined(WIN32)
	ASSERT_EQ(0, fp1.toString().compare(u8"\\usr\\local\\share\\trillek"));
#else
	ASSERT_EQ(0, fp1.toString().compare(u8"/usr/local/share/trillek"));
#endif

	fp1 += "/";
	fp1 /= "assets";
#if defined(WIN32)
	ASSERT_EQ(0, fp1.toString().compare(u8"\\usr\\local\\share\\trillek\\assets"));
#else
	ASSERT_EQ(0, fp1.toString().compare(u8"/usr/local/share/trillek/assets"));
#endif
}

TEST(FilePath_class_test, Existence) {
	using namespace tec;
#if defined(WIN32)
	ASSERT_TRUE(FilePath(u8"c:\\windows\\").DirExists());
	ASSERT_FALSE(FilePath(u8"c:\\windows\\foor\\bar\\tardis\\").DirExists());
	ASSERT_TRUE(FilePath(u8"c:\\windows\\notepad.exe").FileExists());
	ASSERT_FALSE(FilePath(u8"c:\\windows\\notepad_caca.exe").FileExists());
#else
	ASSERT_TRUE(FilePath(u8"/home/").DirExists());
	ASSERT_FALSE(FilePath(u8"/fake_folder/foo/bar/").DirExists());
	ASSERT_TRUE(FilePath(u8"/bin/bash").FileExists());
	ASSERT_FALSE(FilePath(u8"/bin/blablafoobar").FileExists());
#endif
}

TEST(FilePath_class_test, Empty) {
	using namespace tec;
	FilePath fp1(u8"c:\\windows\\notepad.exe");
	ASSERT_FALSE(fp1.empty());

	FilePath fp2(""); // Invalid path
	ASSERT_TRUE(fp2.empty());
}

TEST(FilePath_class_test, IsAbsolute) {
	using namespace tec;
	FilePath fp1(u8"c:\\windows\\notepad.exe");
	ASSERT_TRUE(fp1.isAbsolutePath());

	FilePath fp2(u8"../foo/bar/img.png"); // Relative path
	ASSERT_FALSE(fp2.isAbsolutePath());
}

TEST(FilePath_class_test, IsValid) {
	using namespace tec;
	FilePath fp1(u8"c:\\windows\\notepad.exe");
	ASSERT_TRUE(fp1.isValidPath());

	FilePath fp2(u8"../foo/bar/img.png"); // Relative path
	ASSERT_TRUE(fp2.isValidPath());

	FilePath fp3(u8"./foo/bar/img.png"); // Relative path
	ASSERT_TRUE(fp3.isValidPath());

	FilePath fp4(u8"K/bad$#/bar\\img.png"); // Bad Path
	ASSERT_FALSE(fp4.isValidPath());

	FilePath fp5(""); // Bad Path
	ASSERT_FALSE(fp5.isValidPath());;
}

TEST(FilePath_class_test, BasePath) {
	using namespace tec;
#if defined(WIN32)
	FilePath fp1(u8"c:\\windows\\notepad.exe");
	ASSERT_EQ(0, fp1.BasePath().toString().compare(u8"c:\\windows\\"));
	ASSERT_EQ(0, fp1.BasePath().BasePath().toString().compare(u8"c:\\"));
#else
	FilePath fp1(u8"/usr/local/share/MyApp/foo.ini");
	ASSERT_EQ(0, fp1.BasePath().toString().compare(u8"/usr/local/share/MyApp/"));
	ASSERT_EQ(0, fp1.BasePath().BasePath().toString().compare(u8"/usr/local/share/"));
#endif
}

TEST(FilePath_class_test, FileNameAndExtension) {
	using namespace tec;

	FilePath fp1(u8"c:\\windows\\notepad.exe");
	ASSERT_EQ(0, fp1.FileName().compare(u8"notepad.exe"));
	ASSERT_EQ(0, fp1.FileExtension().compare(u8"exe") );
}

TEST(FilePath_class_test, Subpath) {
	using namespace tec;
	
#if defined(WIN32)
	FilePath fp(u8"c:\\usr/local/share/MyApp/foo.ini");
	auto sub1 = fp.Subpath(0, 1);
	auto sub2 = fp.Subpath(0, 2);
	auto sub3 = fp.Subpath(3, 4);
	auto sub4 = fp.Subpath(3);
	ASSERT_EQ(0, sub1.toString().compare(u8"c:"));
	ASSERT_EQ(0, sub2.toString().compare(u8"c:\\usr"));
	ASSERT_EQ(0, sub3.toString().compare(u8"\\share"));
	ASSERT_EQ(0, sub4.toString().compare(u8"\\share\\MyApp\\foo.ini"));

	auto sub5 = fp.SubpathFrom("share", true);
	ASSERT_STREQ(u8"share\\MyApp\\foo.ini", sub5.toString().c_str() );
	auto sub6 = fp.SubpathFrom("share");
	ASSERT_STREQ(u8"MyApp\\foo.ini", sub6.toString().c_str() );
#else
	FilePath fp(u8"/usr/local/share/MyApp/foo.ini");
	auto sub1 = fp.Subpath(0, 1);
	auto sub2 = fp.Subpath(0, 2);
	auto sub3 = fp.Subpath(3, 4);
	auto sub4 = fp.Subpath(3);
	ASSERT_EQ(0, sub1.toString().compare(u8"/"));
	ASSERT_EQ(0, sub2.toString().compare(u8"/usr"));
	ASSERT_EQ(0, sub3.toString().compare(u8"/share"));
	ASSERT_EQ(0, sub4.toString().compare(u8"/share/MyApp/foo.ini"));

	auto sub5 = fp.SubpathFrom("share", true);
	ASSERT_STREQ(u8"share/MyApp/foo.ini", sub5.toString().c_str() );
	auto sub6 = fp.SubpathFrom("share");
	ASSERT_STREQ(u8"MyApp/foo.ini", sub6.toString().c_str() );
#endif
}

TEST(FilePath_class_test, GetProgramPath) {
	using namespace tec;
	auto where = FilePath::GetProgramPath();
	ASSERT_FALSE(where.empty());
	ASSERT_TRUE(where.isValidPath());
	ASSERT_TRUE(where.isAbsolutePath());
	std::cout << "I'm at " << where << "\n";
}

TEST(FilePath_class_test, GetUserPaths) {
	using namespace tec;

	auto settings = FilePath::GetUserSettingsPath();
	auto user_data = FilePath::GetUserDataPath();
	auto cache = FilePath::GetUserCachePath();

	ASSERT_FALSE(settings.empty());
	ASSERT_TRUE(settings.isValidPath());
	ASSERT_TRUE(settings.isAbsolutePath());

	ASSERT_FALSE(user_data.empty());
	ASSERT_TRUE(user_data.isValidPath());
	ASSERT_TRUE(user_data.isAbsolutePath());

	ASSERT_FALSE(cache.empty());
	ASSERT_TRUE(cache.isValidPath());
	ASSERT_TRUE(cache.isAbsolutePath());

	std::cout << FilePath::GetUserSettingsPath() << "\n";
	std::cout << FilePath::GetUserDataPath() << "\n";
	std::cout << FilePath::GetUserCachePath() << "\n";
}

TEST(FilePath_class_test, CreateDir) {
	using namespace tec;
#if defined(WIN32)
	auto fp = FilePath(u8"c:/foo");

#else
	auto fp = FilePath(u8"/foo");
#endif

	ASSERT_TRUE(FilePath::MkPath(fp));
	ASSERT_TRUE(fp.DirExists());
}

TEST(FilePath_class_test, AssetsPath) {
	using namespace tec;
	auto fp = FilePath::GetAssetsBasePath();
	std::cout << fp << "\n";

	FilePath posible_path(u8"./assets");
	if (posible_path.DirExists()) {
		ASSERT_FALSE(fp.empty());
		ASSERT_TRUE(fp.isValidPath());

		auto fp2 = FilePath::GetAssetPath("shaders/debug.vert"); // NOTE Rememeber to change this if debug.vert is moved
		std::cout << fp2 << "\n";
		ASSERT_TRUE(fp2.isValidPath());
		ASSERT_TRUE(fp2.FileExists());
	}
	else {
		std::cout << "Run test from root folder of the project or make a link to assets to check GetAssetsBasePath\n";
	}
}
