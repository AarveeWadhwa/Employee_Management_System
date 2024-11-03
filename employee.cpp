#include <iostream>
#include <vector>
#include <iomanip> // For setw() to format output
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// User Authentication class
class User {
public:
    string username;
    string password;
    string role; // "Admin" or "Employee"
    int id;

    User(string uname, string pwd, string r) : username(uname), password(pwd), role(r), id(-1) {}

    // Constructor for Employee
    User(string uname, string pwd, string r, int empId) : username(uname), password(pwd), role(r), id(empId) {}
};

User * currentUser  = nullptr;
vector<User> users;

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

    string toString() const {
        return "Rating: " + to_string(rating) + " | Feedback: " + feedback;
    }
};

// Base class Employee
class Employee {
protected:
    string eName;
    int eID;
    string eDesignation;
    double finalSalary; // Keep this as protected
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

    virtual string toFileString() const {
        return "Name: " + eName + "\nID: " + to_string(eID) + "\nDesignation: " + eDesignation + 
               "\nType: " + eType + "\nSalary: " + to_string(finalSalary) + 
               "\nStatus: " + status + "\nPerformance Review: " + review.toString() + "\n";
    }

    int getID() {
        return eID;
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

    // Setter for finalSalary
    void setFinalSalary(double salary) {
        finalSalary = salary;
    }

    // Getter for finalSalary
    double getFinalSalary() const {
        return finalSalary;
    }

    void addPerformanceReview() {
        review.inputReview();
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
            increment = 0 ;       }

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
   void loadEmployeesFromFile() {
    ifstream inFile("employee_details.txt");
    string line;
    bool headerPrinted = false;

    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            // If a new record starts, print the header (only once)
            if (!headerPrinted) {
                cout << left << setw(20) << "Name" 
                     << setw(10) << "ID"
                     << setw(20) << "Designation" 
                     << setw(15) << "Salary"
                     << setw(15) << "Increment" 
                     << setw(30) << "Status" 
                     << setw(15) << "Type" 
                     << setw(30) << "Performance Review" << endl;

                // Print a longer separator line
                cout << string(20, '-') << " "
                     << string(10, '-') << " "
                     << string(20, '-') << " "
                     << string(15, '-') << " "
                     << string(15, '-') << " "
                     << string(30, '-') << " "
                     << string(15, '-') << " "
                     << string(30, '-') << endl;

                headerPrinted = true;
            }
            
            // Extract and format employee details from the file
            if (line.find("Name:") != string::npos) {
                string name = line.substr(6);
                getline(inFile, line); // Read next line (ID)
                int id = stoi(line.substr(4));
                getline(inFile, line);  // Read next line (Designation)
                string designation = line.substr(13);
                getline(inFile, line);  // Read next line (Type)
                string type = line.substr(6);
                getline(inFile, line);  // Read next line (Salary)
                double salary = stod(line.substr(8));
                getline(inFile, line);  // Read next line (Status)
                string status = line.substr(8); // Assuming status starts at index 8
                getline(inFile, line);  // Read next line (Performance Review)
                string review = line.substr(19); // Assuming review starts at index 19

                // Print the formatted employee record without vertical lines
                cout << left << setw(20) << name 
                     << setw(10) << id
                     << setw(20) << designation 
                     << setw(15) << salary
                     << setw(15) << "N/A" // Increment is not stored in the file, so we can put "N/A"
                     << setw(30) << status 
                     << setw(15) << type 
                     << setw(30) << review << endl;

                // Print a separator line between records
                cout << string(20, '-') << " "
                     << string(10, '-') << " "
                     << string(20, '-') << " "
                     << string(15, '-') << " "
                     << string(15, '-') << " "
                     << string(30, '-') << " "
                     << string(15, '-') << " "
                     << string(30, '-') << endl;
            }
        }
        inFile.close();
    } else {
        cout << "Error: Unable to open employee_details.txt!" << endl;
    }
}
    void addEmployee() {
        string name, designation, type;
        int id;

        cout << "Enter employee name: ";
        cin >> name;
        cout << "Enter employee ID: ";
        cin >> id;

        // Check for duplicate ID
        for (const auto& employee : employees) {
            if (employee->getID() == id) {
                cout << "Error: Duplicate ID! An employee with ID " << id << " already exists." << endl;
                return; // Exit the function if a duplicate ID is found
            }
        }

        cout << "Enter employee designation (CEO/Manager/Labour): ";
        cin >> designation;
        cout << "Enter employee type (Permanent/Visiting): ";
        cin >> type;

        Employee* newEmployee;
        if (type == "Permanent") {
            newEmployee = new PermanentEmp(name, id, designation);
        } else if (type == "Visiting") {
            newEmployee = new VisitingEmp(name, id, designation);
        } else {
            cout << "Invalid employee type!" << endl;
            return; // Exit if the type is invalid
        }

        newEmployee->calculateSalary();
        newEmployee->addPerformanceReview(); // Adding performance review input after salary calculation

        employees.push_back(newEmployee);

        // Write the new employee to the file
        ofstream outFile("employee_details.txt", ios::app);
        if (outFile.is_open()) {
            outFile << newEmployee->toFileString() << endl; // Assuming toFileString() formats the employee data correctly
            outFile.close();
        } else {
            cout << "Error: Unable to open employee_details.txt for writing!" << endl;
        }
    }

    void deleteEmployee(int id) {
        for (size_t i = 0; i < employees.size(); i++) {
            if (employees[i]->getID() == id) {
                cout << "Employee with ID " << id << " deleted!" << endl;
                delete employees[i];
                employees.erase(employees.begin() + i);

                // Rewrite the file without the deleted employee
                ofstream outFile("employee_details.txt");
                if (outFile.is_open()) {
                    for (const auto& employee : employees) {
                        outFile << employee->toFileString() << endl;
                    }
                    outFile.close();
                } else {
                    cout << "Error: Unable to open employee_details.txt for writing!" << endl;
                }

                return;
            }
        }
        cout << "Employee with ID " << id << " not found!" << endl;
    }

    void searchEmployee(int id) {
        ifstream file("employee_details.txt");
        if (!file) {
            cerr << "Error opening employee details file!" << endl;
            return;
        }

        string line;
        bool found = false;

        // Loop through each line in the file
        while (getline(file, line)) {
            if (line.find("Name:") != string::npos) {
                string name = line.substr(6);
                getline(file, line); // Read ID
                int empID = stoi(line.substr(4));
                getline(file, line); // Read Designation
                string designation = line.substr(13);
                getline(file, line); // Read Type
                string type = line.substr(6);
                getline(file, line); // Read Salary
                double salary = stod(line.substr(8));
                getline(file, line); // Read Status
                string status = line.substr(8);
                getline(file, line); // Read Performance Review
                string review = line.substr(19);

                if (empID == id) {
                    if (currentUser ->role == "Admin" || (currentUser ->role == "Employee" && empID == currentUser ->id)) {
                        cout << "Employee found:\n";
                        cout << "Name: " << name << "\nID: " << empID
                             << "\nDesignation: " << designation << "\nType: " << type
                             << "\nSalary: " << salary << "\nStatus: " << status
                             << "\nReview: " << review << endl;
                        found = true;
                    } else {
                        cout << "You are not authorized to view this employee's information." << endl;
                        found = true;
                    }
                    break; // Exit the loop once the employee is found
                }
            }
        }

        if (!found) {
            cout << "Employee with ID " << id << " not found!" << endl;
        }

        file.close();
    }

    void editEmployee(int id) {
        for (size_t i = 0; i < employees.size(); i++) {
            if (employees[i]->getID() == id) {
                string newName, newDesignation, newType;
                double adjustmentAmount;
                char adjustmentType;

                cout << "Enter new name: ";
                cin >> newName;
                cout << "Enter new designation (CEO/Manager/Labour): ";
                cin >> newDesignation;
                cout << "Enter new type (Permanent/Visiting): ";
                cin >> newType;

                // Update employee details
                employees[i]->setName(newName);
                employees[i]->setDesignation(newDesignation);
                employees[i]->setType(newType);

                // Prompt for salary adjustment
                cout << "Do you want to (A)dd bonus or (D)educt salary? (A/D): ";
                cin >> adjustmentType;

                if (adjustmentType == 'A' || adjustmentType == 'a') {
                    cout << "Enter bonus amount: ";
                    cin >> adjustmentAmount;
                    employees[i]->setFinalSalary(employees[i]->getFinalSalary() + adjustmentAmount); // Add bonus
                                } else if (adjustmentType == 'D' || adjustmentType == 'd') {
                    cout << "Enter deduction amount: ";
                    cin >> adjustmentAmount;
                    employees[i]->setFinalSalary(employees[i]->getFinalSalary() - adjustmentAmount); // Deduct salary
                } else {
                    cout << "Invalid option! No changes made to salary." << endl;
                }

                // Recalculate salary if necessary
                if (newType == "Permanent") {
                    employees[i]->calculateSalary(); // This will calculate based on designation
                }

                employees[i]->addPerformanceReview(); // Prompt for performance review after editing

                // Rewrite the file with the updated employee
                ofstream outFile("employee_details.txt");
                if (outFile.is_open()) {
                    for (const auto& employee : employees) {
                        outFile << employee->toFileString() << endl;
                    }
                    outFile.close();
                    cout << "Employee record updated successfully!" << endl;
                } else {
                    cout << "Error: Unable to open employee_details.txt for writing!" << endl;
                }

                return; // Exit the function after editing
            }
        }
        cout << "Employee with ID " << id << " not found!" << endl;
    }

    void displayAllEmployees() {
        if (currentUser ->role == "Admin") {
            // Display all employees as before
            loadEmployeesFromFile();
            cout << "\n--- In-Memory Employees ---" << endl;
            for (size_t i = 0; i < employees.size(); i++) {
                employees[i]->displayInfo();
            }
        } else {
            // Display only the logged-in employee's information
            for (size_t i = 0; i < employees.size(); i++) {
                if (employees[i]->getID() == currentUser ->id) {
                    employees[i]->displayInfo();
                    break;
                }
            }
        }
    }
};

