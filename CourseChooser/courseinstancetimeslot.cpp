#include "courseinstancetimeslot.h"

CourseInstanceTimeSlot::CourseInstanceTimeSlot(CourseInstance *instance, uint8_t dayInWeek, uint8_t fromHourInDay, uint8_t toHourInDay, string classInBuildings):
    instance(instance), dayInWeek(dayInWeek), classInBuildings(classInBuildings)
{
    if (fromHourInDay > toHourInDay)
        throw "A time slot can't end before it begun!";
    this->fromHourInDay = fromHourInDay;
    this->toHourInDay = toHourInDay;
}

CourseInstanceTimeSlot::CourseInstanceTimeSlot(CourseInstance *instance):
    CourseInstanceTimeSlot(instance, 0, 0, 0, "")
{

}

CourseInstanceTimeSlot::CourseInstanceTimeSlot(const CourseInstanceTimeSlot& other)
{
    *this = other;
}

CourseInstanceTimeSlot::CourseInstanceTimeSlot(CourseInstanceTimeSlot&& other)
{
    *this = other;
    other.classInBuildings = "";
    other.dayInWeek = 0;
    other.fromHourInDay = 0;
    other.toHourInDay = 0;
    other.instance = nullptr;
}

CourseInstance *CourseInstanceTimeSlot::getInstance() const
{
    return instance;
}

void CourseInstanceTimeSlot::setInstance(CourseInstance *newInstance)
{
    instance = newInstance;
}

uint8_t CourseInstanceTimeSlot::getDayInWeek() const
{
    return dayInWeek;
}

void CourseInstanceTimeSlot::setDayInWeek(uint8_t newDayInWeek)
{
    dayInWeek = newDayInWeek;
}

uint8_t CourseInstanceTimeSlot::getFromHourInDay() const
{
    return fromHourInDay;
}

void CourseInstanceTimeSlot::setFromHourInDay(uint8_t newFromHourInDay)
{
    fromHourInDay = newFromHourInDay;
}

uint8_t CourseInstanceTimeSlot::getToHourInDay() const
{
    return toHourInDay;
}

void CourseInstanceTimeSlot::setToHourInDay(uint8_t newToHourInDay)
{
    toHourInDay = newToHourInDay;
}

string CourseInstanceTimeSlot::getClassInBuildings() const
{
    return classInBuildings;
}

void CourseInstanceTimeSlot::setClassInBuildings(string newClassInBuildings)
{
    classInBuildings = newClassInBuildings;
}

// Checks for intersection in time slots: O(1)
bool intersection(const CourseInstanceTimeSlot &slotA, const CourseInstanceTimeSlot &slotB)
{
    return ((slotA.dayInWeek == slotB.dayInWeek) && (slotA.fromHourInDay > slotB.toHourInDay) && (slotA.toHourInDay < slotB.fromHourInDay));
}

CourseInstanceTimeSlot& CourseInstanceTimeSlot::operator=(const CourseInstanceTimeSlot &other)
{
    this->classInBuildings = other.classInBuildings;
    this->dayInWeek = other.dayInWeek;
    this->fromHourInDay = other.fromHourInDay;
    this->toHourInDay = other.toHourInDay;
    this->instance = other.instance;

    return *this;
}
