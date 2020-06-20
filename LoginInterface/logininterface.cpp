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

    trans = new QTranslator();

    if (get_language_from_ini_file() == English)
    {
        if (!trans->load(":/english.qm"))
        {
            qDebug() << __FILE__ << __LINE__ << "failed to load qm file";
        }
    }
    else
    {
        if (!trans->load(":/chinese.qm"))
        {
            qDebug() << __FILE__ << __LINE__ << "failed to load qm file";
        }
    }

    if (qApp->installTranslator(trans))
    {
        qDebug() << "succeed";
    }
    else
        qDebug() << "Failed";
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

LanguageScales LoginInterface::get_language_from_ini_file()
{
    QFile language_file(QString(QDir::currentPath()+"/language.ini"));

    if (!language_file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << __FILE__ << __LINE__ << "language init file open failed";
    }
    else
    {
        QByteArray tmp = language_file.readAll();
        if (tmp.isEmpty())
        {
            qDebug() << "file is empty";
            language_file.write("language=chinese");

            return Chinese;
        }
        else
        {
            if (tmp == "language=chinese")
                return Chinese;
            else if (tmp == "language=english")
                return English;
        }

        language_file.close();
    }

    return None;
}

void LoginInterface::on_login_btn_clicked()
{
    QString current_user = ui->accounts->currentText();
    QString entered_password = ui->password->text();

    if (is_accounts_password_matched(current_user, entered_password))
    {
        ui->progressBar->setVisible(true);

        delete db;

        this->close();

        int level = ui->accounts->currentIndex();

        ModelSelector *w = new ModelSelector(nullptr, Accounts(level), trans);
        w->show();
    }
    else
    {
//        QMessageBox::warning(this, "Tips", "密码错误！");
        ui->password_wrong_label->setText(tr("密码错误！"));
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

void LoginInterface::on_showPassword_stateChanged(int state)
{
    if (state == Qt::Checked)
        ui->password->setEchoMode(QLineEdit::Normal);
    else
        ui->password->setEchoMode(QLineEdit::Password);
}

void LoginInterface::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}
