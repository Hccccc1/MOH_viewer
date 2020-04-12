#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>

enum Accounts
{
    SuperUser = 0,
    Technician,
    Customer,
};

class UserDatabase
{
public:
    UserDatabase();
    ~UserDatabase();

    void create_table();
    void insert_values(quint8 id, QString accounts, QString password, int level);
    void delete_user_by_acccount(QString accounts);
    QString get_useraccounts_by_id(int id);
    QString get_userpassword_by_accounts(const QString accounts);

    QSqlDatabase user_db;

private:
};

#endif // USERDATABASE_H
