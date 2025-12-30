cmake_minimum_required(VERSION 3.14)
project(bank)

# 1. Standard Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# Prevent re-downloading if already populated
if(NOT googletest_POPULATED)
    FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)
enable_testing()

# ==========================================
# 3. Main Library (BankAccount)
# ==========================================
add_library(bank_account src/bank_account.cpp)
target_include_directories(bank_account PUBLIC src)

# ==========================================
# 4. Test Executables
# ==========================================

# --- A. Bank Account Test ---
add_executable(test_bank tests/test_bank_account.cpp)
target_link_libraries(test_bank PRIVATE bank_account gtest_main)
gtest_discover_tests(test_bank)

# --- B. LRU Cache Test ---
add_executable(test_lru tests/test_lru.cpp)
target_include_directories(test_lru PRIVATE src)
# REMOVED "--coverage" here. The YAML flags handle it.
target_link_libraries(test_lru PRIVATE gtest_main gtest) 
gtest_discover_tests(test_lru)

# --- C. ThreadPool Test ---
add_executable(test_threadpool tests/test_threadpool.cpp)
target_include_directories(test_threadpool PRIVATE src)
# REMOVED "--coverage" here.
# Added "pthread" because std::thread requires it on Linux
target_link_libraries(test_threadpool PRIVATE gtest_main gtest pthread)
gtest_discover_tests(test_threadpool)
