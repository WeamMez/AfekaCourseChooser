#ifndef COURSEINSTANCETIMESLOT_H
#define COURSEINSTANCETIMESLOT_H

#include <cstdint>
#include <string>

class CourseInstance;

using namespace std;

class CourseInstanceTimeSlot
{
private:
    CourseInstance *instance;
    uint8_t dayInWeek, fromHourInDay, toHourInDay;
    string classInBuildings;
public:
    CourseInstanceTimeSlot(CourseInstance *instance, uint8_t dayInWeek, uint8_t fromHourInDay, uint8_t toHourInDay, string classInBuildings) noexcept(false);
    CourseInstanceTimeSlot(CourseInstance *instance = nullptr);
    CourseInstanceTimeSlot(const CourseInstanceTimeSlot& other);
    CourseInstanceTimeSlot(CourseInstanceTimeSlot&& other);

    CourseInstance *getInstance() const;
    void setInstance(CourseInstance *newInstance);
    uint8_t getDayInWeek() const;
    void setDayInWeek(uint8_t newDayInWeek);
    uint8_t getFromHourInDay() const;
    void setFromHourInDay(uint8_t newFromHourInDay);
    uint8_t getToHourInDay() const;
    void setToHourInDay(uint8_t newToHourInDay);
    string getClassInBuildings() const;
    void setClassInBuildings(string newClassInBuildings);

    friend bool intersection(const CourseInstanceTimeSlot &slotA, const CourseInstanceTimeSlot &slotB);

    CourseInstanceTimeSlot& operator=(const CourseInstanceTimeSlot& other);
};

#endif // COURSEINSTANCETIMESLOT_H
