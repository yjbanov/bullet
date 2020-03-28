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

#include <bitset>
#include <iostream>
#include "gtest/gtest.h"
#include "stddef.h"

#include "runtime/runtime.h"

class PersistentHeapTest : public testing::Test {
 protected:
  void SetUp() override { heap = new bullet::PersistentHeap(); }

  void TearDown() override { delete heap; }

  bullet::PersistentHeap* heap;
};

class TestPersistent : public bullet::PersistentObject {};

TEST_F(PersistentHeapTest, PersistentHeapAllocate) {
  bullet::PersistentObject* pointer = heap->allocate<TestPersistent>();
  EXPECT_NE(pointer, nullptr) << "Allocated pointer should not be null";
}

TEST_F(PersistentHeapTest, PersistentHeapCollect) {
  bool message_received = false;
  std::string message = "";
  bullet::critical_error_handler = [&](const std::string msg) {
    message_received = true;
    message = msg;
  };
  bullet::PersistentObject* pointer = heap->allocate<TestPersistent>();
  heap->collect(pointer);
  EXPECT_TRUE(message_received);
  EXPECT_EQ(message, "Not implemented");
}

class ObjectHeader {
 public:
  unsigned short classIndex;
};

class Bar {
 public:
  ObjectHeader header;
  char letter;
  int* number1;
  bool really;
  int* number2;
};

constexpr size_t pointer_size = sizeof(void*);

constexpr std::bitset<sizeof(unsigned long)> classMaps[1] = {
    0 | 1 << (offsetof(class Bar, number1) / pointer_size) |
    1 << (offsetof(class Bar, number2) / pointer_size)};

TEST_F(PersistentHeapTest, PersistentHeapFieldMap) {
  std::cout << "Bar::header offset is " << offsetof(class Bar, header)
            << std::endl;
  std::cout << "Bar::letter offset is " << offsetof(class Bar, letter)
            << std::endl;
  std::cout << "Bar::number1 offset is " << offsetof(class Bar, number1)
            << std::endl;
  std::cout << "Bar::really offset is " << offsetof(class Bar, really)
            << std::endl;
  std::cout << "Bar::number2 offset is " << offsetof(class Bar, number2)
            << std::endl;

  std::cout << classMaps[0] << std::endl;
}
