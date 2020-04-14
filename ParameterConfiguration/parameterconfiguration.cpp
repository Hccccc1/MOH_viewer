#include "parameterconfiguration.h"
#include "ui_parameterconfiguration.h"

ParameterConfiguration::ParameterConfiguration(QWidget *parent, uint8_t model) :
    QWidget(parent),
    ui(new Ui::ParameterConfiguration)
{
    ui->setupUi(this);

    current_model = model;

//    ui->label->setText(QString::number(model));
}

ParameterConfiguration::~ParameterConfiguration()
{
    delete ui;
}
