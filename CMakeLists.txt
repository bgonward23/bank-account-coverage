cmake_minimum_required(VERSION 3.14)
project(bank)

set(CMAKE_CXX_STANDARD 17)

# BankAccount library
add_library(bank_account
     src/bank_account.cpp
)

target_include_directories(bank_account PUBLIC
     ${PROJECT_SOURCE_DIR}/src
)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
   googletest
   URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Tests
enable_testing()

add_executable(test_bank
     tests/test_bank_account.cpp
)

target_link_libraries(test_bank
     bank_account
     gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_bank)