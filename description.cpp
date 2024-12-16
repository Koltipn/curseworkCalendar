#include "Description.h"
#include "ui_Description.h"

Description::Description(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Description)
{
    ui->setupUi(this);
}

Description::~Description()
{
    delete ui;
}
