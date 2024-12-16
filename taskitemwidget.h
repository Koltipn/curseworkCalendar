#ifndef TASKITEMWIDGET_H
#define TASKITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class TaskItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskItemWidget(const QString &text, QWidget *parent = nullptr);

signals:
    void removeRequested();  // Сигнал для удаления элемента

private:
    QLabel *taskLabel;
    QPushButton *removeButton;
};

#endif // TASKITEMWIDGET_H
