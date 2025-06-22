//============================================================================
// Name        : FinalProject.cpp
// Author      : Brooke Slampak
// Version     : 1.0
//============================================================================

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Declares a structure named Course to hold course components
struct Course {
    string courseId;
    string courseName;
    vector<string> prerequisites;
};

// Stores and manages Course objects
class HashTable {
private:
    // Sets fixed size of hash table to 100
    static const int TABLE_SIZE = 100;
    // Creates a vector of size 100 that makes up the hash table
    vector<list<Course>> table;

    // Generates an index from a string
    int hash(const string& key) const {
        int value = 0;
        for (char c : key) value += c;
        return value % TABLE_SIZE;
    }

public:
    // Initializes the hash table with 100 empty buckets
    HashTable() : table(TABLE_SIZE) {}

    // Inserts a course into the table
    void insert(const Course& course) {
        // Finds the correct bucket index
        int idx = hash(course.courseId);
        // Adds the course to the list
        table[idx].push_back(course);
    }

    // Searches for a course by its ID
    Course* search(const string& courseId) {
        // Hashes courseId to find the right bucket
        int idx = hash(courseId);
        // Iterates through the list to find a match
        for (auto& course : table[idx]) {
            if (course.courseId == courseId)
                return &course;
        }
        // Returns nullptr if course not found
        return nullptr;
    }

    // Returns a vector of all courses in the hash table
    vector<Course> getAllCourses() {
        vector<Course> result;
        // Iterates over every bucket 
        // For each list inside the bucket, appends each course to the result
        for (auto& bucket : table) {
            for (auto& course : bucket)
                result.push_back(course);
        }
        return result;
    }
};

// Converts string to all uppercase letters
void toUpperCase(string& s) {
    for (auto& c : s) c = toupper(c);
}

// reads course data from a CSV file and inserts it into hash table
void loadCourses(HashTable& table, const string& filename) {
    // Creates input file stream to read the file
    ifstream file(filename);
    // If file can't be opened, outputs an error
    if (!file.is_open()) {
        cout << "Failed to open file.\n";
        return;
    }

    // Reads the file line by line
    string line;
    while (getline(file, line)) {
        // Creates a stringstream from the line so it can be split using commas
        stringstream ss(line);
        // id and name store the first two fields
        // token is used to read each prerequisite
        string id, name, token;
        // Creates a new course object to fill in
        Course course;

        // Extracts the course ID and course name from the stringstream, up to the first and second commas
        getline(ss, id, ',');
        getline(ss, name, ',');

        // Stores the extracted values into the Course object
        course.courseId = id;
        course.courseName = name;

        // For any remaining values in the line after ID and name, they are prerequisites
        // Each one is pushed into the course's prerequisites vector
        while (getline(ss, token, ',')) {
            course.prerequisites.push_back(token);
        }

        // Inserts the fully populated Course object into the hash table
        table.insert(course);
    }

    cout << "Courses loaded successfully.\n";
}

// Outputs a sorted list of courses to the console
void printCourseList(HashTable& table) {
    auto courses = table.getAllCourses();
    sort(courses.begin(), courses.end(), [](Course a, Course b) {
        return a.courseId < b.courseId;
        });

    cout << "Here is a sample schedule:\n";
    for (auto& course : courses) {
        if (course.courseId.find("CSCI") == 0 || course.courseId.find("MATH") == 0)
            cout << course.courseId << ", " << course.courseName << "\n";
    }
}

// Prompts the user for a course ID, looks it up, and outputs info
void printCourseInfo(HashTable& table) {
    string inputId;
    cout << "What course do you want to know about? ";
    cin >> inputId;
    toUpperCase(inputId);

    Course* course = table.search(inputId);
    if (!course) {
        cout << "Course not found.\n";
        return;
    }

    cout << course->courseId << ", " << course->courseName << "\n";
    if (!course->prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1)
                cout << ", ";
        }
        cout << "\n";
    }
    else {
        cout << "Prerequisites: None\n";
    }
}

// Main function that handles menu selection
int main() {
    HashTable courseTable;
    bool loaded = false;
    int choice;

    cout << "Welcome to the course planner.\n";

    do {
        cout << "\n1. Load Data Structure\n"
            << "2. Print Course List\n"
            << "3. Print Course\n"
            << "9. Exit\n"
            << "What would you like to do? ";

        cin >> choice;

        switch (choice) {
        case 1: {
            string filename;
            cout << "Enter file name: ";
            cin >> filename;
            loadCourses(courseTable, filename);
            loaded = true;
            break;
        }
        case 2:
            if (loaded)
                printCourseList(courseTable);
            else
                cout << "Please load the data first.\n";
            break;
        case 3:
            if (loaded)
                printCourseInfo(courseTable);
            else
                cout << "Please load the data first.\n";
            break;
        case 9:
            cout << "Thank you for using the course planner!\n";
            break;
        default:
            cout << choice << " is not a valid option.\n";
            break;
        }

    } while (choice != 9);

    return 0;
}