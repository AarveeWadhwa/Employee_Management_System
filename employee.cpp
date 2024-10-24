#include <iostream>
#include <vector>
#include <iomanip> // For setw() to format output
#include <string>
#include <fstream> // For file handling
using namespace std;

// User Authentication class
class User {
public:
    string username;
    string password;
    string role; // "Admin" or "Employee"

    User(string uname, string pwd, string r) : username(uname), password(pwd), role(r) {}
};

// Performance Review class
class PerformanceReview {
private:
    int rating;  // Rating out of 5
    string feedback;

public:
    void inputReview() {
        cout << "Enter performance rating (1-5): ";
        cin >> rating;
        cout << "Enter feedback: ";
        cin.ignore();  // To clear the newline character from the input buffer
        getline(cin, feedback);
    }

    void displayReview() const {
        cout << "Rating: " << rating << " | Feedback: " << feedback << endl;
    }

    string getReview() const {
        return "Rating: " + to_string(rating) + " | Feedback: " + feedback;
    }
};

// Base class Employee
class Employee {
protected:
    string eName;
    int eID;
    string eDesignation;
    double finalSalary;
    string eType;  // Permanent or Visiting
    double increment;
    string status;
    PerformanceReview review; // Adding performance review to Employee

public:
    Employee(string name, int id, string designation, string type) 
        : eName(name), eID(id), eDesignation(designation), eType(type), finalSalary(0.0), increment(0.0), status("Discontinued") {}

    virtual void calculateSalary() = 0; // Abstract function

    virtual void displayInfo() {
        cout << left << setw(20) << eName << setw(10) << eID 
             << setw(20) << eDesignation << setw(15) << finalSalary 
             << setw(15) << increment << setw(30) << status << setw(15) << eType << endl;
        review.displayReview(); // Display performance review
    }

    int getID() {
        return eID;
    }

    double getSalary() {
        return finalSalary;
    }

    string getPerformanceReview() {
        return review.getReview();
    }

    void setName(string name) {
        eName = name;
    }

    void setDesignation(string designation) {
        eDesignation = designation;
    }

    void setType(string type) {
        eType = type;
    }

    void addPerformanceReview() {
        review.inputReview();
    }

    // New function to edit salary with bonus or deduction
    void editSalary() {
        double bonusOrDeduction;
        cout << "Enter bonus or deduction amount (positive for bonus, negative for deduction): ";
        cin >> bonusOrDeduction;
        finalSalary += bonusOrDeduction;

        if (bonusOrDeduction > 0) {
            cout << "Bonus of " << bonusOrDeduction << " added to salary. New salary: " << finalSalary << endl;
        } else {
            cout << "Deduction of " << abs(bonusOrDeduction) << " applied. New salary: " << finalSalary << endl;
        }
    }
};

// Derived class for Permanent Employee
class PermanentEmp : public Employee {
public:
    PermanentEmp(string name, int id, string designation) 
        : Employee(name, id, designation, "Permanent") {}

    void calculateSalary() override {
        double basicSalary;

        cout << "Enter employee's current salary: ";
        cin >> finalSalary;

        if (eDesignation == "CEO") {
            basicSalary = 50000;
        } else if (eDesignation == "Manager") {
            basicSalary = 30000;
        } else if (eDesignation == "Labour") {
            basicSalary = 15000;
        } else {
            cout << "Invalid Designation!" << endl;
            return;
        }

        int yearsWorked;
        cout << "How many years has the employee worked? ";
        cin >> yearsWorked;

        if (yearsWorked >= 6) {
            increment = 10;
        } else if (yearsWorked >= 3) {
            increment = 7;
        } else if (yearsWorked >= 2) {
            increment = 5;
        } else {
            increment = 0;
        }

        double newSalary = basicSalary + (basicSalary * increment / 100);
        cout << "Calculated Salary after " << increment << "% increment: " << newSalary << endl;

        // Set status and next increment details in status column
        double nextIncrement = increment + (yearsWorked < 6 ? 3 : 0);  // Next increment percentage
        status = "Continued (Next Increment: " + to_string(nextIncrement).substr(0, 4) + "%)";
    }

