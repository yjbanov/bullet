// Copyright 2019 The Bullet Runtime Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include "gtest/gtest.h"

#include "runtime/runtime.h"

class ArenaTest : public testing::Test {
 protected:
  void SetUp() override { bullet::temporary_memory = bullet::Arena(); }
};

TEST_F(ArenaTest, ArenaAllocate) {
  int* pointer = bullet::temporary_memory.allocate<int>();
  EXPECT_TRUE(pointer != nullptr) << "Allocated pointer should not be null";
}

TEST_F(ArenaTest, ArenaZero) {
  for (int i = 0; i < 1000; i++) {
    int* pointer = bullet::temporary_memory.allocate<int>();
    EXPECT_TRUE(pointer != nullptr) << "Allocated pointer should not be null";
    EXPECT_TRUE(*pointer == 0) << "Allocated values should be zero";
  }
}

TEST_F(ArenaTest, ArenaOutOfMemory) {
  bool message_received = false;
  std::string message = "";
  bullet::critical_error_handler = [&](const std::string msg) {
    message_received = true;
    message = msg;
  };
  while (!message_received) {
    int* pointer = bullet::temporary_memory.allocate<int>();
    EXPECT_TRUE(pointer != nullptr) << "Allocated pointer should not be null";
  }
  EXPECT_EQ(message, "Out of memory");
}

TEST_F(ArenaTest, ArenaFree) {
  for (int i = 0; i < 1000; i++) {
    int* pointer = bullet::temporary_memory.allocate<int>();
    *pointer = 1;
  }
  bullet::temporary_memory.free();
  for (int i = 0; i < 1000; i++) {
    int* pointer = bullet::temporary_memory.allocate<int>();
    EXPECT_EQ(*pointer, 0) << "Allocated values should be zero";
  }
}
