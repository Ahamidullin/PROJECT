#include <gtest/gtest.h>
#include "../src/neo.cpp"

// Пример теста 
TEST(SampleTest, AssertionTrue) {
ASSERT_TRUE(true);
}

// Основная функция тестов 
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
