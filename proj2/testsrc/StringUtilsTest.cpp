#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    std::string string1 = "abcdefg";
    std::string strtest1 = "cdef";
    std::string strtest2 = "defg";
    std::string string2 = "Hello World!";
    std::string strtest3 = "lo Worl";
    EXPECT_EQ(StringUtils::Slice(string1, 2, 6), strtest1);
    EXPECT_EQ(StringUtils::Slice(string1, 0,0), string1);
    EXPECT_EQ(StringUtils::Slice(string1, 3,0), strtest2);
    EXPECT_EQ(StringUtils::Slice(string2, 3,10), strtest3);
}

TEST(StringUtilsTest, Capitalize){
    std::string string3 = "hello, Test strIng";
    std::string stringtest = "Hello, test string";
    EXPECT_EQ(StringUtils::Capitalize(string3), stringtest);
}

TEST(StringUtilsTest, Upper){
    std::string string2 = "abcDeFg";
    std::string stringtest = "ABCDEFG";
    EXPECT_EQ(StringUtils::Upper(string2), stringtest);
}

TEST(StringUtilsTest, Lower){
    std::string string2 = "aBCDeFg";
    std::string stringtest = "abcdefg";
    EXPECT_EQ(StringUtils::Lower(string2), stringtest);
}

TEST(StringUtilsTest, LStrip){
    std::string string1 = "    Hello   ";
    std::string stringtest = "Hello   ";
    EXPECT_EQ(StringUtils::LStrip(string1), stringtest);
}

TEST(StringUtilsTest, RStrip){
    std::string string1 = "    Hello   ";
    std::string stringtest = "    Hello";
    EXPECT_EQ(StringUtils::RStrip(string1), stringtest);
}

TEST(StringUtilsTest, Strip){
    std::string string1 = "    Hello   ";
    std::string stringtest = "Hello";
    EXPECT_EQ(StringUtils::Strip(string1), stringtest);
}

TEST(StringUtilsTest, Center){
    std::string string1 = "abcdef";
    std::string string2 = "abcdefg";
    std::string stringtest = "       abcdef       ";
    std::string stringtest2 = "      abcdefg       ";
    EXPECT_EQ(StringUtils::Center(string1, 20), stringtest);
    EXPECT_EQ(StringUtils::Center(string2, 20), stringtest2);
}

TEST(StringUtilsTest, LJust){
    std::string string1 = "abcdef";
    std::string stringtest = "abcdef    ";
    EXPECT_EQ(StringUtils::LJust(string1, 10), stringtest);
}

TEST(StringUtilsTest, RJust){
    std::string string1 = "abcdef";
    std::string stringtest = "    abcdef";
    EXPECT_EQ(StringUtils::RJust(string1, 10), stringtest);
}

TEST(StringUtilsTest, Replace){
    std::string string1 = "hello, test string one";
    std::string string2 = "hello, test string test one";
    std::string stringtest = "hello, banana string one";
    std::string stringtest2 = "hello, banana string banana one";
    std::string string3 = "aabbccaaabbbcccaaaa";
    std::string stringtest3 = "eebbcceeabbbccceeee";
    std::string old2 = "aa";
    std::string newstring2 = "ee";
    std::string old = "test";
    std::string newstring = "banana";
    EXPECT_EQ(StringUtils::Replace(string1, old, newstring), stringtest);
    EXPECT_EQ(StringUtils::Replace(string2, old, newstring), stringtest2);
    EXPECT_EQ(StringUtils::Replace(string3, old2, newstring2), stringtest3);
}

TEST(StringUtilsTest, Split){
    std::string string1 = "This string should split on white space";
    std::vector<std::string> answer = {"This", "string", "should", "split", "on", "white", "space"};
    EXPECT_EQ(StringUtils::Split(string1), answer);
    std::string string2 = "travel location";
    std::vector<std::string> answer2 = {"travel l", "cati", "n"};
    EXPECT_EQ(StringUtils::Split(string2, "o"), answer2);
}

TEST(StringUtilsTest, Join){
    std::vector<std::string> vect = {"Happy", "birthday"};
    std::string answer = "Happy birthday";
    EXPECT_EQ(StringUtils::Join(" ", vect), answer);
}

TEST(StringUtilsTest, ExpandTabs){
    std::string string1 = "happy\tbirth\tday";
    std::string answer = "happy   birth   day";
    std::string answer2 = "happy birth day";
    std::string string2 = "1234\t1231\t21";
    std::string answer3 = "1234123121";
    EXPECT_EQ(StringUtils::ExpandTabs(string1, 4), answer);
    EXPECT_EQ(StringUtils::ExpandTabs(string1, 3), answer2);
    EXPECT_EQ(StringUtils::ExpandTabs(string2, 0), answer3);
}

TEST(StringUtilsTest, EditDistance){
    std::string string1 = "kitten";
    std::string answer1 = "sitting";
    std::string string2 = "joyful";
    std::string answer2 = "Joyful";
    EXPECT_EQ(StringUtils::EditDistance(string1, answer1), 3);
    EXPECT_EQ(StringUtils::EditDistance(string2, answer2, true), 0);
}