/**
 * Unit tests of TEC - FileSystem
 */

#include "filesystem.hpp"

#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

TEST(FilePath_class_test, Constructor) {
	using namespace tec;
	// Construction from strings
	std::string bad_path = "c:\\usr/share\\MyApp\\foo/bar.png";
	FilePath fp1(bad_path);
#if defined(WIN32)
	ASSERT_EQ(fp1.toString(), "c:\\usr\\share\\MyApp\\foo\\bar.png");
#else
	ASSERT_EQ(fp1.toString(), "/usr/share/MyApp/foo/bar.png");
#endif
	FilePath fp2("c:/usr/local/share/");
#if defined(WIN32)
	ASSERT_EQ(fp2.toString(), "c:\\usr\\local\\share\\");
#else
	ASSERT_EQ(fp2.toString(), "/usr/local/share/");
#endif
}

TEST(FilePath_class_test, Operators) {
	using namespace tec;
	FilePath fp1("/usr/");
	fp1 = "c:\\windows\\";
#if defined(WIN32)
	ASSERT_EQ(fp1.toString(), "c:\\windows\\");
#else
	ASSERT_EQ(fp1.toString(), "/windows/");
#endif

	fp1 = FilePath("/usr/") + "local/";
#if defined(WIN32)
	ASSERT_EQ(fp1.toString(), "\\usr\\local\\");
#else
	ASSERT_EQ(fp1.toString(), "/usr/local/");
#endif

	fp1 += "share";
#if defined(WIN32)
	ASSERT_EQ(fp1.toString(), "\\usr\\local\\share");
#else
	ASSERT_EQ(fp1.toString(), "/usr/local/share");
#endif

	fp1 /= "trillek";
#if defined(WIN32)
	ASSERT_EQ(fp1.toString(), "\\usr\\local\\share\\trillek");
#else
	ASSERT_EQ(fp1.toString(), "/usr/local/share/trillek");
#endif

	fp1 += "/";
	fp1 /= "assets";
#if defined(WIN32)
	ASSERT_EQ(fp1.toString(), "\\usr\\local\\share\\trillek\\assets");
#else
	ASSERT_EQ(fp1.toString(), "/usr/local/share/trillek/assets");
#endif
}

TEST(FilePath_class_test, Existence) {
	using namespace tec;
#if defined(WIN32)
	ASSERT_TRUE(FilePath("c:\\windows\\").DirExists());
	ASSERT_FALSE(FilePath("c:\\windows\\foor\\bar\\tardis\\").DirExists());
	ASSERT_TRUE(FilePath("c:\\windows\\notepad.exe").FileExists());
	ASSERT_FALSE(FilePath("c:\\windows\\notepad_caca.exe").FileExists());
#else
	ASSERT_TRUE(FilePath("/home/").DirExists());
	ASSERT_FALSE(FilePath("/fake_folder/foo/bar/").DirExists());
	ASSERT_TRUE(FilePath("/bin/bash").FileExists());
	ASSERT_FALSE(FilePath("/bin/blablafoobar").FileExists());
#endif
}

TEST(FilePath_class_test, Empty) {
	using namespace tec;
	FilePath fp1("c:\\windows\\notepad.exe");
	ASSERT_FALSE(fp1.empty());

	FilePath fp2(""); // Invalid path
	ASSERT_TRUE(fp2.empty());
}

TEST(FilePath_class_test, IsAbsolute) {
	using namespace tec;
	FilePath fp1("c:\\windows\\notepad.exe");
	ASSERT_TRUE(fp1.isAbsolutePath());

	FilePath fp2("../foo/bar/img.png"); // Relative path
	ASSERT_FALSE(fp2.isAbsolutePath());
}

TEST(FilePath_class_test, IsValid) {
	using namespace tec;
	FilePath fp1("c:\\windows\\notepad.exe");
	ASSERT_TRUE(fp1.isValidPath());

	FilePath fp2("../foo/bar/img.png"); // Relative path
	ASSERT_TRUE(fp2.isValidPath());

	FilePath fp3("./foo/bar/img.png"); // Relative path
	ASSERT_TRUE(fp3.isValidPath());

	FilePath fp4("K/bad$#/bar\\img.png"); // Bad Path
	ASSERT_FALSE(fp4.isValidPath());

	FilePath fp5(""); // Bad Path
	ASSERT_FALSE(fp5.isValidPath());
	;
}

