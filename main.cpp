#include <string>
#include <iostream>
#include "student.h"
using namespace std;

int main()
{
    while (true) {
        string data_filepath;
        string grades_filepath;
        cout << "\nWelcome to the Learning Management System!\n\n";
        cout << "Pleases enter the path of a CSV file that contains the students' data\n";
        cin >> data_filepath;
        cout << "Pleases enter the path of a CSV file that contains the students' grades\n";
        cin >> grades_filepath;
        import_info(data_filepath);
        cout << "Choose from the menu below the action you want to perform:\n";
        cout
                << "1: Export normalized grades\n2: Display data and grades of a student using his/her ID\n3: Show Statistics of courses' grades\n";
        cout << "Please enter ONLY the number of your preferred action\n";
        int choice;
        cin >> choice;
        if (choice == 1) {
            cout << "Please enter the path to which you want to export the grades\n";
            string exp_pathfile;
            cin >> exp_pathfile;
            export_normalized_grades(exp_pathfile, grades_filepath);
        }
        else if (choice == 2) {
            string id;
            cout << "Please enter a valid ID\n";
            cin >> id;
            import_grades(grades_filepath, id);
            print_data(id);
        }
        else if (choice == 3) {
            compute_stats(grades_filepath);
        }
        else {
            cout << "Please enter a valid choice. Make sure it's a number: 1 or 2 or 3\n";
        }
        cout << "Do you wish to restart??\nEnter y or n\n";
            char res;
            cin >> res;
            if (res == 'n') break;
        while (true) {
            if (res != 'y') {
                cout
                        << "Please enter a valid choice. Make sure you enter a single letter y or n representing yes or no\n";
                cin >> res;
            }
            else break;
        }
    }
}

