#include <iostream>
#include <iomanip>
#include <string>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;

public:
    double balance;
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual void displayDetails() const {
        std::cout << "Account Details for Account (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    }

    virtual void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            std::cout << "Withdrawal successful." << std::endl;
        } else {
            std::cout << "Insufficient funds for withdrawal." << std::endl;
        }
    }

    virtual ~Account() = default;
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    int amt = 300;
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%" << std::endl;
    }

    void withdraw(double amount) override {
        const double minBalance = 100.0; // Minimum balance requirement for savings account
        if (balance - amount >= minBalance) {
            balance -= amount;
            std::cout << "Withdrawal successful." << std::endl;
        } else {
            std::cout << "Insufficient funds for withdrawal. Minimum balance requirement not met." << std::endl;
        }
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    int amt = 300;
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }

    void withdraw(double amount) override {
        if (balance - amount >= -overdraftLimit) {
            balance -= amount;
            std::cout << "Withdrawal successful." << std::endl;
        } else {
            std::cout << "Insufficient funds for withdrawal. Overdraft limit exceeded." << std::endl;
        }
    }
};

// Overload + operator for transferring money between accounts
CurrentAccount operator+(const CurrentAccount& lhs, const SavingsAccount& rhs) {
    CurrentAccount result = lhs;
    result.deposit(rhs.amt);
    const_cast<SavingsAccount&>(rhs).balance -= rhs.amt; // Reset the balance of the savings account after transfer
    return result;
}

// Overload << operator to display account details
std::ostream& operator<<(std::ostream& os, const Account& account) {
    account.displayDetails();
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings;
    std::cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << "Account Details after deposit and withdrawal:" << std::endl;
    std::cout << savings;
    std::cout << current;

    // Transfer 300 from savings to current
    current = current + savings;

    std::cout << "Account Details after transfer:" << std::endl;
    std::cout << savings;
    std::cout << current;

    return 0;
}

