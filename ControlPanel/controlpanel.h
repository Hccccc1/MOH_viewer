#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>

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
    Ui::ControlPanel *ui;

    uint8_t current_model;
};

#endif // CONTROLPANEL_H
