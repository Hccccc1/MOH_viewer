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
    connectToSlider(ui->roundProcessBar);
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::connectToSlider(QRoundProgressBar *bar)
{
//    bar->setRange(ui->Slider->minimum(), ui->Slider->maximum());
//    bar->setValue(ui->Slider->value());
//    connect(ui->Slider, SIGNAL(valueChanged(int)), bar, SLOT(setValue(int)));
}
