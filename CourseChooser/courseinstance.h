#ifndef COURSEINSTANCE_H
#define COURSEINSTANCE_H

#include <vector>
#include "courseinstancetimeslot.h"

class Course;

class CourseInstance
{
private:
    Course *course;
    uint64_t id;
    string lecturer;
    bool active;
    vector<CourseInstanceTimeSlot> timeSlots;

    ssize_t findTimeSlotIndex(uint8_t dayInWeek, uint8_t hourInDay);
public:
    CourseInstance(Course *course = nullptr, uint64_t id = 0, bool active = true);
    CourseInstance(const CourseInstance& other);
    CourseInstance(CourseInstance&& other);

    Course *getCourse() const;
    void setCourse(Course *newCourse);
    uint64_t getId() const;
    void setId(uint64_t newId);
    const string &getLecturer() const;
    void setLecturer(string newLecturer);
    bool getActive() const;
    void setActive(bool newActive);
    const vector<CourseInstanceTimeSlot> &getTimeSlots() const;

    void addTimeSlot(CourseInstanceTimeSlot &timeSlot);
    void removeTimeSlot(uint8_t dayInWeek, uint8_t hourInDay);

#if (__cplusplus >= 202002L)
    int64_t operator<=>(const CourseInstance &other) const;
#else
    bool operator<(const CourseInstance &other) const;
    bool operator>(const CourseInstance &other) const;
    bool operator==(const CourseInstance &other) const;
#endif

    CourseInstance& operator=(const CourseInstance &other);
};

#endif // COURSEINSTANCE_H
