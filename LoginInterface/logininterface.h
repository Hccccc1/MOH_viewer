#ifndef LOGININTERFACE_H
#define LOGININTERFACE_H

#include <QWidget>
#include <QMessageBox>
#include <QPalette>
#include <QBrush>
#include <QPainter>
#include <QThread>
#include "userdatabase.h"
#include "MOH_viewer/modelselector.h"

namespace Ui {
class LoginInterface;
}

class LoginInterface : public QWidget
{
    Q_OBJECT

public:
    explicit LoginInterface(QWidget *parent = nullptr);
    ~LoginInterface();

private:
    Ui::LoginInterface *ui;
    UserDatabase *db = new UserDatabase();

    QPalette palette;

    bool is_accounts_password_matched(const QString account, const QString  entered_password) const;

    void paintEvent(QPaintEvent *event);

private slots:
    void on_login_btn_clicked();
    void on_showPassword_stateChanged(int state);
};

#endif // LOGININTERFACE_H
