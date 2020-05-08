#include "logininterface.h"
#include "ui_logininterface.h"

#include "AllBitsAndRegs.h"

LoginInterface::LoginInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginInterface)
{
    ui->setupUi(this);

    QString accounts = db->get_useraccounts_by_id(SuperUser);
    ui->accounts->addItem(accounts);

    accounts = db->get_useraccounts_by_id(Technician);
    ui->accounts->addItem(accounts);

    accounts = db->get_useraccounts_by_id(Customer);
    ui->accounts->addItem(accounts);

    ui->password->setEchoMode(QLineEdit::Password);

    ui->progressBar->setVisible(false);
}

LoginInterface::~LoginInterface()
{
    delete ui;
}

bool LoginInterface::is_accounts_password_matched(const QString account, QString const entered_password) const
{
    QString password = db->get_userpassword_by_accounts(account);

    if (entered_password == password)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void LoginInterface::on_login_btn_clicked()
{
    QString current_user = ui->accounts->currentText();
    QString entered_password = ui->password->text();

    if (is_accounts_password_matched(current_user, entered_password))
    {
        ui->progressBar->setVisible(true);

//        for (int i = 0; i < 100; i++)
//        {
//            ui->progressBar->setValue(i);
//            QThread::msleep(20);
//        }

        delete db;

        this->close();

        ModelSelector *w = new ModelSelector();
        w->show();
    }
    else
    {
//        QMessageBox::warning(this, "Tips", "密码错误！");
        ui->password_wrong_label->setText("密码错误！");
        ui->password->clear();
    }
}

void LoginInterface::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
