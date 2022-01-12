#include "newinstancedialog.h"
#include "ui_newinstancedialog.h"

NewInstanceDialog::NewInstanceDialog(QWidget *parent, CourseInstance *instance) :
    QDialog(parent),
    ui(new Ui::NewInstanceDialog)
{
    ui->setupUi(this);
    for (int i = 0; i < ui->timeTableWidget->columnCount(); i++)
    {
        for (int j = 0; j < ui->timeTableWidget->rowCount(); j++)
        {
            ui->timeTableWidget->setCellWidget(j, i, &lineEdits[i][j]);
        }
    }
    if (instance)
    {
        ui->instanceIdLineEdit->setEnabled(false);
        ui->instanceIdLineEdit->setText(QString::number(instance->getId()));
        ui->instanceLecturerLineEdit->setText(QString::fromStdString(instance->getLecturer()));
        const vector<CourseInstanceTimeSlot>& timeslots = instance->getTimeSlots();
        for (size_t i = 0; i < timeslots.size(); i++)
        {
            size_t arrDay = timeslots[i].getDayInWeek() - FIRST_DAY_IN_WEEK;
            size_t arrFromHour = timeslots[i].getFromHourInDay() - DAY_FROM_HOUR;
            size_t arrToHour = timeslots[i].getToHourInDay() - DAY_FROM_HOUR;

            for (size_t j = arrFromHour; j < arrToHour; j++)
                lineEdits[arrDay][j].setText(QString::fromStdString(timeslots[i].getClassInBuildings()));
        }
    }
}

NewInstanceDialog::~NewInstanceDialog()
{
    delete ui;
}

QString NewInstanceDialog::getTimeTableCell(int day, int hour)
{
    return lineEdits[day - FIRST_DAY_IN_WEEK][hour - DAY_FROM_HOUR].text();
}

QString NewInstanceDialog::getLecturer()
{
    return ui->instanceLecturerLineEdit->text();
}

QString NewInstanceDialog::getId()
{
    return ui->instanceIdLineEdit->text();
}
