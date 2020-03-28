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

#include "runtime.h"
#include <stdlib.h>
#include <cstring>
#include <iostream>

namespace bullet {
constexpr int _memory_size = 10 * 1024 * 1024;  // 10MB

void print_and_exit(const std::string& message) {
  std::cout << message << std::endl;
  std::exit(1);
}

std::function<void(const std::string&)> critical_error_handler = print_and_exit;

Arena temporary_memory = Arena();

Arena::Arena() {
  _memory =
      calloc(_memory_size, 1);  // as opposed to malloc, this zero-inits memory.
  _pointer = _memory;
}

void* Arena::bump(int object_size) {
  if (static_cast<char*>(_pointer) - static_cast<char*>(_memory) >=
      _memory_size - 1) {
    critical_error_handler("Out of memory");
  }
  void* result = _pointer;
  _pointer = static_cast<char*>(_pointer) + object_size;
  return result;
}

void Arena::free() {
  memset(_memory, 0, _memory_size);
  _pointer = _memory;
}

CollectionResult* PersistentHeap::collect(PersistentObject* root) {
  critical_error_handler("Not implemented");
  return nullptr;
}
}  // namespace bullet
