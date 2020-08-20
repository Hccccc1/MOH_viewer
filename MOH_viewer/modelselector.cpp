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

    main_windows.clear();
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

    current_model = MPT5;

    QString str_model = ui->model_combobox->currentText();

    if (str_model == "MPT5")
    {
        current_model = MPT5;
    }

//    MOH_viewer *w = new MOH_viewer(nullptr, current_model, current_account, current_trans);
//    w->show();


    DeviceLog *m_device_log_widget = new DeviceLog(this, current_model);
    MOH_Array *element = (MOH_Array* )malloc(sizeof (MOH_Array));

    m_modbus = new ModbusSerial(this, m_device_log_widget);

    element->slave_addr = 0;
    element->moh_viewer = new MOH_viewer(nullptr, current_model, current_account, current_trans, m_modbus, m_device_log_widget);

    main_windows.push_back(element);

//    main_windows[0].moh_viewer->set_current_modbus(m_modbus);

    main_windows[widget_counter]->moh_viewer->show();

    connect(main_windows[widget_counter]->moh_viewer, &MOH_viewer::change_slave_addr, this, [=] (int SlaveAddr) {
        for (auto tmp : main_windows)
        {
            if (tmp->moh_viewer == sender())
            {
                tmp->moh_viewer->change_log_slave_addr(SlaveAddr);
                tmp->slave_addr = SlaveAddr;
            }
        }
    });

    connect(main_windows[widget_counter]->moh_viewer->sys_setting, &SystemSetting::new_widget_needed, this, &ModelSelector::get_new_widget);

    widget_counter++;

    this->close();

//    deleteLater();
}

void ModelSelector::on_model_cancel_clicked()
{
    this->deleteLater();
}

void ModelSelector::get_new_widget(int slave_addr)
{
    DeviceLog *m_device_log_widget = new DeviceLog(this, current_model, slave_addr);

    MOH_Array *element = (MOH_Array* )malloc(sizeof (MOH_Array));

    element->slave_addr = slave_addr;
    element->moh_viewer = new MOH_viewer(nullptr, current_model, current_account, current_trans, m_modbus, m_device_log_widget);

    main_windows.push_back(element);

    element->moh_viewer->show();

    if (m_modbus->modbus_client->state() == QModbusDevice::ConnectedState)
        emit element->moh_viewer->sys_setting->serial_connected();

//    connect(main_windows[widget_counter]->moh_viewer, &MOH_viewer);
}
