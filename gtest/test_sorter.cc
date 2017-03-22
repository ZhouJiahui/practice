#include <gtest/gtest.h>
#include <stdlib.h>
#include <vector>
#include "sorter.h"

std::vector<int> GenerateRandomVector(int n) {
  std::vector<int> numbers(n);
  for (int i = 0; i < n; ++i) {
    numbers[i] = ::rand();
  }
  return numbers;
}

void DoTest(int n) {
  std::vector<int> numbers(GenerateRandomVector(n));
  MySorter my_sorter(numbers);
  my_sorter.DoSort();
  StdSorter std_sorter(numbers);
  std_sorter.DoSort();
  ASSERT_TRUE(my_sorter.GetNumbers() == std_sorter.GetNumbers()); 
}

TEST(SorterTest, zeroNumbers) {
  DoTest(0);
}

TEST(SorterTest, oneNumbers) {
  DoTest(1);
}

TEST(SorterTest, tenNumbers) {
  DoTest(10);
}

TEST(SorterTest, hundredNumbers) {
  DoTest(100);
}

TEST(SorterTest, thousandNumbers) {
  DoTest(1000);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);  

  return RUN_ALL_TESTS();
} 
