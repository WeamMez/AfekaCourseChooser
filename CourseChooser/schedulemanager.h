#ifndef SCHEDULEMANAGER_H
#define SCHEDULEMANAGER_H

#include <bits/stdc++.h>
#include "schedule.h"

#define COURSE_END_ID 100000
#define COURSE_FROM_INSTANCE_ID(id) (id/100)%100000

class ScheduleManager
{
private:
    vector<Schedule> allSchedules;
    vector<Course> allCourses;

    // Binary search for course, if not found, return the place it would have been if it was there.
    vector<Course>::iterator binarySearchCourse(int32_t id);

    // Calculate the number of theoretical schedule options. basically multiply number of
    // instances of each course.
    //uint64_t calcuateTheoreticalCount();
public:
    ScheduleManager();
    bool addCourse(Course &newCourse);
    Course *getCourseById(int32_t id);
    void removeCourse(int32_t id);
    bool addCourseInstance(uint64_t id, string lecturer);
    void removeCourseInstance(uint64_t id);
    void calculate();
    Schedule *getScheduleByIndex(size_t index);
    size_t getScheduleCount();
    size_t getCourseCount();
    Course *getCourseByIndex(size_t index);
    size_t getCourseIndexById(int32_t id);
    CourseInstance *getCourseInstanceById(uint64_t id);
    const vector<Course> *getCourseVec();
};

#endif // SCHEDULEMANAGER_H
