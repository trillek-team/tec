/**
 * Unit tests of TEC - FileSystem
 */

#include "filesystem.hpp"

#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

TEST(Path_class_test, Constructor) {
	using namespace tec;
	// Construction from strings
	const std::string_view messy_path_sv = "c:\\usr/share\\MyApp\\foo/bar.png";
	std::string messy_path_s{messy_path_sv};
	ASSERT_EQ(Path(messy_path_sv).toString(), "c:/usr/share/MyApp/foo/bar.png");
	ASSERT_EQ(Path(messy_path_s).toString(), "c:/usr/share/MyApp/foo/bar.png");
	ASSERT_EQ(Path(std::move(messy_path_s)).toString(), "c:/usr/share/MyApp/foo/bar.png");
	ASSERT_EQ(Path("c:/usr/local/share/").toString(), "c:/usr/local/share/");
	ASSERT_EQ(Path("./usr/foo/bar/").toString(), "usr/foo/bar/");
	ASSERT_EQ(Path("usr/././foo/bar/").toString(), "usr/foo/bar/");
	ASSERT_EQ(Path("/usr/././foo/bar").toString(), "/usr/foo/bar");
	ASSERT_EQ(Path(".").toString(), ".");
	ASSERT_EQ(Path("/./").toString(), "/");
	ASSERT_EQ(Path("/.").toString(), "/");
	ASSERT_EQ(Path("./").toString(), "./");
	ASSERT_EQ(Path("./.").toString(), ".");
	ASSERT_EQ(Path("../").toString(), "../");
	ASSERT_EQ(Path("/foo.").toString(), "/foo");
	ASSERT_EQ(Path("/.foo.").toString(), "/.foo");
	ASSERT_EQ(Path("/../.././bar").toString(), "/bar");
	ASSERT_EQ(Path("usr/././.././foo/bar/").toString(), "foo/bar/");
	ASSERT_EQ(Path("usr/././../../foo/bar/").toString(), "../foo/bar/");
}

TEST(Path_class_test, Operators) {
	using namespace tec;
	Path fp1("/usr/");
	fp1 = "c:\\windows\\";
	ASSERT_EQ(fp1.toString(), "c:/windows/");
	fp1 = "empty";
	ASSERT_EQ(fp1.toString(), "empty");
	ASSERT_THROW(fp1 /= "empty?", PathException);
	fp1 = "empty?";
	try {
		fp1 /= "?";
	}
	catch (PathException& e) {
		std::cout << e.what() << '\n';
	}
	ASSERT_FALSE(fp1);
	fp1 /= "stillbad";
	ASSERT_FALSE(fp1);

	fp1 = Path("/usr/") / "local/";
	ASSERT_TRUE(fp1);
	ASSERT_EQ(fp1.toString(), "/usr/local/");
	fp1 /= "share";
	ASSERT_EQ(fp1.toString(), "/usr/local/share");
	fp1 /= "/trillek/";
	ASSERT_EQ(fp1.toString(), "/usr/local/share/trillek/");
	fp1 /= "/assets";
	ASSERT_EQ(fp1.toString(), "/usr/local/share/trillek/assets");

	Path fp2 = ((Path("/usr/") / Path("/local/")) / "share" / "\\trillek" / "/") / "assets";
	ASSERT_EQ(fp2.toString(), "/usr/local/share/trillek/assets");
}

TEST(Path_class_test, Existence) {
	using namespace tec;
#if defined(WIN32)
	ASSERT_TRUE(Path("c:\\Windows\\").DirExists());
	ASSERT_FALSE(Path("c:\\Windows\\foor\\bar\\tardis\\").DirExists());
	ASSERT_TRUE(Path("c:\\Windows\\notepad.exe").FileExists());
	ASSERT_FALSE(Path("c:\\Windows\\notepad_caca.exe").FileExists());
#else
	ASSERT_TRUE(Path("/home/").DirExists());
	ASSERT_FALSE(Path("/fake_folder/foo/bar/").DirExists());
	ASSERT_FALSE(Path("/bin/bash").DirExists());
	ASSERT_TRUE(Path("/bin/bash").FileExists());
	ASSERT_FALSE(Path("/bin/blablafoobar").FileExists());
#endif
}

