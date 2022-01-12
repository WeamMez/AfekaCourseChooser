#include "newcoursedialog.h"
#include "ui_newcoursedialog.h"

NewCourseDialog::NewCourseDialog(QWidget *parent, Course *course) :
    QDialog(parent),
    ui(new Ui::NewCourseDialog)
{
    ui->setupUi(this);
    courseIdValidator = new QIntValidator(0, 99999, this);
    ui->courseIdLineEdit->setValidator(courseIdValidator);
    if (course)
    {
        ui->courseIdLineEdit->setEnabled(false);
        ui->courseIdLineEdit->setText(QString::number(course->getId()));
        ui->courseNameLineEdit->setText(QString::fromStdString(course->getName()));
    }
}

NewCourseDialog::~NewCourseDialog()
{
    delete courseIdValidator;
    delete ui;
}

QString NewCourseDialog::getCourseName()
{
    return ui->courseNameLineEdit->text();
}

QString NewCourseDialog::getCourseId()
{
    return ui->courseIdLineEdit->text();
}
