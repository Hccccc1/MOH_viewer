#ifndef MODELSELECTOR_H
#define MODELSELECTOR_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include "MOH_viewer/moh_viewer.h"
#include "AllBitsAndRegs.h"

namespace Ui {
class ModelSelector;
}

class ModelSelector : public QWidget
{
    Q_OBJECT

public:
    enum DeviceType{
        MPT5,
    };

    explicit ModelSelector(QWidget *parent = nullptr, Accounts account = Customer);
    ~ModelSelector();

private:
    Ui::ModelSelector *ui;

    void paintEvent(QPaintEvent *event);

    Accounts current_account;

private slots:
    void on_model_confirm_clicked();
    void on_model_cancel_clicked();
};

#endif // MODELSELECTOR_H
