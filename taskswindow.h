#ifndef TASKSWINDOW_H
#define TASKSWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "descriptionwindow.h"
#include "taskitemwidget.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class TasksWindow;
}

class TasksWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TasksWindow(QWidget *parent = nullptr);
    ~TasksWindow();

signals:
    void taskRemoved(const QDate &date);  // Сигнал для удаления задачи по дате
    void taskUpdated(const QDate &date, const QString &newText); // Сигнал для обновления задачи


private slots:
    void on_exit_clicked();
    void on_add_clicked();
    void addTask(const QString &text, const QDate &date);
    void removeTask(QListWidgetItem *item);  // Удаление задачи
    void editTask(QListWidgetItem *item);
    void on_taskItem_doubleClicked(QListWidgetItem *item);
    void on_searchTextChanged(const QString &searchText);
    void on_filterChanged(); // Слот для обработки изменения фильтра
    void updateTaskWidgets();

private:
    Ui::TasksWindow *ui;
    DescriptionWindow *descriptionWindow;

    void saveTasks();       // Метод для сохранения задач
    void loadTasks();
};

#endif // TASKSWINDOW_H
