#ifndef SCHEDULEFILEHANDLER_H
#define SCHEDULEFILEHANDLER_H

#include <QFile>
#include "schedulemanager.h"

class ScheduleFileHandler
{
public:
    ScheduleFileHandler(QString filename);
    ~ScheduleFileHandler();

    const QDataStream::Version streamVersion = QDataStream::Version::Qt_5_6;

    bool openForWrite();
    bool writeCourseVec(const vector<Course> *vec);

    bool openForRead();
    bool readScheduleManager(ScheduleManager *manager);

    void close();

private:
    QFile file;
    QDataStream *stream = nullptr;
    quint32 appVersion;

    bool writeTimeSlot(const CourseInstanceTimeSlot *timeslot);
    bool writeInstance(const CourseInstance *instance);
    bool writeCourse(const Course *course);

    bool readTimeSlot(CourseInstanceTimeSlot *timeslot);
    bool readInstance(CourseInstance *instance);
    bool readCourse(Course *course);
};

#endif // SCHEDULEFILEHANDLER_H
