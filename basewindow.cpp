#include "basewindow.h"
#include "ui_basewindow.h"
#include "mainwindow.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

BaseWindow::BaseWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BaseWindow)
    , tasksWindow(nullptr)
{
    ui->setupUi(this);
    loadNotes();

    showFullScreen();

    QPixmap pix(":/img/images/2.png");
    QPixmap pix_background(":/img/images/3.png");
    QPixmap pix_tabl1(":/img/images/4.png");
    QPixmap pix_tabl2(":/img/images/5.png");
    int w = 2000;
    ui->shapka->setFixedWidth(w); // Установка ширины виджета shapka
    ui->background->setFixedWidth(w);
    int h = ui->shapka->height();
    int h_background = 1200;
    int h_tabls = 700;
    int w_tabls = 1000;
    ui->background->setFixedHeight(h_background);

    ui->shapka->setPixmap(pix.scaled(w, h, Qt::IgnoreAspectRatio));
    ui->background->setPixmap(pix_background.scaled(w, h_background, Qt::IgnoreAspectRatio));
    ui->tabl1->setPixmap(pix_tabl1.scaled(w_tabls, h_tabls, Qt::KeepAspectRatio));
    ui->tabl2->setPixmap(pix_tabl2.scaled(w_tabls, h_tabls, Qt::KeepAspectRatio));
    // Установка прозрачности для виджета background
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.7); // Задайте желаемый уровень прозрачности (от 0.0 до 1.0)
    ui->background->setGraphicsEffect(opacityEffect);

    //styles
    ui->title->setStyleSheet("font-size: 40px;font-style: italic;");
    ui->label->setStyleSheet("font-size: 40px; color: darkred");
    ui->text_tabl1->setStyleSheet("font-size: 30px; color: red; font-weight: bold");
    ui->text_tabl2->setStyleSheet("font-size: 30px; color: red; font-weight: bold");

    QString buttonStyle = "QPushButton {"
                          "color: white;"
                          "font-size: 24px;"
                          "background-color: darkred;"
                          "border-radius: 10px;"
                          "padding: 10px 20px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #D94343;"
                          "}"
                          "QPushButton:pressed {"
                          "background-color: #D13636;"
                          "}";
    ui->exit->setStyleSheet(buttonStyle);
    ui->change->setStyleSheet(buttonStyle);
    ui->calendar->setStyleSheet(buttonStyle);
    ui->tasks->setStyleSheet(buttonStyle);



}

BaseWindow::~BaseWindow()
{
    delete ui;
}

void BaseWindow::on_exit_clicked()
{
    QApplication::exit();
}

void BaseWindow::on_change_clicked()
{
    this->close();
    MainWindow* mainWin = new MainWindow();
    mainWin->show();
}

void BaseWindow::updateNoteForDate(const QDate &date, const QString &newNote)
{
    if (notes.contains(date)) {
        notes[date] = newNote; // Обновляем заметку для указанной даты
    }
}

void BaseWindow::on_tasks_clicked()
{
    if (!tasksWindow) {
        tasksWindow = new TasksWindow(this);

        connect(tasksWindow, &TasksWindow::taskRemoved, this, &BaseWindow::removeNoteForDate);
        connect(tasksWindow, &TasksWindow::taskUpdated, this, &BaseWindow::updateNoteForDate); // Подключение сигнала обновления

        connect(tasksWindow, &QWidget::destroyed, this, [this]() {
            tasksWindow = nullptr;
        });

        tasksWindow->show();
    } else {
        tasksWindow->raise();
        tasksWindow->activateWindow();
    }
}




void BaseWindow::addNoteForDate(const QDate &date, const QString &note) {
    notes[date] = note;
}

void BaseWindow::on_calendar_clicked() {
    QDialog calendarDialog(this);
    calendarDialog.setWindowTitle("Выбор даты");

    QVBoxLayout *layout = new QVBoxLayout(&calendarDialog);
    QCalendarWidget *calendar = new QCalendarWidget(&calendarDialog);
    layout->addWidget(calendar);

    calendarDialog.setLayout(layout);

    connect(calendar, &QCalendarWidget::clicked, &calendarDialog, &QDialog::accept);

    if (calendarDialog.exec() == QDialog::Accepted) {
        QDate selectedDate = calendar->selectedDate();
        if (notes.contains(selectedDate)) {
            QMessageBox::information(this, "Заметка", notes[selectedDate]);
        } else {
            QMessageBox::information(this, "Заметка", "Заметок на выбранную дату нет.");
        }
    }
}

void BaseWindow::removeNoteForDate(const QDate &date) {
    notes.remove(date);  // Удаляем заметку по указанной дате
}


void BaseWindow::loadNotes() {
    QFile file("tasks.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Could not open file for reading in BaseWindow");
        return;
    }

    QJsonDocument loadDoc(QJsonDocument::fromJson(file.readAll()));
    QJsonArray tasksArray = loadDoc.array();

    for (int i = 0; i < tasksArray.size(); ++i) {
        QJsonObject taskObject = tasksArray[i].toObject();
        QString taskText = taskObject["text"].toString();
        QDate date = QDate::fromString(taskObject["date"].toString(), "dd.MM.yyyy");

        // Добавляем заметку в календарь
        notes[date] = taskText;
    }
    file.close();
}
