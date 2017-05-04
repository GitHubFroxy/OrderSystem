/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：数据库的创建文件
 *简要描述：创建数据库，数据表
 *
 *当前版本：V2.0
 *作者：Kelvin Li
 *创作日期：2014/10
 *说明：
*****************************************************************/
#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QtSql>
#include <QObject>


/*创建数据里并链接*/
static bool createConnection()
{
    QSqlDatabase db;            //创建一个数据库
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setHostName("localhost");
    db.setDatabaseName("OderSystemKitchen.db");  //设置数据库的名字
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
        qCritical("Can't open database: %s(%s)",
                  db.lastError().text().toLocal8Bit().data(),
                  qt_error_string().toLocal8Bit().data());

        return false;
    }

    return true;
}

/***************关闭数据库****************/
static bool closeConnection()
{
    QSqlDatabase::database().close();
    return 1;
}


#endif // DATABASE_H
