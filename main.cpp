#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <map>

using namespace std;

// Class 1: Represents a single Expense
class Expense {
private:
    string date;
    string category;
    double amount;
    string note;

public:
    Expense(string d, string c, double a, string n) : date(d), category(c), amount(a), note(n) {}

    string getDate() const { return date; }
    string getCategory() const { return category; }
    double getAmount() const { return amount; }
    string getNote() const { return note; }

    void display() const {
        cout << left << setw(10) << date << setw(15) << category 
             << setw(10) << amount << note << endl;
    }
};

// Class 2: Manages the collection and file I/O
class BudgetManager {
private:
    vector<Expense> expenses; 
    double monthlyBudget;
    const string filename = "expenses.txt";

public:
    BudgetManager() : monthlyBudget(5000.0) { loadFromFile(); }

    void addExpense() {
        string d, c, n;
        double a;
        
        cout << "\nEnter Date (DD-MM):" << endl; cin >> d;
        
        // VALIDATION: Category must be from a fixed list
        while (true) {
            cout << "Enter Category (Food/Transport/Bills/Entertainment/Other):" << endl; 
            cin >> c;
            if (c == "Food" || c == "Transport" || c == "Bills" || c == "Entertainment" || c == "Other") {
                break;
            }
            cout << "Invalid category! Please use one of the allowed words.\n" << endl;
        }

        // VALIDATION: Amount must be positive
        while (true) {
            cout << "Enter Amount:" << endl; cin >> a;
            if (a > 0) break;
            cout << "Error: Amount must be positive!\n" << endl;
        }
        
        cin.ignore();
        cout << "Enter Note:" << endl; getline(cin, n);

        expenses.emplace_back(d, c, a, n);
        saveToFile();
        checkBudget();
        cout << "\n>>> Expense added successfully! <<<\n" << endl;
    }

    void viewAll() const {
        cout << "\n--- All Expenses ---\n";
        cout << left << setw(10) << "Date" << setw(15) << "Category" 
             << setw(10) << "Amount" << "Note" << endl;
        for (const auto& e : expenses) e.display();
        cout << "\n" << endl;
    }

    // NEW FUNCTION: Search by Category or Date
    void searchExpense() const {
        int choice;
        string query;
        cout << "\nSearch by: 1. Date  2. Category\nChoice:" << endl;
        cin >> choice;
        cout << "Enter search term:" << endl;
        cin >> query;

        cout << "\n--- Search Results ---\n";
        cout << left << setw(10) << "Date" << setw(15) << "Category" << setw(10) << "Amount" << "Note" << endl;
        bool found = false;
        for (const auto& e : expenses) {
            if ((choice == 1 && e.getDate() == query) || (choice == 2 && e.getCategory() == query)) {
                e.display();
                found = true;
            }
        }
        if (!found) cout << "No expenses found matching '" << query << "'.\n";
        cout << "\n" << endl;
    }

    // NEW FUNCTION: Reports (Monthly Total & Category-wise)
    void viewReports() const {
        double total = 0;
        map<string, double> catTotals; // Map to store category-wise totals
        
        for (const auto& e : expenses) {
            total += e.getAmount();
            catTotals[e.getCategory()] += e.getAmount();
        }

        cout << "\n======= FINANCIAL REPORT =======\n";
        cout << "Total Monthly Expenses: Rs." << total << "\n";
        cout << "Monthly Budget Limit:   Rs." << monthlyBudget << "\n";
        
        if (total > monthlyBudget) {
            cout << "!!! WARNING: You have exceeded your monthly budget !!!\n";
        }

        cout << "\n--- Category-wise Breakdown ---\n";
        for (const auto& pair : catTotals) {
            cout << "- " << left << setw(15) << pair.first << ": Rs." << pair.second << "\n";
        }
        cout << "================================\n\n" << endl;
    }

    void checkBudget() {
        double total = 0;
        for (const auto& e : expenses) total += e.getAmount();
        if (total > monthlyBudget) {
            cout << "\n!!! WARNING: Monthly budget exceeded! Total: " << total << " !!!\n" << endl;
        }
    }

    void saveToFile() { 
        ofstream outFile(filename);
        for (const auto& e : expenses) {
            outFile << e.getDate() << "," << e.getCategory() << "," 
                    << e.getAmount() << "," << e.getNote() << endl;
        }
        outFile.close();
    }

    void loadFromFile() {
        ifstream inFile(filename);
        if (!inFile) return;
        string d, c, n;
        double a;
        while (getline(inFile, d, ',')) {
            getline(inFile, c, ',');
            inFile >> a; inFile.ignore();
            getline(inFile, n);
            expenses.emplace_back(d, c, a, n);
        }
        inFile.close();
    }
};

int main() {
    BudgetManager manager;
    int choice;

    do {
        cout << "===============================\n";
        cout << "      MONTHLY MONEY COACH      \n";
        cout << "===============================\n";
        cout << "1. Add a New Expense\n";
        cout << "2. View All Expenses\n";
        cout << "3. Search Expenses\n";
        cout << "4. View Reports (Totals)\n";
        cout << "5. Exit Program\n";
        cout << "-------------------------------\n";
        cout << "Type your choice in the pop-up box:" << endl; 
        
        cin >> choice;

        switch (choice) {
            case 1: manager.addExpense(); break;
            case 2: manager.viewAll(); break;
            case 3: manager.searchExpense(); break;
            case 4: manager.viewReports(); break;
            case 5: cout << "Exiting... Refresh the webpage to start over.\n" << endl; break;
            default: cout << "Invalid choice! Please type a number 1-5.\n" << endl;
        }
    } while (choice != 5);

    return 0;
}