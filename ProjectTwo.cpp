#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

// Structure to represent a Course
struct Course {
    string number;
    string title;
    vector<string> prerequisites;
};

// Structure to represent a Hash Table
struct HashTable {
    vector<list<Course>> table;
    int size;

    HashTable(int size) : size(size) {
        table.resize(size);
    }
};

// Hash function to map course number to an index in the table
int hashFunction(const string& courseNumber, int size) {
    hash<string> hasher;
    return hasher(courseNumber) % size;
}

// Function to create a Course object from course data
Course createCourse(const vector<string>& courseData) {
    Course course;
    course.number = courseData[0];
    course.title = courseData[1];
    for (size_t i = 2; i < courseData.size(); ++i) {
        course.prerequisites.push_back(courseData[i]);
    }
    return course;
}

// Function to check if the line format is valid
bool isValidFormat(const string& line) {
    vector<string> courseData;
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) {
        courseData.push_back(item);
    }
    return courseData.size() >= 2;
}

// Function to load course data from a file into the hash table
void loadCoursesFromFile(const string& filename, HashTable& hashTable) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "Error: Unable to open the file" << endl;
        return;
    }

    while (getline(file, line)) {
        if (isValidFormat(line)) {
            stringstream ss(line);
            string item;
            vector<string> courseData;
            while (getline(ss, item, ',')) {
                courseData.push_back(item);
            }
            Course course = createCourse(courseData);
            int index = hashFunction(course.number, hashTable.size);
            hashTable.table[index].push_back(course);
        }
        else {
            cout << "Error: Invalid file format" << endl;
        }
    }

    file.close();
}

// Function to print course information and prerequisites
void printCourseInfoAndPrerequisites(const Course& course) {
    cout << "Course Number: " << course.number << endl;
    cout << "Course Title: " << course.title << endl;
    cout << "Prerequisites: ";
    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    }
    else {
        for (const auto& prereq : course.prerequisites) {
            cout << prereq << " ";
        }
        cout << endl;
    }
}

// Function to find a course by its number
Course* findCourseByNumber(const string& courseNumber, const list<Course>& courseList) {
    for (const auto& course : courseList) {
        if (course.number == courseNumber) {
            return const_cast<Course*>(&course);
        }
    }
    return nullptr;
}

// Function to print an alphabetically ordered list of courses
void printAlphabeticallyOrderedCourses(const HashTable& hashTable) {
    vector<Course> allCourses;
    for (const auto& bucket : hashTable.table) {
        for (const auto& course : bucket) {
            allCourses.push_back(course);
        }
    }

    sort(allCourses.begin(), allCourses.end(), [](const Course& a, const Course& b) {
        return a.number < b.number;
        });

    for (const auto& course : allCourses) {
        printCourseInfoAndPrerequisites(course);
    }
}

// Function to display the menu
void displayMenu() {
    cout << "Menu:" << endl;
    cout << "1. Load file data" << endl;
    cout << "2. Print alphabetically ordered list of courses" << endl;
    cout << "3. Print course information and prerequisites" << endl;
    cout << "9. Exit" << endl;
}

int main() {
    HashTable hashTable(10);  // Initial hash table size, can be adjusted as needed
    int userInput;

    do {
        displayMenu();
        cin >> userInput;

        switch (userInput) {
        case 1:
        {
            string filename;
            cout << "Enter file name: ";
            cin >> filename;
            loadCoursesFromFile(filename, hashTable);
        }
        break;
        case 2:
            printAlphabeticallyOrderedCourses(hashTable);
            break;
        case 3:
        {
            string courseNumber;
            cout << "Enter course number: ";
            cin >> courseNumber;
            int index = hashFunction(courseNumber, hashTable.size);
            Course* course = findCourseByNumber(courseNumber, hashTable.table[index]);
            if (course == nullptr) {
                cout << "Course not found" << endl;
            }
            else {
                printCourseInfoAndPrerequisites(*course);
            }
        }
        break;
        case 9:
            cout << "Exiting program" << endl;
            break;
        default:
            cout << "Invalid option" << endl;
        }
    } while (userInput != 9);

    return 0;
}
