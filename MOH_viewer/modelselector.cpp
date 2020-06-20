#include "modelselector.h"
#include "ui_modelselector.h"

ModelSelector::ModelSelector(QWidget *parent, Accounts account, QTranslator *trans) :
    QWidget(parent),
    ui(new Ui::ModelSelector),
    current_account(account),
    current_trans(trans)
{
    ui->setupUi(this);

    ui->model_combobox->addItem("MPT5");
}

ModelSelector::~ModelSelector()
{
    delete ui;
}

void ModelSelector::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void ModelSelector::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

void ModelSelector::on_model_confirm_clicked()
{
//    qDebug() << __LINE__ << ui->model_combobox->currentText();

    uint8_t current_model = MPT5;

    QString str_model = ui->model_combobox->currentText();

    if (str_model == "MPT5")
    {
        current_model = MPT5;
    }

    MOH_viewer *w = new MOH_viewer(nullptr, current_model, current_account, current_trans);
    w->show();

    this->close();

    deleteLater();
}

void ModelSelector::on_model_cancel_clicked()
{
    this->deleteLater();
}


