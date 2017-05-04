/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-16
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:基于Qt的点餐系统的设计---订单临时显示列表的实现
Function:将标题栏，任务栏，内容窗口进行布局
**************************************************/
#ifndef ORDERLISTWIDGET_H
#define ORDERLISTWIDGET_H

#include <QWidget>
#include <QMap>

class EvaluateDialog;
class SqlHelp;
class QSqlQueryModel;

namespace Ui {
class OrderListWidget;
}

class OrderListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderListWidget(QWidget *parent = 0);
    ~OrderListWidget();

private:
    void initForm();
    void init();
    void initData(QString tableName, QString columnNames[], int columnWidths[]);

    bool isContainValue(const QString &);
    bool valueIsExist(const QString &name);

    void refreshMenuInfo();

    void deleteMenuInfo(const QString &);       //删除对应菜单

    QString getAllOrdetMenuInfo();                 //获取当前订单中所有菜单信息

Q_SIGNALS:
    void signalSendMenuOk(const QString &);     //确认点菜
    void signalSendToServiceMenuAccount(const QString&); //结账(台号)
    void signalSendToServiceBackMenu(const QString &);  //退菜
    void signalSendToServiceHurryMenu(const QString&);  //催菜
    void signalRestartOrderMenu();

public Q_SLOTS:
    void slotReceiveMenuInfo(const QString &);
    void slotClearMenuInfo();
    void slotReceiveTableId(const QString &);

    //提交订单OK
    void  slotSubmitOrderOk();


private slots:
    void on_pbn_backMenu_clicked();

    void on_pbn_hurryMenu_clicked();

    void on_pbn_accounts_clicked();

    void on_pbnOk_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::OrderListWidget *ui;

    QStringList m_menuNameList;
    QMap<QString,QString>m_menuMap;
    int m_rowCount;

    EvaluateDialog *m_pEvaluateDialog;

    SqlHelp *m_pSqlHelp;
    QSqlQueryModel *model;

    QString ColumnNames[3];                    //列名数组声明
    int ColumnWidths[3];                       //列宽数组声明

    QString m_tableId;
    QString m_currMenuName;

    QString m_menuInfo;
    bool m_isSubmitOrder;                       //是否提交了订单
};

#endif // ORDERLISTWIDGET_H
