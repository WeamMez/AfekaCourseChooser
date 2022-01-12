#include "course.h"

Course::Course(string name, int32_t id, bool active):
    name(name), id(id), active(active)
{

}

Course::Course(Course&& other)
{
    *this = other;

    other.id = 0;
    other.name = "";
    other.active = true;

    other.instances.clear();
}

Course::Course(const Course& other)
{
    *this = other;
}

Course::Course():
    Course("", 0)
{

}

const string &Course::getName() const
{
    return name;
}

void Course::setName(const string &newName)
{
    name = newName;
}

int32_t Course::getId() const
{
    return id;
}

void Course::setId(int32_t newId)
{
    id = newId;
}

const vector<CourseInstance> &Course::getInstances() const
{
    return instances;
}

bool Course::getActive() const
{
    return active;
}

void Course::setActive(bool newActive)
{
    active = newActive;
}

size_t Course::getInstanceCount() const
{
    return instances.size();
}

CourseInstance *Course::getInstanceByIndex(size_t index)
{
    if (index < instances.size())
        return &instances[index];
    return nullptr;
}

CourseInstance *Course::getInstanceById(uint64_t id)
{
    for (size_t i = 0; i < instances.size(); i++)
    {
        if (instances[i].getId() == id)
        {
            return &instances[i];
        }
    }
    return nullptr;
}

// Adding an instance of the course: O(n), n = instances.size()
void Course::addInstance(CourseInstance &newInstance)
{
    removeInstance(newInstance.getId());
    newInstance.setCourse(this);
    instances.push_back(newInstance);
}

void Course::removeInstance(uint64_t instanceId)
{
    for (size_t i = 0; i < instances.size(); i++)
        if (instances[i].getId() == instanceId)
        {
            instances.erase(instances.begin() + i);
            break;
        }
}

bool Course::operator<(const Course &other) const
{
    return this->id < other.id;
}

bool Course::operator>(const Course &other) const
{
    return this->id > other.id;
}

bool Course::operator==(const Course &other) const
{
    return this->id == other.id;
}

bool Course::operator<(int32_t id) const
{
    return this->id < id;
}

bool Course::operator>(int32_t id) const
{
    return this->id > id;
}

bool Course::operator==(int32_t id) const
{
    return this->id == id;
}

Course& Course::operator=(const Course &other)
{
    name = other.name;
    id = other.id;
    active = other.active;
    instances = other.instances;

    this->instances.resize(other.instances.size());
    for (size_t i = 0; i < other.instances.size(); i++)
    {
        this->instances[i] = other.instances[i];
        this->instances[i].setCourse(this);
    }

    return *this;
}
