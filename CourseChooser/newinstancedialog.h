#ifndef NEWINSTANCEDIALOG_H
#define NEWINSTANCEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "schedulemanager.h"
#include "courseinstance.h"

namespace Ui {
class NewInstanceDialog;
}

class NewInstanceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewInstanceDialog(QWidget *parent = nullptr, CourseInstance *instance = nullptr);
    ~NewInstanceDialog();

    QString getTimeTableCell(int day, int hour);
    QString getLecturer();
    QString getId();

private:
    Ui::NewInstanceDialog *ui;
    array<array<QLineEdit, HOURS_IN_DAY>, DAYS_IN_WEEK> lineEdits;
};

#endif // NEWINSTANCEDIALOG_H
