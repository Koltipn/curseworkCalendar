#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QPixmap>
#include "basewindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Устанавливаем фон через QSS для всего окна
    this->setStyleSheet("QMainWindow { background-image: url(:/img/images/background_start.jpg); background-position: center; background-repeat: no-repeat; }");
    ui->labelWelcome->setStyleSheet("color: black; font-size: 40px; font-weight: bold; text-align: center;");
    ui->labelChoose->setStyleSheet("color: black; font-size: 36px; font-style: bold; text-align: center;");


    // Стили для кнопок
    QString buttonStyle = "QPushButton {"
                          "color: black;"
                          "font-size: 16px;"
                          "background-color: #E0E0E0;"
                          "border-radius: 10px;"
                          "background-size: cover;"
                          "border : none"
                          "padding: 10px 20px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #D3D3D3;"
                          "}"
                          "QPushButton:pressed {"
                          "background-color: #C0C0C0;"
                          "}";

    // Применяем стиль к кнопкам
    ui->amateurButton->setStyleSheet(buttonStyle);
    ui->professionalButton->setStyleSheet(buttonStyle);

    // Подключаем кнопки к слотам
    connect(ui->amateurButton, &QPushButton::clicked, this, &MainWindow::on_amateurButton_clicked);
    connect(ui->professionalButton, &QPushButton::clicked, this, &MainWindow::on_professionalButton_clicked);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveUserChoice(const QString &choice)
{
    QSettings settings("YourCompany", "CourseWork");
    settings.setValue("userMode", choice);
}

void MainWindow::openBaseWindow()
{
    // Проверяем, если baseWindow уже открыт, не создаем новое окно
    if (!baseWindow) {
        baseWindow = new BaseWindow;
        baseWindow->setAttribute(Qt::WA_DeleteOnClose);
        baseWindow->show();

        // Закрываем текущее окно
        this->close();

        // Освобождаем указатель, когда окно закрыто
        connect(baseWindow, &QWidget::destroyed, this, [this]() {
            baseWindow = nullptr;
        });
    }
}



void MainWindow::on_amateurButton_clicked()
{
    saveUserChoice("Садовод-любитель");
    openBaseWindow();
}

void MainWindow::on_professionalButton_clicked()
{
    saveUserChoice("Профессионал");
    openBaseWindow();
}
