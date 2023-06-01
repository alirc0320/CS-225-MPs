/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
//  */

// This function reads the contents of a file and returns them as a V2D (vector of vectors of strings)
V2D file_to_V2D(const std::string& filename) {
    // Open the file for reading
    std::ifstream infile(filename);

    // Initialize variables to hold each line of the file and the split strings
    std::string line;
    std::vector<std::string> split_line;
    V2D contents;

    // Read each line of the file and split it into separate strings using a delimiter
    while (std::getline(infile, line)) {
        split_line.clear();

        // Split the line using a comma as the delimiter
        size_t pos = 0;
        while (pos < line.length()) {
            size_t endpos = line.find(',', pos);
            if (endpos == std::string::npos) endpos = line.length();
            split_line.push_back(trim(line.substr(pos, endpos - pos)));
            pos = endpos + 1;
        }

        // Add the split strings to the contents vector
        contents.push_back(split_line);
    }

    // Close the file
    infile.close();

    // Return the contents of the file as a V2D
    return contents;
}
/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
//  */
V2D clean(const V2D& cv, const V2D& student) {
    // Create a new V2D object to store the clean data
    V2D cleanData;

    // Loop through each course in the CV data
    auto i = 0u;
    while (i < cv.size()) {
        // Create a new vector to hold the students for this course
        std::vector<std::string> thisCourse;

        // Get the name of the course
        std::string courseName = cv[i][0];
        thisCourse.push_back(courseName);

        // Loop through each student in the course
        auto j = 1u;
        while (j < cv[i].size()) {
            std::string studentName = cv[i][j];

            // Search for the student in the student data
            auto k = 0u;
            while (k < student.size()) {
                if (studentName == student[k][0]) {
                    // If the student is found, check if they took this course
                    auto l = 1u;
                    while (l < student[k].size()) {
                        if (student[k][l] == courseName) {
                            // If the student took the course, add them to the list for this course
                            thisCourse.push_back(studentName);
                            break;
                        }
                        ++l;
                    }
                    break;
                }
                ++k;
            }
            ++j;
        }

        // If there is more than one student for this course, add it to the clean data
        if (thisCourse.size() > 1) {
            cleanData.push_back(thisCourse);
        }
        ++i;
    }

    // Return the clean data
    return cleanData;
}
/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */

V2D schedule(const std::vector<std::vector<std::string>>& courses, const std::vector<std::string>& timeslots) {
    // Create an adjacency matrix to represent the relationships between courses.
    std::vector<std::vector<int>> adjacency_matrix;
    size_t i = 0;
    while(i < courses.size()) {
        adjacency_matrix.push_back(std::vector<int>(courses.size(), 0));
        i++;
    }
    i = 0;
    while(i < courses.size()) {
        size_t j = 0;
        while(j < i) {
            // Create sets of courses for each course vector, ignoring the course name which is at index 0.
            std::set<std::string> course1Set(courses[i].begin() + 1, courses[i].end());
            std::set<std::string> course2Set(courses[j].begin() + 1, courses[j].end());

            // Create an empty vector to hold the intersection of the two sets.
            std::vector<std::string> intersection;

            // Use the set_intersection function to find the common courses between the two sets.
            std::set_intersection(course1Set.begin(), course1Set.end(), course2Set.begin(), course2Set.end(), std::back_inserter(intersection));

            // Fill in the adjacency matrix by checking if courses i and j are connected.
            adjacency_matrix[i][j] = adjacency_matrix[j][i] = !intersection.empty() ? 1 : 0;
            j++;
        }
        i++;
    }

    // Initialize the list of course colors to -1, indicating that no color has been assigned to any course.
    std::vector<int> course_colors(courses.size(), -1);

    // Iterate over each course and try to assign a time slot to it.
    i = 0;
    while(i < courses.size()) {
        bool is_valid = true;
        // Iterate over each available time slot and try to assign the current course to one of them.
        size_t j = 0;
        while(j < timeslots.size()) {
            is_valid = true;
            // Iterate over each other course and check if the current course can be assigned to the current time slot without conflicting with any other course.
            size_t k = 0;
            while(k < courses.size()) {
                // If course i is connected to course k and they have the same color, then course i cannot be assigned to the current time slot.
                if (i != k && adjacency_matrix[i][k] && course_colors[k] == static_cast<int>(j)) {
                    is_valid = false;
                    break;
                }
                k++;
            }
            // If the current course can be assigned to the current time slot without conflicting with any other course, assign it to that time slot and break out of the loop.
            if(is_valid) {
                course_colors[i] = j;
                break;
            }
            j++;
        }
        // If no valid time slot was found for the current course, return an error schedule.
        if(!is_valid) {
            return V2D(1, std::vector<std::string>(1, "-1"));
        }
        i++;
    }

    // Create a 2D vector to represent the final schedule.
    V2D schedule_times(timeslots.size());
    // Add each time slot to the first column of the schedule.
    i = 0;
    while(i < timeslots.size()) {
        schedule_times[i].push_back(timeslots[i]);
        i++;
    }



    // Add each course to its assigned time slot in the schedule.
    i = 0;
    while(i < course_colors.size()) {
        // If the course has been assigned a time slot, add it to that time slot in the schedule.
        if(course_colors[i] != -1) {
            schedule_times[course_colors[i]].push_back(courses[i][0]);
        }
        i++;
    }

    // Return the final schedule.
    return schedule_times;
}
//Ali