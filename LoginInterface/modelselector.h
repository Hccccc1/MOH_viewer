#ifndef MODELSELECTOR_H
#define MODELSELECTOR_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
//#include "MOH_Viewer/moh_viewer.h"
#include "AllBitsAndRegs.h"
#include "manager.h"

namespace Ui {
class ModelSelector;
}

//class MOH_Viewer;

class ModelSelector : public QWidget
{
    Q_OBJECT

public:
    enum DeviceType{
        MPT5,
    };

//    struct MOH_Array
//    {
//        int slave_addr;
//        MOH_viewer* moh_viewer = nullptr;
//    };

//    QVector<MOH_Array* > main_windows;

    explicit ModelSelector(QWidget *parent = nullptr, Accounts account = Customer, QTranslator *trans = nullptr);
    ~ModelSelector();

//public slots:
//    void get_new_widget(int);

private:
    Ui::ModelSelector *ui;

    uint8_t current_model = 0;
    Accounts current_account;

    uint8_t widget_counter = 0;

//    ModbusSerial *m_modbus = nullptr;

    QTranslator *current_trans = nullptr;

    Manager *manager = nullptr;

//    MOH_Array element;

private slots:
    void on_model_confirm_clicked();
    void on_model_cancel_clicked();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void changeEvent(QEvent *e);
};

#endif // MODELSELECTOR_H
