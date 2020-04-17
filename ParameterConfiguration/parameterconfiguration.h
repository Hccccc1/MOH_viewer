#ifndef PARAMETERCONFIGURATION_H
#define PARAMETERCONFIGURATION_H

#include <QDebug>
#include <QWidget>

namespace Ui {
class ParameterConfiguration;
}

class ParameterConfiguration : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterConfiguration(QWidget *parent = nullptr, uint8_t model = 0);
    ~ParameterConfiguration();

private:
    Ui::ParameterConfiguration *ui;

    uint8_t current_model;
};

#endif // PARAMETERCONFIGURATION_H
