#include "userdatabase.h"

UserDatabase::UserDatabase()
{
    if (user_db.contains("qt_sql_default_connection"))
    {
        user_db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        user_db = QSqlDatabase::addDatabase("QSQLITE");
    }

    user_db.setDatabaseName("./user_database.db");

    QDir current_path = QDir::currentPath();
    QSqlQuery query;

    if (!current_path.exists("user_database.db"))
    {
        if (!user_db.open())
        {
            qDebug() << __func__ << __LINE__ << user_db.lastError();
        }

        this->create_table();

//        this->insert_values(0, "adminstrastor", "*mohsuper1", 0);
//        this->insert_values(1, "technician", "&mohtest2", 1);
//        this->insert_values(2, "customer", "#mohuser3", 2);

        this->insert_values(0, "adminstrastor", "123", 0);
        this->insert_values(1, "technician", "456", 1);
        this->insert_values(2, "customer", "789", 2);
    }
    else
    {
        if (!user_db.open())
        {
            qDebug() << __func__ << __LINE__ << user_db.lastError();
        }
    }
}

UserDatabase::~UserDatabase()
{
    user_db.close();
}

void UserDatabase::create_table()
{
    QSqlQuery query;
    QString cmds = "create table all_users (id int primary key, accounts vchar(30), password vchar(30), level int)";

    query.prepare(cmds);

    if (!query.exec())
    {
        qDebug() << __func__ << __LINE__ << query.lastError();
    }
}

void UserDatabase::insert_values(quint8 id, QString accounts, QString password, int level)
{
    QSqlQuery query;
    QString insert_2db = "insert into all_users values (%1,'%2','%3',%4)";
    insert_2db = insert_2db.arg(id).arg(accounts).arg(password).arg(level);

    query.prepare(insert_2db);
    if (!query.exec())
    {
        qDebug() << __LINE__ << "Insert error :" << query.lastError();
    }
}

void UserDatabase::delete_user_by_acccount(QString accounts)
{
    QSqlQuery query;
    QString delete_record = "delete from all_users where accounts = '%1'";

    delete_record = delete_record.arg(accounts);

    query.prepare(delete_record);

    if (!query.exec())
    {
        qDebug() << "Delete error: " << query.lastError();
    }
}

QString UserDatabase::get_useraccounts_by_id(int id)
{
    QSqlQuery query;
    QString get_accounts = "select accounts from all_users where id = %1";

    get_accounts = get_accounts.arg(id);

    query.prepare(get_accounts);

    if (!query.exec())
    {
        qDebug() << __func__ << __LINE__ << query.lastError();
    }

    query.first();

    QString accounts = query.value(0).toString();

    return accounts;
}

QString UserDatabase::get_userpassword_by_accounts(const QString accounts)
{
    QSqlQuery query;
    QString get_password = "select password from all_users where accounts = '%1'";

    get_password = get_password.arg(accounts);

    query.prepare(get_password);

    if (!query.exec())
    {
        qDebug() << __func__ << __LINE__ << query.lastError();
    }

    query.first();

    QString password = query.value(0).toString();

    return password;
}
