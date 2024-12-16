#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QDialog>
#include "taskswindow.h"
#include <QDate>
#include <QMap>

namespace Ui {
class BaseWindow;
}

class TasksWindow;

class BaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BaseWindow(QWidget *parent = nullptr);
    ~BaseWindow();
    void addNoteForDate(const QDate &date, const QString &note); // Метод для добавления заметки
    void removeNoteForDate(const QDate &date); // Новый метод для удаления заметки

private slots:
    void on_exit_clicked();
    void on_tasks_clicked();
    void on_calendar_clicked();  // Новый слот для кнопки календаря
    void updateNoteForDate(const QDate &date, const QString &newNote); // Новый слот для обновления заметки


    void on_change_clicked();

private:
    Ui::BaseWindow *ui;
    TasksWindow *tasksWindow;
    QMap<QDate, QString> notes; // Хранение заметок с привязкой к дате
    void loadNotes();  // Новый метод для загрузки заметок из файла

};

#endif // BASEWINDOW_H
