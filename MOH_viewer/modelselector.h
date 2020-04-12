#ifndef MODELSELECTOR_H
#define MODELSELECTOR_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include "MOH_viewer/moh_viewer.h"

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

    explicit ModelSelector(QWidget *parent = nullptr);
    ~ModelSelector();

private:
    Ui::ModelSelector *ui;

    void paintEvent(QPaintEvent *event);

private slots:
    void on_model_confirm_clicked();
    void on_model_cancel_clicked();
};

#endif // MODELSELECTOR_H