    void displayInfo() override {
        Employee::displayInfo();
    }
};

// Derived class for Visiting Employee
class VisitingEmp : public Employee {
public:
    VisitingEmp(string name, int id, string designation) 
        : Employee(name, id, designation, "Visiting") {}

    void calculateSalary() override {
        int dailyWage;
        cout << "Enter Daily Wage for " << eDesignation << ": ";
        cin >> dailyWage;
        int workDays;
        cout << "Enter Number of Work Days: ";
        cin >> workDays;
        finalSalary = dailyWage * workDays;
        status = "Continued (No Increment)";
    }

    void displayInfo() override {
        Employee::displayInfo();
    }
};

// Class to manage employees
class EmployeeManager {
private:
    vector<Employee*> employees;

public:
    void addEmployee() {
    string name, designation, type;
    int id, yearsWorked;
    double currentSalary;

    cout << "Enter employee name: ";
    cin >> name;
    cout << "Enter employee ID: ";
    cin >> id;

    // Check for duplicate ID
    for (size_t i = 0; i < employees.size(); i++) {
        if (employees[i]->getID() == id) {
            cout << "Error: Duplicate ID!" << endl;
            return;
        }
    }

    cout << "Enter employee designation (CEO/Manager/Labour): ";
    cin >> designation;
    cout << "Enter employee type (Permanent/Visiting): ";
    cin >> type;

    if (type == "Permanent") {
        cout << "Enter years worked: ";
        cin >> yearsWorked;
        cout << "Enter current salary: ";
        cin >> currentSalary;

        // Create Permanent employee
        PermanentEmp* newEmp = new PermanentEmp(name, id, designation);
        employees.push_back(newEmp);

        // Calculate new salary and determine status
        newEmp->calculateSalary(yearsWorked); // Assuming you have modified calculateSalary to accept yearsWorked
        double newSalary = newEmp->getSalary();

        // Determine status
        string status = (currentSalary < newSalary) ? "Discontinued" : "Continued";

        // Store employee details in the file
        ofstream outFile("employee_details.txt", ios::app); // Append mode to keep previous records
        if (outFile.is_open()) {
            outFile << "Name: " << name << "\n";
            outFile << "ID: " << id << "\n";
            outFile << "Designation: " << designation << "\n";
            outFile << "Type: " << type << "\n";
            outFile << "Salary: " << newSalary << "\n";
            outFile << "Status: " << status << "\n"; // Store the status
            outFile << "Performance Review: " << newEmp->getPerformanceReview() << "\n";
            outFile << "-------------------------------\n";
            outFile.close();
            cout << "Employee record added and saved successfully!" << endl;
        } else {
            cout << "Error: Unable to open file!" << endl;
        }
    } else if (type == "Visiting") {
        employees.push_back(new VisitingEmp(name, id, designation));

        // Calculate salary and add performance review
        employees.back()->calculateSalary();
        employees.back()->addPerformanceReview();

        // Store employee details in the file
        ofstream outFile("employee_details.txt", ios::app); // Append mode to keep previous records
        if (outFile.is_open()) {
            outFile << "Name: " << name << "\n";
            outFile << "ID: " << id << "\n";
            outFile << "Designation: " << designation << "\n";
            outFile << "Type: " << type << "\n";
            outFile << "Salary: " << employees.back()->getSalary() << "\n";
            outFile << "Status: N/A\n"; // No status for visiting employees
            outFile << "Performance Review: " << employees.back()->getPerformanceReview() << "\n";
            outFile << "-------------------------------\n";
            outFile.close();
            cout << "Employee record added and saved successfully!" << endl;
        } else {
            cout << "Error: Unable to open file!" << endl;
        }
    } else {
        cout << "Invalid employee type!" << endl;
    }
}
    void loadEmployeesFromFile() {
    ifstream inFile("employee_details.txt");
    string line;
    bool headerPrinted = false;

    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            // If a new record starts, print the header (only once)
            if (!headerPrinted) {
                cout << left << setw(20) << "Name" << setw(10) << "ID"
                     << setw(20) << "Designation" << setw(15) << "Salary"
                     << setw(10) << "Increment" << setw(40) << "Status" << setw(10) << "Type" << endl;
                cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
                headerPrinted = true;
            }

            // Extract and format employee details from the file
            if (line.find("Name:") != string::npos) {
                string name = line.substr(6);
                getline(inFile, line);  // Read next line (ID)
                string id = line.substr(4);
                getline(inFile, line);  // Read next line (Designation)
                string designation = line.substr(13);
                getline(inFile, line);  // Read next line (Type)
                string type = line.substr(6);
                getline(inFile, line);  // Read next line (Salary)
                string salary = line.substr(8);
                getline(inFile, line);  // Read next line (Increment)
                string increment = line.substr(10);
                getline(inFile, line);  // Read next line (Performance Review)
                string review = line.substr(19);  // Assuming review is also needed but not printed
                
                // Read the next line for Status
                getline(inFile, line);  // Read Status
                string status = line.substr(8); // Adjusting index to match your status format
                
                // Print the formatted employee record
                cout << left << setw(20) << name << setw(10) << id
                     << setw(20) << designation << setw(15) << salary
                     << setw(10) << increment << setw(40) << status << setw(10) << type << endl;

                getline(inFile, line); // Skip the separator line
            }
        }
        inFile.close();
    } else {
        cout << "Error: Unable to open employee_details.txt!" << endl;
    }
}
    
    void deleteEmployee(int id) {
        ifstream inFile("employee_details.txt");
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;

        if (inFile.is_open() && tempFile.is_open()) {
            while (getline(inFile, line)) {
                if (line.find("ID: " + to_string(id)) != string::npos) {
                    found = true; // Found employee to delete
                    // Skip the next lines to remove the employee
                    for (int i = 0; i < 5; ++i) {
                        getline(inFile, line); // Skip the next lines of this employee
                    }
                } else {
                    // Copy unchanged lines to the temp file
                    tempFile << line << endl;
                }
            }
            inFile.close();
            tempFile.close();

            // Replace old file with updated file
            remove("employee_details.txt");
            rename("temp.txt", "employee_details.txt");

            if (found) {
                cout << "Employee record deleted successfully!" << endl;
            } else {
                cout << "Error: Employee ID not found!" << endl;
            }
        } else {
            cout << "Error: Unable to open file!" << endl;
        }
    }

    void searchEmployee(int id) {ifstream inFile("employee_details.txt"); // Open the file for reading
    if (!inFile) {
        cout << "Error: Unable to open file!" << endl;
        return;
    }

    string line;
    bool found = false;

    // Read the file line by line
    while (getline(inFile, line) && !found) {
        // Check if the line contains the ID
        if (line.find("ID: " + to_string(id)) != string::npos) {
            found = true; // Employee ID found
            // Display the employee information
            do {
                cout << line << endl;
            } while (getline(inFile, line) && line != "-------------------------------"); // Continue displaying until separator
            cout << line << endl; // To display the separator
        }
    }

    if (!found) {
        cout << "Employee with ID " << id << " not found!" << endl;
    }

    inFile.close(); // Close the file
}

    void editEmployee(int id) {
        for (size_t i = 0; i < employees.size(); i++) {
            if (employees[i]->getID() == id) {
                string newName, newDesignation, newType;
                cout << "Editing Employee with ID: " << id << endl;

                cout << "Enter new name: ";
                cin >> newName;
                employees[i]->setName(newName);

                cout << "Enter new designation (CEO/Manager/Labour): ";
                cin >> newDesignation;
                employees[i]->setDesignation(newDesignation);

                cout << "Enter new type (Permanent/Visiting): ";
                cin >> newType;
                employees[i]->setType(newType);

                cout << "Employee details updated!" << endl;
                employees[i]->displayInfo();  // Display updated info
                return;
            }
        }
        cout << "Employee with ID " << id << " not found!" << endl;
    }

    void editEmployeeSalary(int id) {
        ifstream inFile("employee_details.txt");
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;

        if (inFile.is_open() && tempFile.is_open()) {
            while (getline(inFile, line)) {
                if (line.find("ID: " + to_string(id)) != string::npos) {
                    found = true; // Found employee to edit
                    // Skip the next lines to remove the employee record
                    cout << "Editing Employee ID: " << id << endl;

                    // Get new values from the user
                    string newName, newDesignation;
                    cout << "Enter new name: ";
                    cin >> newName;
                    cout << "Enter new designation: ";
                    cin >> newDesignation;

                    // Update details
                    line = "Name: " + newName;
                    tempFile << line << endl;
                    tempFile << "ID: " << id << endl;
                    tempFile << "Designation: " << newDesignation << endl;

                    // Copy the remaining employee info
                    for (int i = 0; i < 4; ++i) {
                        getline(inFile, line);
                        tempFile << line << endl; // Copy the remaining employee details
                    }

                    cout << "Employee record updated successfully!" << endl;
                } else {
                    // Copy unchanged lines to the temp file
                    tempFile << line << endl;
                }
            }
            inFile.close();
            tempFile.close();

            // Replace old file with updated file
            remove("employee_details.txt");
            rename("temp.txt", "employee_details.txt");

            if (!found) {
                cout << "Error: Employee ID not found!" << endl;
            }
        } else {
            cout << "Error: Unable to open file!" << endl;
        }
    }

    void displayEmployees(){
    loadEmployeesFromFile(); // Load and display employees from the file

    // If you want to also display in-memory employees, do that here
    cout << "\n--- In-Memory Employees ---" << endl;
    for (size_t i = 0; i < employees.size(); i++) {
        employees[i]->displayInfo();
    }
}
};

