// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

// Предварительное объявление класса BaseWindow
class BaseWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_amateurButton_clicked();
    void on_professionalButton_clicked();

private:
    Ui::MainWindow *ui;
    void saveUserChoice(const QString &choice);
    void openBaseWindow();

    BaseWindow *baseWindow = nullptr;  // Указатель на BaseWindow
};

#endif // MAINWINDOW_H
