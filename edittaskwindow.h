#ifndef EDITTASKWINDOW_H
#define EDITTASKWINDOW_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class EditTaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskWindow(const QString &initialText, QWidget *parent = nullptr);
    QString getEditedText() const;

signals:
    void taskEdited(const QString &newText);

private slots:
    void onSaveClicked();

private:
    QTextEdit *textEdit;
    QPushButton *saveButton;
};

#endif // EDITTASKWINDOW_H
