cmake_minimum_required(VERSION 3.14)
project(bank)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
if(NOT googletest_POPULATED)
    FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)
enable_testing()

# --- Libraries & Executables ---

# 1. Bank Account Lib
add_library(bank_account src/bank_account.cpp)
target_include_directories(bank_account PUBLIC src)

# 2. Test: Bank
add_executable(test_bank tests/test_bank_account.cpp)
target_link_libraries(test_bank PRIVATE bank_account gtest_main)
gtest_discover_tests(test_bank)

# 3. Test: LRU
add_executable(test_lru tests/test_lru.cpp)
target_include_directories(test_lru PRIVATE src)
target_link_libraries(test_lru PRIVATE gtest_main gtest)
gtest_discover_tests(test_lru)

# 4. Test: ThreadPool
add_executable(test_threadpool tests/test_threadpool.cpp)
target_include_directories(test_threadpool PRIVATE src)
target_link_libraries(test_threadpool PRIVATE gtest_main gtest pthread)
gtest_discover_tests(test_threadpool)

# ==========================================
# 5. New 3-Agent GPT-4o Test Versions
# ==========================================

# --- A. Bank Account (3-Agent) ---
# Assumes file is named: tests/test_bank_3agent_gpt4o.cpp
add_executable(test_bank_3agent tests/test_bank_3agent_gpt4o.cpp)
target_link_libraries(test_bank_3agent PRIVATE bank_account gtest_main)
gtest_discover_tests(test_bank_3agent)

# --- B. LRU Cache (3-Agent) ---
# Assumes file is named: tests/test_lru_3agent_gpt4o.cpp
add_executable(test_lru_3agent tests/test_lru_3agent_gpt4o.cpp)
target_include_directories(test_lru_3agent PRIVATE src)
target_link_libraries(test_lru_3agent PRIVATE gtest_main gtest)
gtest_discover_tests(test_lru_3agent)

# --- C. ThreadPool (3-Agent) ---
# Assumes file is named: tests/test_threadpool_3agent_gpt4o.cpp
add_executable(test_threadpool_3agent tests/test_threadpool_3agent_gpt4o.cpp)
target_include_directories(test_threadpool_3agent PRIVATE src)
target_link_libraries(test_threadpool_3agent PRIVATE gtest_main gtest pthread)
gtest_discover_tests(test_threadpool_3agent)
