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

#ifndef BULLET_RUNTIME
#define BULLET_RUNTIME

#include <functional>

namespace bullet {

// Called when an unrecoverable runtime error happens, such as out of memory
// error.
//
// The default implementation prints the error and exits the process with exit
// code 1.
extern std::function<void(const std::string&)> critical_error_handler;

class Arena {
 public:
  Arena();

  template <typename T>
  T* allocate() {
    return static_cast<T*>(bump(sizeof(T)));
  }

  // Frees the entire chunk of memory and zeros out its bytes.
  //
  // This is relatively expensive and should only be called when reusing a
  // temporary memory previously populated with objects. However, this is
  // cheaper than allocating a new memory.
  void free();

 private:
  void* bump(int object_size);
  void* _pointer;
  void* _memory;
};

extern Arena temporary_memory;

class PersistentObject {};

class PersistentHeap;

class CollectionResult {
 public:
  PersistentHeap* newHeap;
  PersistentObject* newRoot;
};

// Garbage-collected persistent memory.
class PersistentHeap {
 public:
  // Stores objects.
  Arena memory;

  PersistentHeap(){};

  // Scans this heap and produces a new heap containing copies of live
  // objects.
  //
  // An object is _live_ if it is reachable from the [root].
  //
  // The returned heap is compact, i.e. it does not contain gaps between
  // live objects.
  CollectionResult* collect(PersistentObject* root);

  template <typename T>
  PersistentObject* allocate() {
    // This is poor man's <T extends PersistentObject> bounds checker.
    // TODO(yjbanov): find a better way to do this.
    if (static_cast<const PersistentObject*>(static_cast<const T*>(NULL)) !=
        nullptr) {
      // Should never reach this code.
      std::abort();
    }
    return memory.allocate<T>();
  }
};
}  // namespace bullet

#endif  // BULLET_RUNTIME
