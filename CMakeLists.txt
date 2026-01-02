cmake_minimum_required(VERSION 3.10)
project(MultiAgentUnitTest)

# Standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================================
# 1. STANDARD COVERAGE FLAGS
# ==========================================
# We use -O0 (Standard Debug) to match the previous separate runs.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# ==========================================
# 2. GOOGLETEST DEPENDENCY
# ==========================================
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

# ==========================================
# 3. SOURCE FILES
# ==========================================
include_directories(src)
add_library(bank_account src/bank_account.cpp)

# ==========================================
# 4. 6-AGENT TARGETS (The Only Ones Active)
# ==========================================

# Bank Account (6-Agent)
add_executable(test_bank_6agent tests/test_bank_account_6agent.cpp)
target_link_libraries(test_bank_6agent PRIVATE bank_account gtest_main)
gtest_discover_tests(test_bank_6agent)

# LRU Cache (6-Agent)
add_executable(test_lru_6agent tests/test_lru_6agent.cpp)
target_include_directories(test_lru_6agent PRIVATE src)
target_link_libraries(test_lru_6agent PRIVATE gtest_main gtest)
gtest_discover_tests(test_lru_6agent)

# Thread Pool (6-Agent)
add_executable(test_threadpool_6agent tests/test_threadpool_6agent.cpp)
target_include_directories(test_threadpool_6agent PRIVATE src)
target_link_libraries(test_threadpool_6agent PRIVATE gtest_main gtest pthread)
gtest_discover_tests(test_threadpool_6agent)
