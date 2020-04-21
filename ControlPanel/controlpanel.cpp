#include "controlpanel.h"
#include "ui_controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent, uint8_t model) :
    QWidget(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);

    current_model = model;

    ui->roundProcessBar->setFormat("%p");
    ui->roundProcessBar->setDecimals(0);
//    connectToSlider(ui->roundProcessBar);
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

//void ControlPanel::connectToSlider(QRoundProgressBar *bar)
//{

//}