// Function to authenticate users
User * authenticate() {
    if(users.empty()){
        users.push_back(User("admin", "admin123", "Admin"));
        users.push_back(User("emp1", "emp123", "Employee", 45));
        users.push_back(User("emp2", "emp234", "Employee", 102));
    }
    string uname, pwd;
    cout << "Enter username: ";
    cin >> uname;
    cout << "Enter password: ";
    cin >> pwd;

    for (User & user : users) {
        if (user.username == uname && user.password == pwd) {
            currentUser  = &user;
            cout << "Login successful! Logged in as " << user.role << endl;
            return currentUser ;
        }
    }
    cout << "Invalid username or password." << endl;
    return nullptr;
}

int main() {
    EmployeeManager manager;

    // Authenticate the user
    if (authenticate()) {
        // Check the role of the current user
        if (currentUser ->role == "Admin") {
            int choice;

            // Admin menu loop
            do {
                cout << "\n--- Admin Menu ---" << endl;
                cout << "1. Add Employee" << endl;
                cout << "2. Delete Employee" << endl;
                cout << "3. Search Employee" << endl;
                cout << "4. Display All Employees" << endl;
                cout << "5. Edit Employee" << endl; // Added option to edit employee
                cout << "0. Exit" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                int id;
                switch (choice) {
                    case 1:
                        manager.addEmployee();
                        break;
                    case 2:
                        cout << "Enter Employee ID to delete: ";
                        cin >> id;
                        manager.deleteEmployee(id);
                        break;
                    case 3:
                        cout << "Enter Employee ID to search: ";
                        cin >> id;
                        manager.searchEmployee(id);
                        break;
                    case 4:
                        manager.displayAllEmployees();
                        break;
                    case 5:
                        cout << "Enter Employee ID to edit: ";
                        cin >> id;
                        manager.editEmployee(id); // Call the edit function
                        break;
                    case 0:
                        cout << "Exiting program." << endl;
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                }
            } while (choice != 0);

        } else if (currentUser ->role == "Employee") {
            int id;

            // Prompt employee to enter their ID to view their own record
            cout << "Enter your Employee ID to view your record: ";
            cin >> id;

            // Search for the employee's record using their ID
            manager.searchEmployee(id);
               }
    }

    return 0; // Exit the program
}
