#ifndef RTCURVE_H
#define RTCURVE_H

#include <QDialog>

namespace Ui {
class RTCurve;
}

class RTCurve : public QDialog
{
    Q_OBJECT

public:
    explicit RTCurve(QWidget *parent = nullptr);
    ~RTCurve();

private:
    Ui::RTCurve *ui;
};

#endif // RTCURVE_H
