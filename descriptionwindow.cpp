#include "descriptionwindow.h"
#include "ui_descriptionwindow.h"
#include <QCalendarWidget>
#include <QMessageBox>

DescriptionWindow::DescriptionWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DescriptionWindow)
{
    ui->setupUi(this);

    ui->description->setStyleSheet("font-size: 50px;color:white");
    ui->tasktext->setPlaceholderText("Введите содержание задачи");
    ui->tasktext->setStyleSheet("font-size: 20px");
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
    ui->save->setStyleSheet(buttonStyle);
    ui->dateButton->setStyleSheet(buttonStyle);
}

DescriptionWindow::~DescriptionWindow()
{
    delete ui;
}

void DescriptionWindow::on_exit_clicked()
{
    delete this;
}

void DescriptionWindow::on_save_clicked()
{
    QString taskText = ui->tasktext->toPlainText();
    emit textSaved(taskText, selectedDate);  // Отправка сигнала с текстом и датой
    delete this;  // Закрытие окна после сохранения
}

void DescriptionWindow::on_dateButton_clicked()
{
    // Открываем календарь для выбора даты
    QCalendarWidget *calendar = new QCalendarWidget(this);
    calendar->setWindowFlags(Qt::Window);  // Открываем календарь как отдельное окно
    calendar->setSelectedDate(selectedDate);

    connect(calendar, &QCalendarWidget::activated, this, [=](const QDate &date) {
        selectedDate = date;
        QMessageBox::information(this, "Выбрана дата", "Дата привязки: " + selectedDate.toString("dd.MM.yyyy"));
        calendar->close();
        calendar->deleteLater();
    });

    calendar->show();
}