TEST(FilePath_class_test, BasePath) {
	using namespace tec;
#if defined(WIN32)
	FilePath fp1("c:\\windows\\notepad.exe");
	ASSERT_EQ(fp1.BasePath().toString(), "c:\\windows\\");
	ASSERT_EQ(fp1.BasePath().BasePath().toString(), "c:\\");
#else
	FilePath fp1("/usr/local/share/MyApp/foo.ini");
	ASSERT_EQ(fp1.BasePath().toString(), "/usr/local/share/MyApp/");
	ASSERT_EQ(fp1.BasePath().BasePath().toString(), "/usr/local/share/");
#endif
}

TEST(FilePath_class_test, FileNameAndExtension) {
	using namespace tec;

	FilePath fp1("c:\\windows\\notepad.exe");
	ASSERT_EQ(fp1.FileName(), "notepad.exe");
	ASSERT_EQ(fp1.FileExtension(), "exe");
}

TEST(FilePath_class_test, Subpath) {
	using namespace tec;

#if defined(WIN32)
	FilePath fp("c:\\usr/local/share/MyApp/foo.ini");
	auto sub1 = fp.Subpath(0, 1);
	auto sub2 = fp.Subpath(0, 2);
	auto sub3 = fp.Subpath(3, 4);
	auto sub4 = fp.Subpath(3);
	ASSERT_EQ(sub1.toString(), "c:");
	ASSERT_EQ(sub2.toString(), "c:\\usr");
	ASSERT_EQ(sub3.toString(), "\\share");
	ASSERT_EQ(sub4.toString(), "\\share\\MyApp\\foo.ini");

	auto sub5 = fp.SubpathFrom("share", true);
	ASSERT_EQ(sub5.toString(), "share\\MyApp\\foo.ini");
	auto sub6 = fp.SubpathFrom("share");
	ASSERT_EQ(sub6.toString(), "MyApp\\foo.ini");
#else
	FilePath fp("/usr/local/share/MyApp/foo.ini");
	auto sub1 = fp.Subpath(0, 1);
	auto sub2 = fp.Subpath(0, 2);
	auto sub3 = fp.Subpath(3, 4);
	auto sub4 = fp.Subpath(3);
	ASSERT_EQ(sub1.toString(), "/");
	ASSERT_EQ(sub2.toString(), "/usr");
	ASSERT_EQ(sub3.toString(), "/share");
	ASSERT_EQ(sub4.toString(), "/share/MyApp/foo.ini");

	auto sub5 = fp.SubpathFrom("share", true);
	ASSERT_EQ(sub5.toString(), "share/MyApp/foo.ini");
	auto sub6 = fp.SubpathFrom("share");
	ASSERT_EQ(sub6.toString(), "MyApp/foo.ini");
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
	auto fp = FilePath("c:/tmp/MyApp/blabla/foo");

#else
	auto fp = FilePath("/tmp/MyApp/blabla/foo");
#endif

	ASSERT_TRUE(FilePath::MkPath(fp));
	ASSERT_TRUE(fp.DirExists());
}

TEST(FilePath_class_test, AssetsPath) {
	using namespace tec;
	auto fp = FilePath::GetAssetsBasePath();
	std::cout << fp << "\n";

	FilePath posible_path("./assets");
	if (posible_path.DirExists()) {
		ASSERT_FALSE(fp.empty());
		ASSERT_TRUE(fp.isValidPath());

		auto fp2 = FilePath::GetAssetPath("shaders/debug.vert"); // NOTE Rememeber to change this if debug.vert is moved
		std::cout << fp2 << "\n";
		ASSERT_TRUE(fp2.isValidPath());
		ASSERT_TRUE(fp2.FileExists());
	}
	else {
		std::cout << "Run test from project root or make a link to assets to check GetAssetsBasePath\n";
	}
}
