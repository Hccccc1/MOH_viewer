#ifndef RTVALUES_H
#define RTVALUES_H

#include <QDialog>

namespace Ui {
class RTValues;
}

class RTValues : public QDialog
{
    Q_OBJECT

public:
    explicit RTValues(QWidget *parent = nullptr);
    ~RTValues();

private:
    Ui::RTValues *ui;
};

#endif // RTVALUES_H
