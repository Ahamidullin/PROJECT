#include <gtest/gtest.h>
#include "../src/neo.cpp"

TEST(SampleTest, AssertionTrue) {
ASSERT_TRUE(true);
}
//
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
