#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <array>
#include <algorithm>
#include "course.h"

#define FIRST_DAY_IN_WEEK 1
#define LAST_DAY_IN_WEEK 6
#define DAY_FROM_HOUR 8
#define DAY_UNTIL_HOUR 23

constexpr uint8_t DAYS_IN_WEEK = LAST_DAY_IN_WEEK - FIRST_DAY_IN_WEEK + 1;
constexpr uint8_t HOURS_IN_DAY = DAY_UNTIL_HOUR - DAY_FROM_HOUR;
const string DAYS_NAMES[DAYS_IN_WEEK] = {"א", "ב", "ג", "ד", "ה", "ו"};

class Schedule
{
private:
    array<array<const CourseInstanceTimeSlot*, HOURS_IN_DAY>, DAYS_IN_WEEK> timeTable;
    // Outer array is for days, inner array is for hours;
    vector<const CourseInstance*> allInstances;
public:
    Schedule();

    const CourseInstanceTimeSlot *getTimeSlot(uint8_t day, uint8_t hour);
    void addCourseInstance(const CourseInstance *instance);
    void removeCourseInstance(uint64_t instanceId);
    vector<CourseInstance *> constradictions(const CourseInstance *instance) const;
};

#endif // SCHEDULE_H
