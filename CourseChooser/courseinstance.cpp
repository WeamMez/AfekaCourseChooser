#include "courseinstance.h"

CourseInstance::CourseInstance(Course *course, uint64_t id, bool active):
    course(course), id(id), active(active)
{

}

CourseInstance::CourseInstance(const CourseInstance& other)
{
    *this = other;
}

CourseInstance::CourseInstance(CourseInstance&& other)
{
    *this = other;
    other.active = false;
    other.course = nullptr;
    other.id = 0;
    other.lecturer = "";
    other.timeSlots.clear();

}

Course *CourseInstance::getCourse() const
{
    return course;
}

void CourseInstance::setCourse(Course *newCourse)
{
    course = newCourse;
}

uint64_t CourseInstance::getId() const
{
    return id;
}

void CourseInstance::setId(uint64_t newId)
{
    id = newId;
}

const string &CourseInstance::getLecturer() const
{
    return lecturer;
}

void CourseInstance::setLecturer(string newLecturer)
{
    lecturer = newLecturer;
}

bool CourseInstance::getActive() const
{
    return active;
}

void CourseInstance::setActive(bool newActive)
{
    active = newActive;
}

const vector<CourseInstanceTimeSlot> &CourseInstance::getTimeSlots() const
{
    return timeSlots;
}

// Add time slots and delete any time slot that exists there: O(n), n = timeSlots.size()
void CourseInstance::addTimeSlot(CourseInstanceTimeSlot &timeSlot)
{
    for (ssize_t i = 0; i < (ssize_t)timeSlots.size(); i++)
        if (intersection(timeSlots[i], timeSlot))
            timeSlots.erase(timeSlots.begin() + i);

    timeSlot.setInstance(this);
    timeSlots.push_back(timeSlot);
}

void CourseInstance::removeTimeSlot(uint8_t dayInWeek, uint8_t hourInDay)
{
    ssize_t index = findTimeSlotIndex(dayInWeek, hourInDay);
    timeSlots.erase(timeSlots.begin() + index);
}

ssize_t CourseInstance::findTimeSlotIndex(uint8_t dayInWeek, uint8_t hourInDay)
{
    ssize_t i;
    for (i = 0; i < (ssize_t)timeSlots.size(); i++)
        if (timeSlots[i].getDayInWeek() == dayInWeek && timeSlots[i].getFromHourInDay() <= hourInDay && timeSlots[i].getToHourInDay() > hourInDay)
        {
            return i;
        }

    return -1;
}

#if (__cplusplus >= 202002L)
int64_t CourseInstance::operator<=>(const CourseInstance &other) const
{
    return other.id - this->id;
}
#else
bool CourseInstance::operator<(const CourseInstance &other) const
{
    return this->id < other.id;
}
bool CourseInstance::operator>(const CourseInstance &other) const
{
    return this->id > other.id;
}
bool CourseInstance::operator==(const CourseInstance &other) const
{
    return this->id == other.id;
}
#endif

CourseInstance& CourseInstance::operator=(const CourseInstance &other)
{
    this->active = other.active;
    this->id = other.id;
    this->lecturer = other.lecturer;
    this->course = other.course;

    this->timeSlots.clear();
    this->timeSlots.resize(other.timeSlots.size());
    for (size_t i = 0; i < other.timeSlots.size(); i++)
    {
        this->timeSlots[i] = other.timeSlots[i];
        this->timeSlots[i].setInstance(this);
    }

    return *this;
}