// Main function
int main() {
    EmployeeManager manager;


    // Sample Users
    User admin("admin", "admin123", "Admin");
    User employee("emp1", "pass123", "Employee");

    string inputUsername, inputPassword;
    cout << "Enter username: ";
    cin >> inputUsername;
    cout << "Enter password: ";
    cin >> inputPassword;

    // User authentication
    if (inputUsername == admin.username && inputPassword == admin.password) {
        cout << "Welcome Admin!" << endl;
        int choice;
        do {
            cout << "1. Add Employee\n2. Edit Employee\n3. Delete Employee\n4. Search Employee\n5. Edit Employee Salary\n6. Display All Employees\n7. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    manager.addEmployee();
                    break;
                case 2: {
                    int id;
                    cout << "Enter employee ID to edit: ";
                    cin >> id;
                    manager.editEmployee(id);
                    break;
                }
                case 3: {
                    int id;
                    cout << "Enter employee ID to delete: ";
                    cin >> id;
                    manager.deleteEmployee(id);
                    break;
                }
                case 4: {
                    int id;
                    cout << "Enter employee ID to search: ";
                    cin >> id;
                    manager.searchEmployee(id);
                    break;
                }
                case 5: {
                    int id;
                    cout << "Enter employee ID to edit salary: ";
                    cin >> id;
                    manager.editEmployeeSalary(id);
                    break;
                }
                case 6:
                    manager.displayEmployees();
                    break;
                case 7:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        } while (choice != 7);
    } else if (inputUsername == employee.username && inputPassword == employee.password) {
        cout << "Welcome Employee!" << endl;
        int choice;
        do {
            cout << "1. Search Employee\n2. Display All Employees\n3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    int id;
                    cout << "Enter employee ID to search: ";
                    cin >> id;
                    manager.searchEmployee(id);
                    break;
                }
                case 2:
                    manager.displayEmployees();
                    break;
                case 3:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        } while (choice != 3);
    } else {
        cout << "Invalid username or password!" << endl;
    }

    return 0;
}
