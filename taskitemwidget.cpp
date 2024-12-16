#include "taskitemwidget.h"

TaskItemWidget::TaskItemWidget(const QString &text, QWidget *parent)
    : QWidget(parent)
{
    taskLabel = new QLabel(text, this);
    taskLabel->setStyleSheet("font-size: 24px; color: black;");

    removeButton = new QPushButton("Удалить", this);
    removeButton->setStyleSheet("font-size: 18px; background-color: red; color: white;");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(taskLabel);
    layout->addWidget(removeButton);
    layout->setContentsMargins(0, 0, 0, 0);

    connect(removeButton, &QPushButton::clicked, this, &TaskItemWidget::removeRequested);
}
