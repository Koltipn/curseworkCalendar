#ifndef DESCRIPTIONWINDOW_H
#define DESCRIPTIONWINDOW_H

#include <QMainWindow>
#include <QDate>

namespace Ui {
class DescriptionWindow;
}

class DescriptionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DescriptionWindow(QWidget *parent = nullptr);
    ~DescriptionWindow();

private slots:
    void on_exit_clicked();
    void on_save_clicked();
    void on_dateButton_clicked();

signals:
    void textSaved(const QString &text, const QDate &date);  // Сигнал для передачи текста

private:
    Ui::DescriptionWindow *ui;
    QDate selectedDate;
};

#endif // DESCRIPTIONWINDOW_H
