#ifndef SYSTEMSETTING_H
#define SYSTEMSETTING_H

#include <QWidget>

namespace Ui {
class SystemSetting;
}

class SystemSetting : public QWidget
{
    Q_OBJECT

public:
    explicit SystemSetting(QWidget *parent = nullptr, uint8_t model = 0);
    ~SystemSetting();

private:
    Ui::SystemSetting *ui;

    uint8_t current_model;
};

#endif // SYSTEMSETTING_H
