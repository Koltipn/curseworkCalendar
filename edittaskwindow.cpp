#include "edittaskwindow.h"
#include <QVBoxLayout>

EditTaskWindow::EditTaskWindow(const QString &initialText, QWidget *parent)
    : QDialog(parent), textEdit(new QTextEdit(this)), saveButton(new QPushButton("Сохранить", this))
{
    setWindowTitle("Редактировать задачу");
    setFixedSize(800, 600); // Можно настроить размер окна по необходимости

    textEdit->setText(initialText);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(textEdit);
    layout->addWidget(saveButton);

    setLayout(layout);

    connect(saveButton, &QPushButton::clicked, this, &EditTaskWindow::onSaveClicked);
}

QString EditTaskWindow::getEditedText() const
{
    return textEdit->toPlainText();
}

void EditTaskWindow::onSaveClicked()
{
    emit taskEdited(getEditedText());
    accept(); // Закрытие окна после сохранения
}
