#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QDebug>
#include <QWidget>
#include <QResizeEvent>
#include <QStyleOption>
#include <QPainter>
#include "QRoundProgressBar/QRoundProgressBar.h"

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr, uint8_t model = 0);
    ~ControlPanel();

private:
    struct Controls
    {
        quint16 speed_ctrl_bl01;
        quint16 speed_ctrl_bl02;
        quint16 speed_ctrl_bl03;
        quint16 speed_ctrl_bl04;
        quint16 speed_ctrl_pmp01;
        quint16 speed_ctrl_pmp02;
        quint16 speed_ctrl_pmp03;
        quint16 speed_ctrl_pmp04;
        quint16 speed_ctrl_pmp05;
    };

    Ui::ControlPanel *ui;

    uint8_t current_model;

//    void connectToSlider(QRoundProgressBar *bar);

};

#endif // CONTROLPANEL_H
