#include "parameterconfiguration.h"
#include "ui_parameterconfiguration.h"

ParameterConfiguration::ParameterConfiguration(QWidget *parent, uint8_t model) :
    QWidget(parent),
    ui(new Ui::ParameterConfiguration)
{
    ui->setupUi(this);

    current_model = model;

//    ui->label->setText(QString::number(model));

    qDebug() << __FILE__ << __LINE__ << ui->label_21->size();
}

ParameterConfiguration::~ParameterConfiguration()
{
    delete ui;
}
