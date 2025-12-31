#include "bank_account.h"
#include <gtest/gtest.h>
#include <stdexcept>

// Test fixture for BankAccount class
class BankAccountTest : public ::testing::Test {
protected:
    void SetUp() override {
        account1 = std::make_unique<BankAccount>("Alice", 100.0);
        account2 = std::make_unique<BankAccount>("Bob", 50.0);
    }

    std::unique_ptr<BankAccount> account1;
    std::unique_ptr<BankAccount> account2;
};

// Test constructor
TEST_F(BankAccountTest, ConstructorInitializesValuesCorrectly) {
    EXPECT_EQ(account1->get_balance(), 100.0);
    EXPECT_EQ(account2->get_balance(), 50.0);
}

// Test deposit with valid amount
TEST_F(BankAccountTest, DepositValidAmountIncreasesBalance) {
    account1->deposit(50.0);
    EXPECT_DOUBLE_EQ(account1->get_balance(), 150.0);
}

// Test deposit zero amount throws exception
TEST_F(BankAccountTest, DepositZeroAmountThrowsException) {
    EXPECT_THROW(account1->deposit(0.0), std::invalid_argument);
}

// Test deposit negative amount throws exception
TEST_F(BankAccountTest, DepositNegativeAmountThrowsException) {
    EXPECT_THROW(account1->deposit(-10.0), std::invalid_argument);
}

// Test withdraw valid amount
TEST_F(BankAccountTest, WithdrawValidAmountDecreasesBalance) {
    account1->withdraw(30.0);
    EXPECT_DOUBLE_EQ(account1->get_balance(), 70.0);
}

// Test withdraw zero amount throws exception
TEST_F(BankAccountTest, WithdrawZeroAmountThrowsException) {
    EXPECT_THROW(account1->withdraw(0.0), std::invalid_argument);
}

// Test withdraw negative amount throws exception
TEST_F(BankAccountTest, WithdrawNegativeAmountThrowsException) {
    EXPECT_THROW(account1->withdraw(-5.0), std::invalid_argument);
}

// Test withdraw more than balance throws exception
TEST_F(BankAccountTest, WithdrawExceedingBalanceThrowsException) {
    EXPECT_THROW(account1->withdraw(150.0), std::runtime_error);
}

// Test withdraw entire balance
TEST_F(BankAccountTest, WithdrawFullBalance) {
    account1->withdraw(100.0);
    EXPECT_DOUBLE_EQ(account1->get_balance(), 0.0);
}

// Test get_balance returns correct value
TEST_F(BankAccountTest, GetBalanceReturnsCurrentBalance) {
    EXPECT_DOUBLE_EQ(account1->get_balance(), 100.0);
    account1->deposit(25.0);
    EXPECT_DOUBLE_EQ(account1->get_balance(), 125.0);
}

// Test transfer to another account
TEST_F(BankAccountTest, TransferAmountToAnotherAccount) {
    account1->transfer(30.0, *account2);
    EXPECT_DOUBLE_EQ(account1->get_balance(), 70.0);
    EXPECT_DOUBLE_EQ(account2->get_balance(), 80.0);
}

// Test transfer entire balance to another account
TEST_F(BankAccountTest, TransferFullBalanceToAnotherAccount) {
    account1->transfer(100.0, *account2);
    EXPECT_DOUBLE_EQ(account1->get_balance(), 0.0);
    EXPECT_DOUBLE_EQ(account2->get_balance(), 150.0);
}

// Test transfer zero amount throws exception
TEST_F(BankAccountTest, TransferZeroAmountThrowsException) {
    EXPECT_THROW(account1->transfer(0.0, *account2), std::invalid_argument);
}

// Test transfer negative amount throws exception
TEST_F(BankAccountTest, TransferNegativeAmountThrowsException) {
    EXPECT_THROW(account1->transfer(-10.0, *account2), std::invalid_argument);
}

// Test transfer exceeding balance throws exception
TEST_F(BankAccountTest, TransferExceedingBalanceThrowsException) {
    EXPECT_THROW(account1->transfer(150.0, *account2), std::runtime_error);
}

// Test transfer to same account throws exception
TEST_F(BankAccountTest, TransferToSameAccountThrowsException) {
    EXPECT_THROW(account1->transfer(10.0, *account1), std::invalid_argument);
}

// Test edge case: very small deposit amount
TEST_F(BankAccountTest, DepositVerySmallAmount) {
    account1->deposit(0.001);
    EXPECT_NEAR(account1->get_balance(), 100.001, 1e-9);
}

// Test edge case: very small withdrawal amount
TEST_F(BankAccountTest, WithdrawVerySmallAmount) {
    account1->withdraw(0.001);
    EXPECT_NEAR(account1->get_balance(), 99.999, 1e-9);
}

// Test multiple operations in sequence
TEST_F(BankAccountTest, MultipleOperationsSequence) {
    account1->deposit(50.0);      // 150.0
    account1->withdraw(25.0);     // 125.0
    account1->transfer(25.0, *account2); // 100.0 and 75.0
    EXPECT_DOUBLE_EQ(account1->get_balance(), 100.0);
    EXPECT_DOUBLE_EQ(account2->get_balance(), 75.0);
}
