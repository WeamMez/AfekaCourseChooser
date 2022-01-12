#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    courseTableInit();
    schedulesTabInit();
    iconInit();
}

void MainWindow::iconInit()
{
    ui->saveButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->openButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
}

void MainWindow::courseTableInit()
{
    ui->courseTableWidget->setColumnCount(COLS_IN_COURSE_TABLE);
    ui->courseTableWidget->setHorizontalHeaderLabels(COLS_NAMES_IN_COURSE_TABLE);
    ui->courseTableWidget->setRowCount(0);
    ui->courseTableWidget->resizeColumnToContents(CourseTableCols::COURSE_ENABLED);
    ui->courseTableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
}

/*
void MainWindow::tableInit()
{
    courseTableModel.setParent(this);
    courseTableModel.setRowCount(HOURS_IN_DAY);
    courseTableModel.setColumnCount(DAYS_IN_WEEK);

    for (int i = 0; i < HOURS_IN_DAY; i++)
    {
        QString rowName = QString("%1:00").arg(i + DAY_FROM_HOUR);
        courseTableModel.setHeaderData(i, Qt::Orientation::Vertical, rowName);
    }
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        QString colName = QString("יום %1").arg(QString::fromStdString(DAYS_NAMES[i]));
        courseTableModel.setHeaderData(i, Qt::Orientation::Horizontal, colName);
    }
}
*/
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTable()
{
    ui->courseTableWidget->setRowCount(0);
    courseCheckBoxes.clear();
    instanceCheckBoxes.clear();
    //ui->courseTableWidget->setRowCount(scheduleManager.getCourseCount());
    for (size_t i = 0; i < scheduleManager.getCourseCount(); i++)
    {
        Course *currCourse = scheduleManager.getCourseByIndex(i);
        QCheckBox *checkbox = new QCheckBox(ui->courseTableWidget);
        checkbox->setCheckState(currCourse->getActive()? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

        QObject::connect(checkbox, SIGNAL(clicked(bool)), this, SLOT(on_checkbox_clicked(void)));
        courseCheckBoxes.append(checkbox);
        instanceCheckBoxes.append(QVector<QCheckBox *>(0));

        int rowIndex = ui->courseTableWidget->rowCount();
        ui->courseTableWidget->insertRow(rowIndex);
        ui->courseTableWidget->setItem(rowIndex, CourseTableCols::TYPE, new QTableWidgetItem(QString(COURSE_TYPE_STRING)));
        ui->courseTableWidget->setCellWidget(rowIndex, CourseTableCols::COURSE_ENABLED, checkbox);
        ui->courseTableWidget->setItem(rowIndex, CourseTableCols::COURSE_ID, new QTableWidgetItem(QString::number(currCourse->getId())));
        ui->courseTableWidget->setItem(rowIndex, CourseTableCols::COURSE_NAME, new QTableWidgetItem(QString::fromStdString(currCourse->getName())));

        // Instances
        for (size_t j = 0; j < currCourse->getInstanceCount(); j++)
        {
            CourseInstance *currInstance = currCourse->getInstanceByIndex(j);
            QCheckBox *instanceCheckBox = new QCheckBox(ui->courseTableWidget);
            instanceCheckBox->setCheckState(currInstance->getActive()? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

            QObject::connect(instanceCheckBox, SIGNAL(clicked(bool)), this, SLOT(on_checkbox_clicked(void)));
            instanceCheckBoxes[i].append(instanceCheckBox);

            rowIndex = ui->courseTableWidget->rowCount();
            ui->courseTableWidget->insertRow(rowIndex);
            ui->courseTableWidget->setItem(rowIndex, CourseTableCols::TYPE, new QTableWidgetItem(QString(INSTANCE_TYPE_STRING)));
            ui->courseTableWidget->setCellWidget(rowIndex, CourseTableCols::COURSE_ENABLED, instanceCheckBox);
            ui->courseTableWidget->setItem(rowIndex, CourseTableCols::COURSE_ID, new QTableWidgetItem(QString::number(currInstance->getId())));
            ui->courseTableWidget->setItem(rowIndex, CourseTableCols::COURSE_NAME, new QTableWidgetItem(QString::fromStdString(currInstance->getLecturer())));

        }
    }

}

void MainWindow::on_addCourseButton_clicked()
{
    NewCourseDialog newCourseDialog(this);
    int choice = newCourseDialog.exec();
    if (choice == QDialog::Accepted)
    {
        string newCourseName = newCourseDialog.getCourseName().toStdString();
        uint32_t newCourseId = newCourseDialog.getCourseId().toUInt();
        if (newCourseId >= COURSE_END_ID)
        {
            errorMessage("מספר קורס שגוי.");
            return;
        }
        Course newCourse(newCourseName, newCourseId);
        scheduleManager.addCourse(newCourse);
        updateTable();
    }
}


void MainWindow::on_removeCourseButton_clicked()
{
    QList<QTableWidgetItem *> selected = ui->courseTableWidget->selectedItems();
    for (int i = 0; i < selected.size(); i++)
    {
        if (selected[i]->column() == CourseTableCols::COURSE_ID)
        {
            uint64_t id = selected[i]->text().toLong();
            if (id < COURSE_END_ID)
                scheduleManager.removeCourse(id);
            else
                scheduleManager.removeCourseInstance(id);
        }
    }
    updateTable();
}


void MainWindow::setInstance(NewInstanceDialog& newInstanceDialog, bool addNew)
{
    uint64_t id = newInstanceDialog.getId().toULong();
    if (addNew && !scheduleManager.addCourseInstance(id, newInstanceDialog.getLecturer().toStdString()))
    {
        errorMessage("מספר הקורס אינו תואם לאף אחד מהקורסים הקיימים.");
        return;
    }
    CourseInstance *instance = scheduleManager.getCourseInstanceById(id);
    for (int i = FIRST_DAY_IN_WEEK; i <= LAST_DAY_IN_WEEK; i++)
    {
        CourseInstanceTimeSlot *thisTimeSlot = nullptr;
        for (int j = DAY_FROM_HOUR; j < DAY_UNTIL_HOUR; j++)
        {
            QString room = newInstanceDialog.getTimeTableCell(i, j);
            if (!room.isEmpty())
            {
                if (thisTimeSlot && thisTimeSlot->getToHourInDay() == j && thisTimeSlot->getClassInBuildings() == room.toStdString())
                    thisTimeSlot->setToHourInDay(j + 1);
                else
                {
                    if (thisTimeSlot)
                    {
                        instance->addTimeSlot(*thisTimeSlot);
                        delete thisTimeSlot;
                    }
                    thisTimeSlot = new CourseInstanceTimeSlot(instance, i, j, j+1, room.toStdString());

                }
            }
        }
        if (thisTimeSlot)
        {
            instance->addTimeSlot(*thisTimeSlot);
            delete thisTimeSlot;
        }
    }
}

void MainWindow::on_addInstanceButton_clicked()
{
    NewInstanceDialog newInstanceDialog(this);
    int choice = newInstanceDialog.exec();
    if (choice == QDialog::Accepted)
    {
        setInstance(newInstanceDialog);
        updateTable();
    }
}


void MainWindow::errorMessage(QString str)
{
    QMessageBox errmsg(this);
    errmsg.critical(this, "שגיאה", str);
    errmsg.show();
}

void MainWindow::on_editCourseButton_clicked()
{
    QList<QTableWidgetItem *> selected = ui->courseTableWidget->selectedItems();
    if (selected.size() > COLS_IN_COURSE_TABLE)
    {
        errorMessage("לא ניתן לערוך מספר אובייקטים ביחד!");
        return;
    }
    if (selected.isEmpty())
    {
        errorMessage("נא בחר אובייקט לעריכה.");
        return;
    }

    ulong idToEdit = 0;
    int i;
    for (i = 0; i < selected.size(); i++)
        if (selected[i]->column() == CourseTableCols::COURSE_ID)
        {
            idToEdit = selected[i]->text().toULong();
            break;
        }
    if (i == selected.size())
    {
        errorMessage("התרחשה שגיאה לא ידועה.");
        return;
    }

    if (idToEdit < COURSE_END_ID)
    {
        // This is a course
        Course *course = scheduleManager.getCourseById(idToEdit);
        NewCourseDialog newCourseDialog(this, course);
        int choice = newCourseDialog.exec();
        if (choice == QDialog::Accepted)
        {
            course->setName(newCourseDialog.getCourseName().toStdString());
        }
    }
    else
    {
        // This is an instance
        CourseInstance *instance = scheduleManager.getCourseInstanceById(idToEdit);
        NewInstanceDialog newInstanceDialog(this, instance);
        int choice = newInstanceDialog.exec();
        if (choice == QDialog::Accepted)
        {
            setInstance(newInstanceDialog, false);
        }
    }

    updateTable();
}

void MainWindow::schedulesTabInit()
{
    ui->schedulesTabWidget->setTabsClosable(false);
    while (0 < ui->schedulesTabWidget->count())
    {
        delete ui->schedulesTabWidget->widget(0);
        ui->schedulesTabWidget->removeTab(0);
    }
}

QTableWidget *MainWindow::tableFromSchedule(Schedule *s, QWidget *parent)
{
    QTableWidget *table = new QTableWidget(HOURS_IN_DAY, DAYS_IN_WEEK, parent);

    QStringList allDays;
    for (int i = 0; i < DAYS_IN_WEEK; i++)
    {
        allDays.append(DAY_STR + QString::fromStdString(DAYS_NAMES[i]));
    }
    table->setHorizontalHeaderLabels(allDays);

    QStringList allHours;
    for (int i = DAY_FROM_HOUR; i < DAY_UNTIL_HOUR; i++)
    {
        allHours.append(QString::number(i) + ":00");
    }
    table->setVerticalHeaderLabels(allHours);

    for (int i = FIRST_DAY_IN_WEEK; i <= LAST_DAY_IN_WEEK; i++)
    {
        for (int j = DAY_FROM_HOUR; j < DAY_UNTIL_HOUR; j++)
        {
            const CourseInstanceTimeSlot *timeslot = s->getTimeSlot(i, j);
            if (timeslot)
            {
               QString str = QString::number(timeslot->getInstance()->getId()) + "\n"
                    + QString::fromStdString(timeslot->getInstance()->getCourse()->getName()) + "\n"
                    + QString::fromStdString(timeslot->getClassInBuildings());
               table->setItem(j - DAY_FROM_HOUR, i - FIRST_DAY_IN_WEEK, new QTableWidgetItem(str));
            }
        }
    }

    table->resizeRowsToContents();

    return table;
}

void MainWindow::updateSchedules()
{
    schedulesTabInit();
    int scheduleCount = scheduleManager.getScheduleCount();
    for (int i = 0; i < scheduleCount; i++)
    {
        ui->schedulesTabWidget->addTab(tableFromSchedule(scheduleManager.getScheduleByIndex(i), ui->schedulesTabWidget), QString("מערכת ")+ QString::number(i + 1));
    }
}

void MainWindow::on_calculateButton_clicked()
{
    scheduleManager.calculate();
    updateSchedules();
}


void MainWindow::on_saveButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "שמור רשימת קורסים", "", "Data File (*.dat);;All Files(*)");
    if (filename.isEmpty())
        return;
    ScheduleFileHandler fh(filename);
    if (!fh.openForWrite())
    {
        errorMessage("אין אפשרות לשמור את הקובץ.");
        return;
    }

    if (!fh.writeCourseVec(scheduleManager.getCourseVec()))
        errorMessage("כתיבה לקובץ נכשלה.");
}


void MainWindow::on_openButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "פתח רשימת קורסים", "", "Data File (*.dat);;All Files(*)");
    if (filename.isEmpty())
        return;
    ScheduleFileHandler fh(filename);
    if (!fh.openForRead())
    {
        errorMessage("אין אפשרות לפתוח את הקובץ.");
        return;
    }
    if (!fh.readScheduleManager(&scheduleManager))
        errorMessage("קריאה מהקובץ נכשלה.");

    updateTable();
}

void MainWindow::on_checkbox_clicked()
{
    for (size_t i = 0; i < scheduleManager.getCourseCount(); i++)
    {
        Course *course = scheduleManager.getCourseByIndex(i);
        course->setActive(courseCheckBoxes[i]->isChecked());
        for (size_t j = 0; j < course->getInstanceCount(); j++)
            course->getInstanceByIndex(j)->setActive(instanceCheckBoxes[i][j]->isChecked());
    }

    updateTable();
}
