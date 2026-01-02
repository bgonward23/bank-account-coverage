cmake_minimum_required(VERSION 3.10)
project(MultiAgentUnitTest)

# Standard C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================================
# 1. ENABLE COVERAGE (The Clean Way)
# ==========================================
# We only add basic coverage flags. 
# WE DO NOT add -fno-inline, which causes the 336-line explosion.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# ==========================================
# 2. DEPENDENCIES (GoogleTest)
# ==========================================
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# ==========================================
# 3. SOURCE FILES
# ==========================================
include_directories(src)

# --- Library: Bank Account ---
add_library(bank_account src/bank_account.cpp)

# ==========================================
# 4. TEST EXECUTABLES (Round 1: Copilot)
# ==========================================

# --- A. Bank Account ---
add_executable(test_bank tests/test_bank_account.cpp)
target_link_libraries(test_bank PRIVATE bank_account gtest_main)
gtest_discover_tests(test_bank)

# --- B. LRU Cache ---
add_executable(test_lru tests/test_lru.cpp)
target_include_directories(test_lru PRIVATE src)
target_link_libraries(test_lru PRIVATE gtest_main gtest)
gtest_discover_tests(test_lru)

# --- C. ThreadPool ---
add_executable(test_threadpool tests/test_threadpool.cpp)
target_include_directories(test_threadpool PRIVATE src)
target_link_libraries(test_threadpool PRIVATE gtest_main gtest pthread)
gtest_discover_tests(test_threadpool)

# ==========================================
# 5. GPT-4o Agent Versions (Round 2)
# ==========================================
add_executable(test_bank_3agent tests/test_bank_account_3agent.cpp)
target_link_libraries(test_bank_3agent PRIVATE bank_account gtest_main)
gtest_discover_tests(test_bank_3agent)

add_executable(test_lru_3agent tests/test_lru_3agent.cpp)
target_include_directories(test_lru_3agent PRIVATE src)
target_link_libraries(test_lru_3agent PRIVATE gtest_main gtest)
gtest_discover_tests(test_lru_3agent)

add_executable(test_threadpool_3agent tests/test_threadpool_3agent.cpp)
target_include_directories(test_threadpool_3agent PRIVATE src)
target_link_libraries(test_threadpool_3agent PRIVATE gtest_main gtest pthread)
gtest_discover_tests(test_threadpool_3agent)

# ==========================================
# 6. Qwen Agent Versions (Round 3)
# ==========================================
add_executable(test_bank_qwen tests/test_bank_account_3agent_qwen.cpp)
target_link_libraries(test_bank_qwen PRIVATE bank_account gtest_main)
gtest_discover_tests(test_bank_qwen)

add_executable(test_lru_qwen tests/test_lru_3agent_qwen.cpp)
target_include_directories(test_lru_qwen PRIVATE src)
target_link_libraries(test_lru_qwen PRIVATE gtest_main gtest)
gtest_discover_tests(test_lru_qwen)

add_executable(test_threadpool_qwen tests/test_threadpool_3agent_qwen.cpp)
target_include_directories(test_threadpool_qwen PRIVATE src)
target_link_libraries(test_threadpool_qwen PRIVATE gtest_main gtest pthread)
gtest_discover_tests(test_threadpool_qwen)
