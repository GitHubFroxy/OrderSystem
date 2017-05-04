/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-18
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:基于Qt的点餐系统的设计----账号信息管理
Function:
**************************************************/
#ifndef ACOUNTMANAGEWIDGET_H
#define ACOUNTMANAGEWIDGET_H

#include <QWidget>
class SqlHelp;
class QSqlQueryModel;

namespace Ui {
class AcountManageWidget;
}

class AcountManageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AcountManageWidget(QWidget *parent = 0);
    ~AcountManageWidget();

private:
    void initForm();
    void init();
    void initConnect();

    void initData(QString tableName,QString columnNames[], int columnWidths[]);

private slots:
    void on_pbnOk_clicked();

    void on_pbnAdd_clicked();

    void on_pbnModify_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_lineEdit_r_pwd_editingFinished();

private:
    Ui::AcountManageWidget *ui;

    bool m_isModify;
    SqlHelp *m_pSqlHelp;

    QString ColumnNames[4];
    int ColumnWidths[4];
    QSqlQueryModel *m_model;
    QString m_currUserId;
};

#endif // ACOUNTMANAGEWIDGET_H
