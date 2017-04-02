#include <gtest/gtest.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <mutex>
#include <thread>
#include <functional>

const unsigned long long COUNT = 500000000ULL;

TEST(CountTest, oneThreadSimple) {
  unsigned long long count = 0;
  for (; count < COUNT; ++count);
  ASSERT_TRUE(count == COUNT); 
}

TEST(CountTest, oneThreadLock) {
  std::mutex mtx;
  unsigned long long count = 0;
  for (; count < COUNT; ++count, mtx.unlock()) {
    mtx.lock();
  }
  ASSERT_TRUE(count == COUNT); 
}

void func2(std::mutex* mtx, unsigned long long* count) {
  for (; *count < COUNT; ++*count, mtx->unlock()) {
    mtx->lock();
  }
}
void func(std::mutex* mtx, unsigned long long* count) {
  while (true) {
    mtx->lock();
    if (*count >= COUNT) {
      mtx->unlock();
      break;
    }
    ++*count;
    mtx->unlock();
  }
}

TEST(CountTest, twoThreadLock) {
  std::mutex mtx;
  unsigned long long count = 0;
  std::thread first(std::bind(&func, &mtx, &count));
  std::thread second(std::bind(&func, &mtx, &count));
  first.join();
  second.join();
  ASSERT_TRUE(count == COUNT);
}

TEST(CountTest, twoThreadLock2) {
  std::mutex mtx;
  unsigned long long count = 0;
  std::thread first(std::bind(&func2, &mtx, &count));
  std::thread second(std::bind(&func2, &mtx, &count));
  first.join();
  second.join();
  ASSERT_TRUE(count == COUNT);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);  

  return RUN_ALL_TESTS();
}
