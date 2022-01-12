#ifndef COURSE_H
#define COURSE_H

#include "courseinstance.h"

class Course
{
private:
    string name;
    int32_t id;
    bool active;
    vector<CourseInstance> instances;

    //ssize_t getInstanceIndexById(uint64_t instanceId);
public:
    Course(string name, int32_t id, bool active = true);
    Course(Course&& other);
    Course(const Course& other);
    Course();

    const string &getName() const;
    void setName(const string &newName);
    int32_t getId() const;
    void setId(int32_t newId);
    const vector<CourseInstance> &getInstances() const;
    bool getActive() const;
    void setActive(bool newActive);

    size_t getInstanceCount() const;
    CourseInstance *getInstanceByIndex(size_t index);
    CourseInstance *getInstanceById(uint64_t id);

    void addInstance(CourseInstance &newInstance);
    void removeInstance(uint64_t instanceId);

    bool operator<(const Course &other) const;
    bool operator>(const Course &other) const;
    bool operator==(const Course &other) const;
    bool operator<(int32_t id) const;
    bool operator>(int32_t id) const;
    bool operator==(int32_t id) const;

    Course& operator=(const Course& other);
};

#endif // COURSE_H
