#include <gtest/gtest.h>
#include "Path.h"

TEST(PathTest, StringTest){
    CPath EmptyPath, RelPath("./foo/bar"), AbsPath("/usr/bin");
    
    EXPECT_EQ(std::string(EmptyPath), "");
    EXPECT_EQ(std::string(RelPath), "./foo/bar");
    EXPECT_EQ(std::string(AbsPath), "/usr/bin");
}

TEST(PathTest, AttributeTest){
    CPath EmptyPath, RelPath("./foo/bar"), AbsPath("/usr/bin"), FilePath("./testpath.cpp");
    
    EXPECT_EQ(std::string(EmptyPath.Directory()), "");
    EXPECT_EQ(std::string(RelPath.Directory()), "./foo");
    EXPECT_EQ(std::string(AbsPath.Directory()), "/usr");
    EXPECT_EQ(std::string(FilePath.Directory()), ".");
    EXPECT_EQ(EmptyPath.BaseName(), "");
    EXPECT_EQ(RelPath.BaseName(), "bar");
    EXPECT_EQ(AbsPath.BaseName(), "bin");
    EXPECT_EQ(FilePath.BaseName(), "testpath.cpp");
    EXPECT_EQ(AbsPath.Extension(), "");
    EXPECT_EQ(FilePath.Extension(), ".cpp");
    
    EXPECT_FALSE(EmptyPath.IsAbsolute());
    EXPECT_FALSE(RelPath.IsAbsolute());
    EXPECT_TRUE(AbsPath.IsAbsolute());
    EXPECT_FALSE(FilePath.IsAbsolute());
}

TEST(PathTest, PathOpTest){
    CPath EmptyPath, RelPath("./foo/bar"), AbsPath("/usr/bin"), FilePath("./foo/testpath.cpp");
        
    EXPECT_EQ(std::string(AbsPath + FilePath), "/usr/bin/./foo/testpath.cpp");
    EXPECT_TRUE( FilePath.AbsolutePath() == CPath::AbsolutePath( FilePath));
    
    EXPECT_TRUE(CPath::ExpandUserPath(CPath("~/foo")) == CPath::NormalizePath(CPath::HomePath() + CPath("./foo")));
    EXPECT_EQ(std::string((AbsPath + FilePath).NormalizePath()), "/usr/bin/foo/testpath.cpp");
    auto Path1 = AbsPath + RelPath;///usr/bin./foo/bar
    auto Path2 = AbsPath + FilePath;// /usr/bin./foo/testpath.cpp
    EXPECT_EQ(std::string(Path1.RelativePathTo(Path2)), "../testpath.cpp");

    CPath testNormalizePath1("../test"), testNormalizePath2("a/foo/../b"), testNormalizePath3("a//b");
    EXPECT_EQ(std::string(testNormalizePath1.NormalizePath()), "../test");
    EXPECT_EQ(std::string(testNormalizePath2.NormalizePath()), "a/b");
    EXPECT_EQ(std::string(testNormalizePath3.NormalizePath()), "a/b");

    CPath A("/home/cjnitta/ecs34/path1"), B("/home/cjnitta/ecs160/path2");

    EXPECT_EQ(std::string(A.RelativePathTo(B)),"../../ecs160/path2");


}
