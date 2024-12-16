#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <QMainWindow>

namespace Ui {
class Description;
}

class Description : public QMainWindow
{
    Q_OBJECT

public:
    explicit Description(QWidget *parent = nullptr);
    ~Description();

private:
    Ui::Description *ui;
};

#endif // DESCRIPTION_H
