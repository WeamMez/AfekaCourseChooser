#include "schedulefilehandler.h"

ScheduleFileHandler::ScheduleFileHandler(QString filename)
{
    file.setFileName(filename);
}

ScheduleFileHandler::~ScheduleFileHandler()
{
    close();
}

bool ScheduleFileHandler::openForWrite()
{
    if (!file.open(QIODevice::WriteOnly))
        return false;
    stream = new QDataStream(&file);
    stream->setVersion(streamVersion);

    appVersion = 1;
    *stream << appVersion;
    return true;
}

bool ScheduleFileHandler::writeTimeSlot(const CourseInstanceTimeSlot *timeslot)
{
    *stream << (quint8)timeslot->getDayInWeek() << (quint8)timeslot->getFromHourInDay()
            << (quint8)timeslot->getToHourInDay()
            << QString::fromStdString(timeslot->getClassInBuildings());

    return true;
}

bool ScheduleFileHandler::writeInstance(const CourseInstance *instance)
{
    *stream << (quint64)instance->getId() << (qint8)instance->getActive()
            << QString::fromStdString(instance->getLecturer());

    const vector<CourseInstanceTimeSlot> &allTimeSlots = instance->getTimeSlots();
    *stream << (quint8)allTimeSlots.size();

    for (quint8 i = 0; i < allTimeSlots.size(); i++)
        if (!writeTimeSlot(&allTimeSlots[i]))
            return false;

    return true;
}

bool ScheduleFileHandler::writeCourse(const Course *course)
{
    *stream << (qint32)course->getId() << (qint8)course->getActive()
            << QString::fromStdString(course->getName());

    *stream << (quint8)course->getInstanceCount();
    const vector<CourseInstance> &allInstances = course->getInstances();

    for (quint8 i = 0; i < course->getInstanceCount(); i++)
        if (!writeInstance(&allInstances[i]))
            return false;

    return true;
}

bool ScheduleFileHandler::writeCourseVec(const vector<Course> *vec)
{
    if (!file.isOpen() || !file.isWritable() || !stream)
        return false;

    *stream << (quint32)vec->size();

    for (quint32 i = 0; i < vec->size(); i++)
        if (!writeCourse(&(*vec)[i]))
            return false;

    return true;
}

bool ScheduleFileHandler::openForRead()
{
    if (file.isOpen() || stream || !file.open(QIODevice::ReadOnly))
        return false;

    stream = new QDataStream(&file);
    stream->setVersion(streamVersion);

    *stream >> appVersion;
    return true;
}

bool ScheduleFileHandler::readTimeSlot(CourseInstanceTimeSlot *timeslot)
{
    quint8 day, fromHour, toHour;
    QString classInBuilding;

    *stream >> day >> fromHour >> toHour >> classInBuilding;
    timeslot->setDayInWeek(day);
    timeslot->setFromHourInDay(fromHour);
    timeslot->setToHourInDay(toHour);
    timeslot->setClassInBuildings(classInBuilding.toStdString());

    return true;
}

bool ScheduleFileHandler::readInstance(CourseInstance *instance)
{
    quint64 id;
    qint8 active;
    QString lecturer;
    quint8 slotsCount;

    *stream >> id >> active >> lecturer >> slotsCount;

    instance->setId(id);
    instance->setActive(active != 0);
    instance->setLecturer(lecturer.toStdString());

    for (quint8 i = 0; i < slotsCount; i++)
    {
        CourseInstanceTimeSlot timeslot(instance);
        if (!readTimeSlot(&timeslot))
            return false;
        instance->addTimeSlot(timeslot);
    }

    return true;
}

bool ScheduleFileHandler::readCourse(Course *course)
{
    qint32 id;
    qint8 active;
    QString name;
    quint8 instancesCount;

    *stream >> id >> active >> name >> instancesCount;

    course->setId(id);
    course->setActive(active != 0);
    course->setName(name.toStdString());

    for (quint8 i = 0; i < instancesCount; i++)
    {
        CourseInstance instance(course);
        if (!readInstance(&instance))
            return false;
        course->addInstance(instance);
    }

    return true;
}

bool ScheduleFileHandler::readScheduleManager(ScheduleManager *manager)
{
    if (!file.isOpen() || !file.isReadable() || !stream)
        return false;

    quint32 courseCount;
    *stream >> courseCount;
    for(quint32 i = 0; i < courseCount; i++)
    {
        Course course;
        if(!readCourse(&course))
            return false;
        manager->addCourse(course);
    }

    return true;
}

void ScheduleFileHandler::close()
{
    if (stream)
        delete stream;
    stream = nullptr;
    file.close();
}
