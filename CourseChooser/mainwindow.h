#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QMessageBox>
#include <QFileDialog>

#include "newcoursedialog.h"
#include "newinstancedialog.h"
#include "schedulefilehandler.h"

#include "schedulemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define COLS_IN_COURSE_TABLE 4
enum CourseTableCols {TYPE, COURSE_ENABLED, COURSE_ID, COURSE_NAME};
const QStringList COLS_NAMES_IN_COURSE_TABLE = {"סוג", "מאופשר", "מספר", "שם"};

const QString COURSE_TYPE_STRING = "קורס";
const QString INSTANCE_TYPE_STRING = "מופע";

const QString DAY_STR = "יום ";

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addCourseButton_clicked();

    void on_removeCourseButton_clicked();

    void on_addInstanceButton_clicked();

    void on_editCourseButton_clicked();

    void on_calculateButton_clicked();

    void on_saveButton_clicked();

    void on_openButton_clicked();

    void on_checkbox_clicked();

private:
    Ui::MainWindow *ui;
    ScheduleManager scheduleManager;
    //QStandardItemModel courseTableModel;
    QVector<QCheckBox *> courseCheckBoxes;
    QVector<QVector<QCheckBox *>> instanceCheckBoxes;

    void iconInit();
    void courseTableInit();
    void updateTable();
    void setInstance(NewInstanceDialog& newInstanceDialog, bool addNew = true);
    void errorMessage(QString str);
    void schedulesTabInit();
    QTableWidget *tableFromSchedule(Schedule *s, QWidget *parent = nullptr);
    void updateSchedules();
};
#endif // MAINWINDOW_H
