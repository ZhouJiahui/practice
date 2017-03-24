#include <stdlib.h>
#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include "map_client.h"

const int TEST_COUNT = 500;
const std::string STRINGS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123567890"; 
const char *IP = "localhost";
const char *PORT = "2020";
std::string GenerateRandomString() {
  int length = ::rand() % 60 + 1;
  std::string result(length, 0);
  for (size_t i = 0; i <length; ++i) {
    size_t j = ::rand() % STRINGS.size();
    result[i] = STRINGS[j]; 
  }
  return result;
}

void OneThreadRun() {
  for (size_t i = 0; i < TEST_COUNT; ++i) {
    std::string key = GenerateRandomString();
    std::string value = GenerateRandomString();
    MapClient map_client_set(IP, PORT);
    EXPECT_EQ(map_client_set.Set(key, value), std::string("OK"));
    MapClient map_client_get(IP, PORT);
    EXPECT_EQ(map_client_get.Get(key), value);
  } 
}

void SomeThreadRun(size_t n) {
  std::thread threads[n];
  for (size_t i = 0; i < n; ++i) {
    threads[i] = std::thread(OneThreadRun);
  }
  for (size_t i = 0; i < n; ++i) {
    threads[i].join();
  }
}
TEST(MapServerTest, oneThread) {
  OneThreadRun();
}


TEST(MapServerTest, TwoThread) {
  SomeThreadRun(2);
}

TEST(MapServerTest, ThreeThread) {
  SomeThreadRun(3);
}

TEST(MapServerTest, TenThread) {
  SomeThreadRun(10);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);  
  return RUN_ALL_TESTS();
}
