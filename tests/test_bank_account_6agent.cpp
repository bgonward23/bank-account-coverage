#include <gtest/gtest.h>
#include <stdexcept>
#include "bank_account.h" // Assuming the implementation is in this header file

// Test fixture for BankAccount tests
class BankAccountTest : public ::testing::Test {
protected:
    BankAccount* account;
    BankAccount* target_account;

    void SetUp() override {
        account = new BankAccount("Alice", 1000.0);
        target_account = new BankAccount("Bob", 500.0);
    }

    void TearDown() override {
        delete account;
        delete target_account;
    }
};

// Nominal Test Cases
TEST_F(BankAccountTest, UT_BankAccountGetBalance_InitialBalance) {
    EXPECT_EQ(account->get_balance(), 1000.0);
}

TEST_F(BankAccountTest, UT_BankAccountDeposit_ValidAmount) {
    account->deposit(500.0);
    EXPECT_EQ(account->get_balance(), 1500.0);
}

TEST_F(BankAccountTest, UT_BankAccountWithdraw_ValidAmount) {
    account->withdraw(300.0);
    EXPECT_EQ(account->get_balance(), 700.0);
}

TEST_F(BankAccountTest, UT_BankAccountTransfer_ValidAmount) {
    account->transfer(200.0, *target_account);
    EXPECT_EQ(account->get_balance(), 800.0);
    EXPECT_EQ(target_account->get_balance(), 700.0);
}

// Boundary Conditions
TEST_F(BankAccountTest, UT_BankAccountDeposit_MinimumAmount) {
    account->deposit(0.01);
    EXPECT_EQ(account->get_balance(), 1000.01);
}

TEST_F(BankAccountTest, UT_BankAccountWithdraw_EntireBalance) {
    account->withdraw(1000.0);
    EXPECT_EQ(account->get_balance(), 0.0);
}

TEST_F(BankAccountTest, UT_BankAccountTransfer_EntireBalance) {
    account->transfer(1000.0, *target_account);
    EXPECT_EQ(account->get_balance(), 0.0);
    EXPECT_EQ(target_account->get_balance(), 1500.0);
}

TEST_F(BankAccountTest, UT_BankAccountDeposit_ZeroAmount) {
    EXPECT_THROW(account->deposit(0.0), std::invalid_argument);
}

TEST_F(BankAccountTest, UT_BankAccountWithdraw_ZeroAmount) {
    EXPECT_THROW(account->withdraw(0.0), std::invalid_argument);
}

TEST_F(BankAccountTest, UT_BankAccountTransfer_ZeroAmount) {
    EXPECT_THROW(account->transfer(0.0, *target_account), std::invalid_argument);
}

// Invalid Inputs
TEST_F(BankAccountTest, UT_BankAccountDeposit_NegativeAmount) {
    EXPECT_THROW(account->deposit(-500.0), std::invalid_argument);
}

TEST_F(BankAccountTest, UT_BankAccountWithdraw_NegativeAmount) {
    EXPECT_THROW(account->withdraw(-300.0), std::invalid_argument);
}

TEST_F(BankAccountTest, UT_BankAccountTransfer_NegativeAmount) {
    EXPECT_THROW(account->transfer(-200.0, *target_account), std::invalid_argument);
}

TEST_F(BankAccountTest, UT_BankAccountWithdraw_ExceedBalance) {
    EXPECT_THROW(account->withdraw(1200.0), std::runtime_error);
}

TEST_F(BankAccountTest, UT_BankAccountTransfer_ExceedBalance) {
    EXPECT_THROW(account->transfer(1200.0, *target_account), std::runtime_error);
}

TEST_F(BankAccountTest, UT_BankAccountTransfer_ToSameAccount) {
    EXPECT_THROW(account->transfer(200.0, *account), std::invalid_argument);
}

// Dependency-Related Scenarios
/*TEST_F(BankAccountTest, UT_BankAccountCreate_InvalidInitialBalance) {
    EXPECT_THROW(BankAccount invalid_account("Alice", -500.0), std::invalid_argument);
}*/

/*TEST_F(BankAccountTest, UT_BankAccountTransfer_UninitializedAccount) {
    BankAccount* uninitialized_account = nullptr;
    EXPECT_THROW(account->transfer(200.0, *uninitialized_account), std::runtime_error);
}*/

// Additional Edge Cases
TEST_F(BankAccountTest, UT_BankAccountThreadSafety_SimultaneousOperations) {
    // TODO: Implement multithreading and synchronization tests.
}

TEST_F(BankAccountTest, UT_BankAccountRapidTransactions_Accuracy) {
    // TODO: Implement rapid consecutive transactions test.
}

TEST_F(BankAccountTest, UT_BankAccountFloatingPoint_Precision) {
    // TODO: Implement floating-point precision test.
}

TEST_F(BankAccountTest, UT_BankAccountDeposit_ExtremeValues) {
    EXPECT_NO_THROW(account->deposit(1e9));
    EXPECT_EQ(account->get_balance(), 1001000000.0);
}

TEST_F(BankAccountTest, UT_BankAccountWithdraw_ExtremeValues) {
    account->deposit(1e9);
    EXPECT_NO_THROW(account->withdraw(1e9));
    EXPECT_EQ(account->get_balance(), 1000.0);
}

TEST_F(BankAccountTest, UT_BankAccountTransfer_ExtremeValues) {
    account->deposit(1e9);
    EXPECT_NO_THROW(account->transfer(1e9, *target_account));
    EXPECT_EQ(account->get_balance(), 1000.0);
    EXPECT_EQ(target_account->get_balance(), 1e9 + 500.0);
}

TEST_F(BankAccountTest, UT_BankAccountNullPointerOperations) {
    BankAccount* null_account = nullptr;
    EXPECT_THROW(null_account->deposit(100.0), std::runtime_error);
    EXPECT_THROW(null_account->withdraw(100.0), std::runtime_error);
    EXPECT_THROW(null_account->transfer(100.0, *target_account), std::runtime_error);
}
