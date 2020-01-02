#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>

using namespace std;

const int EMP_ID_WIDTH = 8;
const int EMP_NAME_WIDTH = 20;
const int EMP_EMAIL_WIDTH = 25;
const int EMP_NUMERIC_WIDTH = 10;
const int TOTAL_WIDTH = 100;

typedef struct Employee{
    int empId;
    string name;
    double basicSalary;
    double pf;
    double healthInsAmt;
    string email;
} Employee;

void readFile(vector<Employee> &);
void printMenu();
void doTask(vector<Employee> &, int);
void addEmployee(vector<Employee> &);
int searchEmployee(vector<Employee> &, int);
void saveToFile(vector<Employee> v_emp);
void printEmpRecords(vector<Employee>);
void printEmployees(vector<Employee> v_emp);
void printEmployee(Employee e);
double getNetSalary(Employee e);
void deleteEmployee(vector<Employee> &v_emp, int);
void displayHeaders();


void readFile(vector<Employee> &v_emp){
    ifstream fin("emp.txt");
    if(!fin){
        cout << "Unable to open file..." << endl;
        exit(1);
    }
    string line;
    while(!fin.eof()){
        std::getline(fin, line);
        istringstream iss(line);

        string strEmpId, strName, strBasicSalary, strPf, strHealthInsAmt, strEmail;
        std::getline(iss, strEmpId, ',');
        std::getline(iss, strName, ',');
        std::getline(iss, strBasicSalary, ',');
        std::getline(iss, strPf, ',');
        std::getline(iss, strHealthInsAmt, ',');
        std::getline(iss, strEmail, ',');

        Employee temp;
        temp.empId = atoi(strEmpId.c_str());
        temp.name = strName;
        temp.basicSalary = atof(strBasicSalary.c_str());
        temp.pf = atof(strPf.c_str());
        temp.healthInsAmt = atof(strHealthInsAmt.c_str());
        temp.email = strEmail;

        v_emp.push_back(temp);
    }
    fin.close();
}

void printMenu(){
    cout << "1. Add Employee" << endl;
    cout << "2. Print Employee Report" << endl;
    cout << "3. Search Employee" << endl;
    cout << "4. Delete Employee" << endl;
    cout << "5. Save" << endl;
    cout << "6. Exit" << endl << endl;
}

void addEmployee(vector<Employee> &v_emp){
    Employee temp;
    bool isDuplicate = false;
    do{
        cout << "Employee id: ";
        cin >> temp.empId;
        isDuplicate = false;
        if(searchEmployee(v_emp, temp.empId) != -1){
            isDuplicate = true;
            cout << "Employee id: " << temp.empId
            << " already exists. Enter unique id " << endl;
        }
    } while(isDuplicate);
    cout << "Name: ";
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    std::getline(cin, temp.name);

    cout << "Basic Salary($): ";
    cin >> temp.basicSalary;
    cout << "PF($): ";
    cin >> temp.pf;
    cout << "Health Ins($): ";
    cin >> temp.healthInsAmt;
    cout << "Email: ";
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    std::getline(cin, temp.email);

    v_emp.push_back(temp);
    cout << "Employee with id: " << temp.empId << " added successfully" << endl;
    cout << v_emp.size() << " records in datafile" << endl << endl;

}

int searchEmployee(vector<Employee> & v_emp, int targetId){
    for(int i = 0; i < v_emp.size(); i++)
        if(v_emp[i].empId == targetId) return i;
    return -1;
}

void doTask(vector<Employee> &v_emp, int option){
    int targetId;
    int index;
    switch(option){
    case 1:
        addEmployee(v_emp);
        cout << endl;
        break;
    case 2:
        printEmployees(v_emp);
        cout << endl;
        break;
    case 3:
        cout << "Enter an id to search: ";
        cin >> targetId;
        index = searchEmployee(v_emp, targetId);
        if(index == -1) cout << "Employee id not found..." << endl;
        else {
            displayHeaders();
            printEmployee(v_emp[index]);
            cout << endl;
        }
        break;
    case 4:
        cout << "Enter an id to delete: ";
        cin >> targetId;
        deleteEmployee(v_emp, targetId);
        cout << endl;
        break;
    case 5:
        saveToFile(v_emp);
        cout << endl;
        break;
    default:
        cout << "Invalid input, select 1 - 6" << endl;
    }
}

