#ifndef NEWCOURSEDIALOG_H
#define NEWCOURSEDIALOG_H

#include <QDialog>
#include <QIntValidator>
#include "course.h"

namespace Ui {
class NewCourseDialog;
}

class NewCourseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCourseDialog(QWidget *parent = nullptr, Course *course = nullptr);
    ~NewCourseDialog();

    QString getCourseName();
    QString getCourseId();

private:
    Ui::NewCourseDialog *ui;
    QIntValidator *courseIdValidator;
};

#endif // NEWCOURSEDIALOG_H
