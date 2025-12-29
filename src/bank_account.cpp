#include "bank_account.h"
BankAccount::BankAccount(const std::string& owner, double balance)
     : owner(owner), balance(balance) {}

void BankAccount::deposit(double amount) {
     if (amount <= 0) {
         throw std::invalid_argument("Deposit amount must be positive.");
     }
     balance += amount;
}

void BankAccount::withdraw(double amount) {
     if (amount <= 0) {
         throw std::invalid_argument("Withdrawal amount must be positive.");
     }
     if (amount > balance) {
         throw std::runtime_error("Insufficient funds.");
     }
     balance -= amount;
}

double BankAccount::get_balance() const {
     return balance; 
}

void BankAccount::transfer(double amount, BankAccount& target_account) {
     if (this == &target_account) {
         throw std::invalid_argument("Cannot transfer to the same account.");
     }
     withdraw(amount);
     target_account.deposit(amount);
}
