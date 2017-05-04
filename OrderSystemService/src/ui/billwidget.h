/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-18
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:基于Qt的点餐系统的设计----账单查询
Function:
**************************************************/

#ifndef BILLWIDGET_H
#define BILLWIDGET_H

#include <QWidget>

class SqlHelp;
class QSqlQueryModel;

namespace Ui {
class BillWidget;
}

class BillWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BillWidget(QWidget *parent = 0);
    ~BillWidget();

private:
    void initForm();

    void initData(QString sql,QString columnNames[], int columnWidths[]);
    void refreshData(const QString &);

Q_SIGNALS:
    void signalSendBillInfo(const QString &tableId);        //发送结账信息
    void signalSendRefreshStatisticInfo();

private slots:
    void on_pbnAccount_clicked();

    void on_pbnFind_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_lineEdit_find_textChanged(const QString &arg1);

    //接收结账的台号ID，进行结账显示
    void slotReceiveTableId(const QString &);

private:
    Ui::BillWidget *ui;

    QString ColumnNamesTable[6];
    int ColumnWidthsTable[6];

    SqlHelp *m_pSqlHelp;
    QSqlQueryModel *model;

    QString m_currTableId;
};

#endif // BILLWIDGET_H
