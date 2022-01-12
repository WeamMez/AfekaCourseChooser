#include "schedule.h"

// A "private" macro.
#define SCHEDULE_TIME_SLOT(day,hour) timeTable[day-FIRST_DAY_IN_WEEK][hour-DAY_FROM_HOUR]

Schedule::Schedule()
{
    for (size_t i = 0; i < timeTable.size(); i++)
        timeTable[i].fill(nullptr);
}

const CourseInstanceTimeSlot *Schedule::getTimeSlot(uint8_t day, uint8_t hour)
{
    return SCHEDULE_TIME_SLOT(day, hour);
}

void Schedule::addCourseInstance(const CourseInstance *instance)
{
    //
    removeCourseInstance(instance->getId());

    allInstances.push_back(instance);

    const vector<CourseInstanceTimeSlot>& timeslots = instance->getTimeSlots();
    for (size_t i = 0; i < timeslots.size(); i++)
        for (uint8_t j = timeslots[i].getFromHourInDay(); j < timeslots[i].getToHourInDay(); j++)
            SCHEDULE_TIME_SLOT(timeslots[i].getDayInWeek(), j) = &timeslots[i];
}

void Schedule::removeCourseInstance(uint64_t instanceId)
{
    const CourseInstance *instance = nullptr;
    vector<const CourseInstance*>::iterator it;
    for (it = allInstances.begin(); it < allInstances.end(); it++)
        if ((*it)->getId() == instanceId)
        {
            instance = (*it);
            break;
        }
    if (instance != nullptr)
    {
        const vector<CourseInstanceTimeSlot> &times = (*it)->getTimeSlots();
        for (vector<CourseInstanceTimeSlot>::const_iterator timesIt = times.begin(); timesIt != times.end(); timesIt++)
        {
            uint8_t slotFrom = (*timesIt).getFromHourInDay(), slotTo = (*timesIt).getToHourInDay();
            uint8_t slotDay = (*timesIt).getDayInWeek();
            for (uint8_t j = slotFrom; j < slotTo; j++)
                timeTable[slotDay][j] = nullptr;
        }
        allInstances.erase(it);
    }
}

vector<CourseInstance *> Schedule::constradictions(const CourseInstance *instance) const
{
    vector<CourseInstance *> ans;
    const vector<CourseInstanceTimeSlot> &times = instance->getTimeSlots();
    for (size_t i = 0; i < times.size(); i++)
    {
        uint8_t slotFrom = times[i].getFromHourInDay(), slotTo = times[i].getToHourInDay();
        uint8_t slotDay = times[i].getDayInWeek();

        for (uint8_t j = slotFrom; j < slotTo; j++)
        {
            if (SCHEDULE_TIME_SLOT(slotDay, j) && find(ans.begin(), ans.end(), SCHEDULE_TIME_SLOT(slotDay, j)->getInstance()) == ans.end())
                ans.push_back(SCHEDULE_TIME_SLOT(slotDay, j)->getInstance());
        }
    }

    return ans;
}
