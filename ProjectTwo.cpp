/*
 * ProjectTwo.cpp
 *
 *  Created on: Apr 8, 2026
 *      Author: Ashlynn Borro
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Course object
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Remove leading and trailing spaces
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");

    if (first == string::npos || last == string::npos) {
        return "";
    }

    return str.substr(first, last - first + 1);
}

// Convert string to uppercase
string toUpperCase(string str) {
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}

// Find a course by course number
int findCourseIndex(const vector<Course>& courses, const string& courseNumber) {
    string searchKey = toUpperCase(courseNumber);

    for (size_t i = 0; i < courses.size(); i++) {
        if (toUpperCase(courses[i].courseNumber) == searchKey) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

// Load courses from CSV file into vector
void loadCoursesFromFile(const string& fileName, vector<Course>& courses) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file \"" << fileName << "\"." << endl;
        return;
    }

    courses.clear();
    string line;

    while (getline(file, line)) {
        if (trim(line).empty()) {
            continue;
        }

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        if (tokens.size() < 2) {
            cout << "Warning: Skipping invalid line: " << line << endl;
            continue;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); i++) {
            if (!tokens[i].empty()) {
                course.prerequisites.push_back(tokens[i]);
            }
        }

        courses.push_back(course);
    }

    file.close();
    cout << "Courses loaded successfully." << endl;
}

// Sort courses alphanumerically by course number
void sortCourses(vector<Course>& courses) {
    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
    });
}

// Print sorted course list
void printCourseList(vector<Course> courses) {
    if (courses.empty()) {
        cout << "No course data loaded." << endl;
        return;
    }

    sortCourses(courses);

    cout << "\nHere is a sample schedule:\n" << endl;
    for (size_t i = 0; i < courses.size(); i++) {
        cout << courses[i].courseNumber << ", " << courses[i].courseTitle << endl;
    }
}

// Print one course and its prerequisites
void printCourseInfo(const vector<Course>& courses, const string& courseNumber) {
    if (courses.empty()) {
        cout << "No course data loaded." << endl;
        return;
    }

    int index = findCourseIndex(courses, courseNumber);

    if (index == -1) {
        cout << "Course " << toUpperCase(courseNumber) << " not found." << endl;
        return;
    }

    const Course& course = courses[index];

    cout << "\n" << course.courseNumber << ", " << course.courseTitle << endl;

    cout << "Prerequisites: ";
    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    } else {
        for (size_t i = 0; i < course.prerequisites.size(); i++) {
            int prereqIndex = findCourseIndex(courses, course.prerequisites[i]);

            if (prereqIndex != -1) {
                cout << courses[prereqIndex].courseNumber << ", "
                     << courses[prereqIndex].courseTitle;
            } else {
                cout << course.prerequisites[i];
            }

            if (i < course.prerequisites.size() - 1) {
                cout << "; ";
            }
        }
        cout << endl;
    }
}

// Display menu
void displayMenu() {
    cout << "\nMenu:" << endl;
    cout << "  1. Load Data Structure" << endl;
    cout << "  2. Print Course List" << endl;
    cout << "  3. Print Course" << endl;
    cout << "  9. Exit" << endl;
    cout << "\nWhat would you like to do? ";
}

int main() {
    vector<Course> courses;
    int choice = 0;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        displayMenu();

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter 1, 2, 3, or 9." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                cout << "Enter file name: ";
                cin.ignore();
                getline(cin, fileName);
                loadCoursesFromFile(fileName, courses);
                break;

            case 2:
                printCourseList(courses);
                break;

            case 3:
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                printCourseInfo(courses, courseNumber);
                break;

            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;

            default:
                cout << choice << " is not a valid option." << endl;
                break;
        }
    }

    return 0;
}



