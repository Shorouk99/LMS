//
// Created by Shorouk Magdy on 1/29/2021.
//
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <numeric>
#include <cmath>
#include <limits>
#include "student.h"
using namespace std;

map<string, Student> database;
void import_info(const string& filename) {
    /**
     * This function creates a new Student object for every ID in the csv file,
     * and assigns the name, age, and gender attributes to it.
     * Each Student object is saved in a map dictionary "database" with its key as the student's ID
     */
    ifstream datafile (filename);
    if (!datafile.is_open()) throw runtime_error("Could not open file");

    string line, data, id;
    getline(datafile, line);

    while (getline(datafile, line)) {
        stringstream ss (line);
        int indx = 0;
        while (getline(ss, data, ',')){
            if (indx == 0) {
                id = data;
                database[id].id = id;
            }
            if (indx == 1) database[id].name = data;
            if (indx == 2) database[id].age = stoi(data);
            if (indx == 3) database[id].gender = data;
            indx ++;
        }

    }
    datafile.close();

}
string to_gpa (int grade){
    /**
     * This function transforms numeric grade to its corresponding GPA
     */
    if (grade >= 90) return "A";
    else if (grade >= 85) return "B+";
    else if (grade >= 80) return "B";
    else if (grade >= 75) return "C+";
    else if (grade >= 70) return "C";
    else if (grade >= 65) return "D+";
    else if (grade > 60) return "D";
    else return "F";
}
void import_grades(const string& filepath, string id) {
    /**
     * This function imports raw grades of each student, and assign it to its corresponding Student object in the
     * database dictionary after normalization and conversion to GPA
     */
    vector<string> subj_col;
    ifstream datafile (filepath);
    if (!datafile.is_open()) throw runtime_error("Could not open file");
    string line, subj, val;
    getline(datafile, line);
    stringstream ss (line);
    while (getline(ss, subj, ',')){
        subj_col.push_back(subj);
    }
    while (getline(datafile,line)) {
        stringstream ss (line);
        int colindx = 0;
        while (getline(ss, val, ',')) {
            if (colindx == 0) {
                id = val;
                database[id].grades.clear();
            }
            else {
                database[id].grades.push_back({subj_col.at(colindx),to_gpa(normalize(filepath, colindx, stoi(val)))});
                //cout << normalize(filepath, colindx, stoi(val)) << endl;
            }
            colindx++;
        }
    }
    datafile.close();
}
void print_data(const string& id) {
    /**
     * This function prints the attributes of each Student object in the database dictionary
     * (name, age, gender, and grades)
     */
    cout << "Name : " << database[id].name << endl;
    cout << "Age : " << database[id].age << endl;
    cout << "Gender : " << database[id].gender << endl;
    cout << "\nGrades\n" << "------------------" << endl;
    for (auto& subj : database[id].grades) {
        cout << subj.first << " : " << subj.second << endl;
    }
}
double compute_mean (vector<double> grades){
    /**
     * This function computes the mean of a given vector of numbers
     */
    if (grades.empty()) return numeric_limits<double>::quiet_NaN();
    return accumulate(grades.begin(), grades.end(), 0.0) / double (grades.size());
}
double compute_sigma(double mean, vector<double> grades) {
    /**
     * This function computes the standard deviation of a distribution, given their mean
     */
    if (grades.size() <= 1) return numeric_limits<double>::quiet_NaN();
    for (double & grade : grades){
        grade = pow(grade-mean, 2);
    }
    return sqrt(accumulate(grades.begin(), grades.end(), 0.0) / double(grades.size()-1));
}
void export_normalized_grades (const string& exp_filepath, const string& filepath){
    /**
     * This function exports grades after being normalized and transformed to GPA to a file of choice
     */
    vector<string> header = import_header(filepath);
    string row = "";
    for (const auto& word : header) {
        row.append(word);
        row.append(",");
    }
    row.erase(row.size()-1);
    row.append("\n");
    ofstream datafile (exp_filepath);
    datafile << row;
    for (const auto& student : database) {
        row.clear();
        row.append(student.first + ",");
        import_grades(filepath, student.first);
        for (const auto & grade : student.second.grades){
            row.append(grade.second + ",");
        }
        row.erase(row.size()-1);
        row.append("\n");
        datafile << row;
    }


}
vector<vector<double>> import_all_grades(const string& filepath){
    /**
     * This function import grades to a vector of vectors where each subject resides in a vector
     * to aid in calculations requiring the full data of each subject
     * like normalization
     */
    vector<vector<double>> all_grades;
    ifstream datafile (filepath);
    if (!datafile.is_open()) throw runtime_error("Could not open file");
    string line, subj, val;
    getline(datafile, line);
    getline(datafile, line);
    stringstream ss (line);
    vector<double> subj_col;
    getline(ss, val, ',');
    while (getline(ss, val, ',')) {
        subj_col.clear();
        subj_col.push_back(stod(val));
        all_grades.push_back(subj_col);
    }
    while (getline(datafile,line)){
        stringstream ss (line);
        int colindx = 0;
        getline(ss, val, ',');
        while (getline(ss, val, ',')){
            all_grades.at(colindx).push_back(stod(val));
            colindx++;
        }
    }
    return all_grades;
}
vector<string> import_header(const string& filepath){
    /**
     * This function creates a vector containing the subjects' names to aid in other functions like exporting
     * where the grades are stripped of their corresponding header
     */
    vector<string> header;
    ifstream datafile (filepath);
    if (!datafile.is_open()) throw runtime_error("Could not open file");
    string line, subj;
    getline(datafile, line);
    stringstream ss (line);
    while(getline(ss, subj, ',')) {
        header.push_back(subj);
    }
    return header;
}
double normalize(const string& filepath, int indx, int grade){
    /**
     * This function normalizes the grades by calculating the Z-score of each grade
     * then transforming the grades distribution to a new distribution
     * having target mean = 70, and target standard deviation = 15
     */
    vector<vector<double>> all_grades = import_all_grades(filepath);
    double mean = compute_mean(all_grades.at(indx-1));
    double sigma = compute_sigma(mean, all_grades.at(indx-1));
    double z_score =  (double (grade) - mean) / sigma;
    return 70.0 + z_score * 15.0;
}
void compute_stats (const string& filepath) {
    /**
     * This function computes statistics if each subject, showing the percentage of each grade band in it
     */
    vector<string> header = import_header(filepath);
    vector<vector<double>> all_grades = import_all_grades(filepath);
    int indx = 1;
    for (const auto& subj : all_grades) {
        double a_count = 0, b_count = 0, c_count = 0, d_count = 0, f_count = 0;
        for (auto grade : subj) {
            string gpa = to_gpa(normalize(filepath, indx, grade));
            if (gpa == "A") a_count++;
            else if (gpa == "B" || gpa == "B+") b_count++;
            else if (gpa == "C" || gpa == "C+") c_count++;
            else if (gpa == "D" || gpa == "D+") d_count++;
            else f_count++;
        }
        a_count = (double (a_count)/double (all_grades.at(1).size()))*100.0;
        b_count = (double (b_count)/double (all_grades.at(1).size()))*100.0;
        c_count = (double (c_count)/double (all_grades.at(1).size()))*100.0;
        d_count = (double (d_count)/double (all_grades.at(1).size()))*100.0;
        f_count = (double (f_count)/double (all_grades.at(1).size()))*100.0;
        cout << "Grades statistics for " << header.at(indx) << " is:\n";
        cout << "A percentage is: " << a_count << "%\n";
        cout <<"B percentage is: " << b_count << "%\n";
        cout <<"C percentage is: " << c_count << "%\n";
        cout << "D percentage is: " << d_count << "%\n";
        cout << "F percentage is: " << f_count << "%\n\n";
        indx++;
    }




}