TEST(Path_class_test, Empty) {
	using namespace tec;
	Path fp1("c:\\windows\\notepad.exe");
	ASSERT_FALSE(fp1.empty());

	Path fp2(""); // Invalid path
	ASSERT_TRUE(fp2.empty());
}

TEST(Path_class_test, Normalize) {
	using namespace tec;
	ASSERT_EQ(Path("./").toString(), "./");
}

TEST(Path_class_test, IsAbsolute) {
	using namespace tec;
	ASSERT_TRUE(Path("c:\\windows\\notepad.exe").isAbsolutePath());
	ASSERT_TRUE(Path("\\windows\\notepad.exe").isAbsolutePath());
	ASSERT_TRUE(Path("/windows/notepad.exe").isAbsolutePath());

	// Relative paths
	ASSERT_FALSE(Path("../foo/bar/img.png").isAbsolutePath());
	ASSERT_FALSE(Path("foo\\bar\\img.png").isAbsolutePath());
	ASSERT_FALSE(Path("./img.png").isAbsolutePath());
	ASSERT_FALSE(Path("img.png").isAbsolutePath());
}

TEST(Path_class_test, IsValid) {
	using namespace tec;
	// Absolute with windows device
	ASSERT_TRUE(Path("c:\\windows\\notepad.exe"));
	// Absolute windows style
	ASSERT_TRUE(Path("\\windows\\notepad.exe"));
	// Absolute unix style
	ASSERT_TRUE(Path("/windows/notepad.exe"));
	// Relative path
	ASSERT_TRUE(Path("../foo/bar/img.png"));
	// Relative path
	ASSERT_TRUE(Path("./foo/bar/img.png"));
	// Strange Relative Path
	// yes this is considered a valid path, just don't pass it as-is through a shell
	ASSERT_TRUE(Path("K/ree$#/[bar ~=` %!@;#$%^&'()_+-]{}\\img.png"));
	// Bad Relative Path
	// this path contains filesystem reserved symbols
	// would not be valid in NTFS nor HFS, although the ext* filesystem would allow this
	ASSERT_FALSE(Path("/file>bad"));
	ASSERT_FALSE(Path("/file<bad"));
	// outlier here is Windows, other systems consider ':' valid
	EXPECT_FALSE(Path("\\:filebad"));
	EXPECT_FALSE(Path(":filebad"));
	EXPECT_FALSE(Path("::filebad"));
	EXPECT_FALSE(Path("f:il:ebad"));
	EXPECT_FALSE(Path("f:\\ilebad:"));
	ASSERT_TRUE(Path("f:"));
	ASSERT_TRUE(Path("f:\\ilegood"));
	ASSERT_FALSE(Path("/file*bad"));
	ASSERT_FALSE(Path("file*bad"));
	ASSERT_FALSE(Path("/file?bad"));
	ASSERT_FALSE(Path("file?bad"));
	ASSERT_FALSE(Path("/file\"bad"));
	ASSERT_FALSE(Path("file\"bad"));
	ASSERT_FALSE(Path("/file|bad"));
	ASSERT_FALSE(Path("file|bad"));
	ASSERT_FALSE(Path("file\001bad"));
	// Empty is a bad path
	ASSERT_FALSE(Path(""));
	// Relative path directly to a file
	ASSERT_TRUE(Path("img.png"));
	// Relative path to a file without an extension
	ASSERT_TRUE(Path("img"));
}

TEST(Path_class_test, BasePath) {
	using namespace tec;
	Path fp1("c:\\windows\\notepad.exe");
	ASSERT_EQ(fp1.BasePath().toString(), "c:/windows") << fp1;
	ASSERT_EQ(fp1.BasePath().BasePath().toString(), "c:/") << fp1.BasePath();
	ASSERT_EQ(fp1.BasePath().BasePath().BasePath().toString(), "c:/") << fp1.BasePath().BasePath().BasePath();

	Path fp2("/usr/local/share/MyApp/foo.ini");
	ASSERT_EQ(fp2.BasePath().toString(), "/usr/local/share/MyApp");
	ASSERT_EQ(fp2.BasePath().BasePath().toString(), "/usr/local/share");
}

