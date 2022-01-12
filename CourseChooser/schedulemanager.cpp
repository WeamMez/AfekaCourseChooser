#include "schedulemanager.h"

ScheduleManager::ScheduleManager()
{

}

vector<Course>::iterator ScheduleManager::binarySearchCourse(int32_t id)
{
    if (allCourses.empty())
        return allCourses.begin();
    vector<Course>::iterator begin = allCourses.begin(),
            end = allCourses.end(),
            curr = begin + (end - begin) / 2;

    while(begin < end)
    {
        if (*curr < id)
            begin = curr + 1;
        else if (*curr > id)
            end = curr - 1;
        else
            return curr;
        curr = begin + (end - begin) / 2;
    }
    if (begin < allCourses.end() && *begin < id)
        return begin + 1;
    return begin;
}
/*
uint64_t ScheduleManager::calcuateTheoreticalCount()
{
    uint64_t count = 1;
    for (vector<Course>::iterator it = allCourses.begin(); it < allCourses.end(); it++)
        count *= it->getInstances().size();
    return count;
}
*/

//Return true if Course was added, false if updated.
bool ScheduleManager::addCourse(Course &newCourse)
{
    vector<Course>::iterator coursePos = binarySearchCourse(newCourse.getId());
    if (getCourseCount() > 0 && *coursePos == newCourse)
    {
        *coursePos = newCourse; // If the same code, update.
        return false;
    }
    allCourses.insert(coursePos, newCourse);
    return true;
}

Course *ScheduleManager::getCourseById(int32_t id)
{
    vector<Course>::iterator it = binarySearchCourse(id);
    if (it < allCourses.end() && it->getId() == id)
        return &(*it);
    return nullptr;
}

void ScheduleManager::removeCourse(int32_t id)
{
    vector<Course>::iterator it = binarySearchCourse(id);
    if (it->getId() == id)
        allCourses.erase(it);
}

bool ScheduleManager::addCourseInstance(uint64_t id, string lecturer)
{
    int32_t courseId = COURSE_FROM_INSTANCE_ID(id);
    Course *course = getCourseById(courseId);
    if (course)
    {
        CourseInstance instance(course, id);
        instance.setLecturer(lecturer);
        course->addInstance(instance);
        return true;
    }
    return false;
}

void ScheduleManager::removeCourseInstance(uint64_t id)
{
    int32_t courseId = COURSE_FROM_INSTANCE_ID(id);
    Course *course = getCourseById(courseId);
    if (course)
        course->removeInstance(id);
}

void ScheduleManager::calculate()
{
    allSchedules.clear();

    // Calculate number of options for schedule.

    vector<size_t> coursesInstanceCount(0);
    vector<size_t> activeCoursesIndices;
    size_t theoreticalOptionCount = 1;

    for (size_t i = 0; i < allCourses.size(); i++)
    {
        if (allCourses[i].getActive())
        {
            activeCoursesIndices.push_back(i);
            coursesInstanceCount.push_back(allCourses[i].getInstances().size());
            theoreticalOptionCount *= coursesInstanceCount[i];
        }
    }

    // Loop over all options and see which ones are possible.

    for (size_t i = 0; i < theoreticalOptionCount; i++)
    {
        Schedule currSchedule;
        vector<size_t> options(coursesInstanceCount.size()); // Holds the indices for the instance of each course.
        size_t currentMul = 1; // Holds the multiplication of the courses instance count so far.
        bool isThereContradictions = false;
        for (size_t j = 0; j < coursesInstanceCount.size(); j++)
        {
            options[j] = (i / currentMul) % coursesInstanceCount[j];
            currentMul *= coursesInstanceCount[j];
            const CourseInstance *currInstance = &(allCourses[activeCoursesIndices[j]].getInstances()[options[j]]);
            if (currInstance->getActive() && currSchedule.constradictions(currInstance).empty())
                currSchedule.addCourseInstance(currInstance);
            else
            {
                isThereContradictions = true;
                break;
            }
        }
        if (!isThereContradictions)
            allSchedules.push_back(currSchedule);
    }
}

Schedule *ScheduleManager::getScheduleByIndex(size_t index)
{
    if (index < allSchedules.size())
        return &allSchedules[index];
    return nullptr;
}

size_t ScheduleManager::getScheduleCount()
{
    return allSchedules.size();
}

size_t ScheduleManager::getCourseCount()
{
    return allCourses.size();
}

Course *ScheduleManager::getCourseByIndex(size_t index)
{
    if (index < getCourseCount())
        return &allCourses[index];
    return nullptr;
}

size_t ScheduleManager::getCourseIndexById(int32_t id)
{
    return getCourseById(id) - allCourses.data();
}

CourseInstance *ScheduleManager::getCourseInstanceById(uint64_t id)
{
    Course *course = getCourseById(COURSE_FROM_INSTANCE_ID(id));
    if (course)
        return course->getInstanceById(id);
    return nullptr;
}

const vector<Course> *ScheduleManager::getCourseVec()
{
    return &allCourses;
}
