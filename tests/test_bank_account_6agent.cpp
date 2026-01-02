#include "bank_account.h"
#include <gtest/gtest.h>
#include <iostream> 
#include <limits>  

// Test case to verify that depositing money increases the account balance correctly.
TEST(BankAccountTest, Deposit) {
    std::cout << "Running Test: Deposit\n";
    BankAccount account("John", 100.0);
    account.deposit(50.0);
    EXPECT_EQ(account.get_balance(), 150.0);
    std::cout << "Deposit Test Passed: Balance is " << account.get_balance() << "\n";
}

// Test case to verify boundary values for deposit functionality.
TEST(BankAccountTest, DepositBoundaryValues) {
    std::cout << "Running Test: DepositBoundaryValues\n";
    BankAccount account("John", 100.0);

    // Smallest positive deposit
    account.deposit(0.01);
    EXPECT_EQ(account.get_balance(), 100.01);

    // Maximum deposit
    EXPECT_NO_THROW(account.deposit(DBL_MAX - 100.01));

    // Deposit zero (invalid)
    EXPECT_THROW(account.deposit(0), std::invalid_argument);

    // Deposit negative amount (invalid)
    EXPECT_THROW(account.deposit(-50.0), std::invalid_argument);

    std::cout << "DepositBoundaryValues Test Passed\n";
}

// Test case to verify that withdrawing money decreases the account balance correctly.
TEST(BankAccountTest, Withdraw) {
    std::cout << "Running Test: Withdraw\n";
    BankAccount account("John", 100.0);
    account.withdraw(50.0);
    EXPECT_EQ(account.get_balance(), 50.0);
    std::cout << "Withdraw Test Passed: Balance is " << account.get_balance() << "\n";
}

// Test case to verify boundary values for withdraw functionality.
TEST(BankAccountTest, WithdrawBoundaryValues) {
    std::cout << "Running Test: WithdrawBoundaryValues\n";
    BankAccount account("John", 100.0);

    // Withdraw exact balance
    EXPECT_NO_THROW(account.withdraw(100.0));
    EXPECT_EQ(account.get_balance(), 0.0);

    // Withdraw more than balance
    EXPECT_THROW(account.withdraw(0.01), std::runtime_error);

    // Withdraw zero (invalid)
    EXPECT_THROW(account.withdraw(0), std::invalid_argument);

    // Withdraw negative amount (invalid)
    EXPECT_THROW(account.withdraw(-50.0), std::invalid_argument);

    std::cout << "WithdrawBoundaryValues Test Passed\n";
}

// Test case to verify that withdrawing more money than the account balance throws an exception.
TEST(BankAccountTest, InsufficientFunds) {
    std::cout << "Running Test: InsufficientFunds\n";
    BankAccount account("John", 100.0);
    EXPECT_THROW(account.withdraw(150.0), std::runtime_error);
    std::cout << "InsufficientFunds Test Passed: Exception thrown as expected\n";
}

// Test case to verify that transferring money between two accounts updates both balances correctly.
TEST(BankAccountTest, Transfer) {
    std::cout << "Running Test: Transfer\n";
    BankAccount account1("John", 100.0);
    BankAccount account2("Jane", 50.0);
    account1.transfer(50.0, account2);
    EXPECT_EQ(account1.get_balance(), 50.0);
    EXPECT_EQ(account2.get_balance(), 100.0);
    std::cout << "Transfer Test Passed: Account1 Balance = " << account1.get_balance()
              << ", Account2 Balance = " << account2.get_balance() << "\n";
}

// Test case to verify boundary values for transfer functionality.
TEST(BankAccountTest, TransferBoundaryValues) {
    std::cout << "Running Test: TransferBoundaryValues\n";
    BankAccount account1("John", 100.0);
    BankAccount account2("Jane", 50.0);

    // Transfer exact balance
    EXPECT_NO_THROW(account1.transfer(100.0, account2));
    EXPECT_EQ(account1.get_balance(), 0.0);
    EXPECT_EQ(account2.get_balance(), 150.0);

    // Transfer zero (invalid)
    EXPECT_THROW(account1.transfer(0, account2), std::invalid_argument);

    // Transfer negative amount (invalid)
    EXPECT_THROW(account1.transfer(-50.0, account2), std::invalid_argument);

    // Transfer to the same account (invalid)
    EXPECT_THROW(account1.transfer(50.0, account1), std::invalid_argument);

    std::cout << "TransferBoundaryValues Test Passed\n";
}

// Test case to verify boundary values for get_balance functionality.
TEST(BankAccountTest, GetBalanceBoundaryValues) {
    std::cout << "Running Test: GetBalanceBoundaryValues\n";
    BankAccount account("John", 0.0);

    // Check initial balance
    EXPECT_EQ(account.get_balance(), 0.0);

    // Add a large amount
    account.deposit(DBL_MAX);
    EXPECT_EQ(account.get_balance(), DBL_MAX);

    // Withdraw a large amount
    account.withdraw(DBL_MAX);
    EXPECT_EQ(account.get_balance(), 0.0);

    std::cout << "GetBalanceBoundaryValues Test Passed\n";
}
