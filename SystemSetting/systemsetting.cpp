#include "systemsetting.h"
#include "ui_systemsetting.h"

SystemSetting::SystemSetting(QWidget *parent, uint8_t model) :
    QWidget(parent),
    ui(new Ui::SystemSetting)
{
    ui->setupUi(this);

    current_model = model;

    ui->label->setText(QString::number(model));
}

SystemSetting::~SystemSetting()
{
    delete ui;
}
