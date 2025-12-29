#include <gtest/gtest.h>
#include "bank_account.h"
#include <stdexcept>

// Unit Test Fixture
class BankAccountTest : public ::testing::Test {
protected:
     BankAccount account1{"John Doe", 100.0};
     BankAccount account2{"Jane Doe", 50.0};
};

// Test BankAccount constructor
TEST_F(BankAccountTest, ConstructorInitializesCorrectly) {
     EXPECT_EQ(account1.get_balance(), 100.0);
     EXPECT_EQ(account2.get_balance(), 50.0);
}

// Test deposit method with valid input
TEST_F(BankAccountTest, DepositValidAmount) {
     account1.deposit(50.0);
     EXPECT_EQ(account1.get_balance(), 150.0);
}

// Test deposit method with zero amount (edge case)
TEST_F(BankAccountTest, DepositZeroThrowsException) {
     EXPECT_THROW(account1.deposit(0.0), std::invalid_argument);
}

// Test deposit method with negative amount (edge case)
TEST_F(BankAccountTest, DepositNegativeThrowsException) {
     EXPECT_THROW(account1.deposit(-10.0), std::invalid_argument);
}

// Test withdraw method with valid input
TEST_F(BankAccountTest, WithdrawValidAmount) {
     account1.withdraw(50.0);
     EXPECT_EQ(account1.get_balance(), 50.0);
}

// Test withdraw method with zero amount (edge case)
TEST_F(BankAccountTest, WithdrawZeroThrowsException) {
     EXPECT_THROW(account1.withdraw(0.0), std::invalid_argument);
}

// Test withdraw method with negative amount (edge case)
TEST_F(BankAccountTest, WithdrawNegativeThrowsException) {
     EXPECT_THROW(account1.withdraw(-10.0), std::invalid_argument);
}

// Test withdraw method with amount greater than balance
TEST_F(BankAccountTest, WithdrawExceedsBalanceThrowsException) {
     EXPECT_THROW(account1.withdraw(200.0), std::runtime_error);
}

// Test transfer method with valid input
TEST_F(BankAccountTest, TransferValidAmount) {
     account1.transfer(50.0, account2);
     EXPECT_EQ(account1.get_balance(), 50.0);
     EXPECT_EQ(account2.get_balance(), 100.0);
}

// Test transfer method with amount exceeding balance
TEST_F(BankAccountTest, TransferExceedsBalanceThrowsException) {
     EXPECT_THROW(account1.transfer(200.0, account2),
std::runtime_error);
}

// Test transfer method to the same account (edge case)
TEST_F(BankAccountTest, SelfTransferThrowsException) {
     EXPECT_THROW(account1.transfer(50.0, account1),
std::invalid_argument);
}

// Test transfer method with zero amount (edge case)
TEST_F(BankAccountTest, TransferZeroThrowsException) {
     EXPECT_THROW(account1.transfer(0.0, account2),
std::invalid_argument);
}

// Test transfer method with negative amount (edge case)
TEST_F(BankAccountTest, TransferNegativeThrowsException) {
     EXPECT_THROW(account1.transfer(-10.0, account2),
std::invalid_argument);
}
