//
// Created by Shorouk Magdy on 1/29/2021.
//

#ifndef LMS_STUDENT_H
#define LMS_STUDENT_H

#include <fstream>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <sstream>
#include <numeric>
#include <cmath>
#include <iterator>
#include <limits>
using namespace std;

class Student {

    vector <pair<string, string>> grades;
    string name, id;
    string gender;
    int age;

public:
    friend void print_data(const string& id);
    friend void import_info(const string& filename);
    friend void import_grades(const string& filepath, string id);
    friend void export_normalized_grades (const string& exp_filepath, const string& filepath);

};

void print_data(const string& id);
void import_info(const string& filename);
void import_grades(const string& filepath, string id);
void export_normalized_grades (const string& exp_filepath, const string& filepath);

vector<string> import_header(const string& filepath);
vector<vector<double>> import_all_grades(const string& filepath);
double normalize(const string& filepath, int indx, int grade);
void compute_stats (const string& filepath);
string to_gpa (int grade);
double compute_mean (vector<double> grades);
double compute_sigma(double mean, vector<double> grades);

#endif //LMS_STUDENT_H