TEST(Path_class_test, FileNameAndExtension) {
	using namespace tec;

	Path fp1("c:\\windows\\notepad.exe");
	ASSERT_EQ(fp1.FileName(), "notepad.exe");
	ASSERT_EQ(fp1.FileStem(), "notepad");
	ASSERT_EQ(fp1.FileExtension(), "exe");
	ASSERT_EQ(Path("notepad.exe").FileName(), "notepad.exe");
	ASSERT_EQ(Path("notepad.exe").FileStem(), "notepad");
	ASSERT_EQ(Path("notepad.exe").FileExtension(), "exe");
}

TEST(Path_class_test, Subpath) {
	using namespace tec;

	Path fp("c:/usr/local/share/MyApp/foo.ini");
	ASSERT_EQ(fp.Subpath(0, 1).toString(), "c:/");
	ASSERT_EQ(fp.Subpath(0, 2).toString(), "c:/usr");
	// if we are not at the beginning of an absolute path, then we should have a relative path returned
	ASSERT_EQ(fp.Subpath(3, 4).toString(), "share");
	ASSERT_EQ(fp.Subpath(3).toString(), "share/MyApp/foo.ini");
	ASSERT_EQ(Path("./assets/foo/bar/mesh.obj").Subpath(2, Path::npos).toString(), "bar/mesh.obj");

	ASSERT_EQ(fp.SubpathFrom("share", true).toString(), "share/MyApp/foo.ini");
	ASSERT_EQ(fp.SubpathFrom("share").toString(), "MyApp/foo.ini");
	ASSERT_EQ(fp.SubpathFrom("nowhere").toString(), "");
}

TEST(Path_class_test, GetProgramPath) {
	using namespace tec;
	auto where = Path::GetProgramPath();
	ASSERT_FALSE(where.empty());
	ASSERT_TRUE(where) << "where=" << where.toString();
	ASSERT_TRUE(where.isAbsolutePath());
	std::cout << "I'm at " << where << "\n";
}

TEST(Path_class_test, GetUserPaths) {
	using namespace tec;

	auto settings = Path::GetUserSettingsPath();
	auto user_data = Path::GetUserDataPath();
	auto cache = Path::GetUserCachePath();

	ASSERT_FALSE(settings.empty());
	ASSERT_TRUE(settings);
	ASSERT_TRUE(settings.isAbsolutePath());

	ASSERT_FALSE(user_data.empty());
	ASSERT_TRUE(user_data);
	ASSERT_TRUE(user_data.isAbsolutePath());

	ASSERT_FALSE(cache.empty());
	ASSERT_TRUE(cache);
	ASSERT_TRUE(cache.isAbsolutePath());

	std::cout << "Settings : " << Path::GetUserSettingsPath() << "\n";
	std::cout << "User Data: " << Path::GetUserDataPath() << "\n";
	std::cout << "Cache    : " << Path::GetUserCachePath() << "\n";
}

TEST(Path_class_test, CreateDir) {
	using namespace tec;
	auto fp = Path::GetProgramPath() / "../MyApp/blabla/foo";
	ASSERT_TRUE(fp) << "fp=" << fp;
	ASSERT_TRUE(fp.isAbsolutePath());
	ASSERT_TRUE(Path::MkPath(fp)) << "fp=" << fp;
	ASSERT_TRUE(fp.DirExists());
}

TEST(Path_class_test, AssetsPath) {
	using namespace tec;
	auto fp = Path::GetAssetsBasePath();
	std::cout << "Assets: " << fp << "\n";

	Path posible_path("assets");
	if (posible_path.DirExists()) {
		ASSERT_FALSE(fp.empty());
		ASSERT_TRUE(fp);

		auto fp2 = Path::shaders / "core.json"; // This file is required to load all the shaders
		std::cout << "Shaders path: " << fp2 << "\n";
		ASSERT_TRUE(fp2);
		ASSERT_TRUE(fp2.FileExists());
	}
	else {
		std::cout << "Run test from project root or make a link to assets to check GetAssetsBasePath\n";
	}
}
