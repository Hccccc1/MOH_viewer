#include "controlpanel.h"
#include "ui_controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent, uint8_t model) :
    QWidget(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);

    current_model = model;

    ui->label->setText(QString::number(model));
}

ControlPanel::~ControlPanel()
{
    delete ui;
}