void printEmployees(vector<Employee> v_emp){

    displayHeaders();

    double totalBasic = 0.0;
    double totalPfDeduction = 0.0;
    double totalHealthIns = 0.0;
    double totalNetSalary = 0.0;

    for(vector<Employee>::iterator it = v_emp.begin(); it != v_emp.end(); it++){
        printEmployee(*it);

        totalBasic += it->basicSalary;
        totalPfDeduction += it->pf;
        totalHealthIns += it->healthInsAmt;
        totalNetSalary += getNetSalary(*it);
    }

    cout << setw(TOTAL_WIDTH) << setfill('-') << " " << endl;
    cout << setfill(' ');

    cout << setw(EMP_ID_WIDTH)      << left << "Total"
         << setw(EMP_NAME_WIDTH)    << left << "In ($)"
         << setw(EMP_EMAIL_WIDTH)   << " "
         << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << right << fixed << totalBasic
         << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << right << fixed << totalPfDeduction
         << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << right << fixed << totalHealthIns
         << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << right << fixed << totalNetSalary
         << endl;

}

void printEmployee(Employee e){
    cout << setw(EMP_ID_WIDTH)      << left  << e.empId
         << setw(EMP_NAME_WIDTH)    << left  << e.name
         << setw(EMP_EMAIL_WIDTH)   << left  << e.email
         << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << fixed << right << e.basicSalary
         << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << fixed << right << e.pf
         << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << fixed << right << e.healthInsAmt
         << setw(EMP_NUMERIC_WIDTH) << setprecision(2) << fixed << right << getNetSalary(e)
         << endl;
}

double getNetSalary(Employee e){
    return e.basicSalary - (e.pf + e.healthInsAmt);
}

void saveToFile(vector<Employee> v_emp){
    ofstream fout("emp.txt");
    if(!fout){
        cout << "Unable to open data file..." << endl;
        return;
    }
    int recCount = 0;
    for(vector<Employee>::iterator it = v_emp.begin(); it != v_emp.end(); it++){
        fout << it->empId << "," << it->name << "," << it->basicSalary << ","
             << it->pf << "," << it->healthInsAmt << "," << it->email;
        recCount++;
        if(recCount != v_emp.size()) fout << endl;
    }
    fout.close();
    cout << endl << "Record has been updated..." << endl;
}

void deleteEmployee(vector<Employee> &v_emp, int targetId){
    int index = searchEmployee(v_emp, targetId);
    if(index == -1) {
        cout << "Target id does not exist..." << endl;
        return;
    }
    cout << "Employee found..." << endl;
    displayHeaders();
    printEmployee(v_emp[index]);
    cout << endl << "Target cannot be recovered once deleted. Would you like to continue?" << endl;
    cout << "Select 1 to delete or 0 to exit" << endl << endl;
    int choice;
    bool option = false;
    while(!option){
        cin >> choice;
        if(choice == 1){
            v_emp.erase(v_emp.begin() + index);
            cout << "Target successfully deleted..." << endl;
            option = true;
        }
        else if(choice == 0){
            cout << "Delete aborted..." << endl;
            return;
        }
        else{
            cout << "Invalid input. Select 1 to delete or 0 to exit" << endl;
        }
    }

}

void displayHeaders(){
    cout << endl;
    cout << setw(EMP_ID_WIDTH)      << left  << "EmpId"
         << setw(EMP_NAME_WIDTH)    << left  << "Name"
         << setw(EMP_EMAIL_WIDTH)   << left  << "Email"
         << setw(EMP_NUMERIC_WIDTH) << left  << "Basic($)"
         << setw(EMP_NUMERIC_WIDTH) << right << "PF($)"
         << setw(EMP_NUMERIC_WIDTH) << right << "HltIns($)"
         << setw(EMP_NUMERIC_WIDTH) << right << "Net($)"
         << endl;

    cout << setw(TOTAL_WIDTH) << setfill('-') << " " << endl;
    cout << setfill(' ');
}


int main(){
    vector<Employee> v_emp;
    readFile(v_emp);

    cout << "Welcome to the Employee records application."
         << " There are currently " << v_emp.size()
         << " records in data file" << endl
         << "Note: no changes are permanent until the 'Save' option is used"
         << endl << endl;
    printMenu();

    bool quit = false;
    while(!quit){
        cout << "Select an option: ";
        int option;
        cin >> option;
        if(option == 6) quit = true;
        else doTask(v_emp, option);
    }

    cout << "Thank you for using our service..." << endl;

    return 0;
}
