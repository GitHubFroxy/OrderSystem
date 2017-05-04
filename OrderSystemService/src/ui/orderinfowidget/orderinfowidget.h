#ifndef ORDERINFOWIDGET_H
#define ORDERINFOWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>

class SqlHelp;
class QSqlQueryModel;

//菜谱信息结构体
struct MenuInfo{
    QString menu_name;
    double menu_price;
    double menu_num;
    double menu_totalPrice;
    QString menu_remark;
};
namespace Ui {
class OrderInfoWidget;
}

class OrderInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderInfoWidget(QWidget *parent = 0);
    ~OrderInfoWidget();

private:
    void init();
    void initForm();
    void initTempTable();
    void initConnect();

    //void insertTableId(const QString &);
    //void insertMenuInfo(const QString &,const QString &);

    bool valueIsExist(const QString &);

    void insertTableInfo(const QString&);
    void loadTableInfo();

    //刷新菜单信息,当顾客选择新单时
    void refreshMenuInfo();
    void refreshTableInfo();        //刷新台号信息

Q_SIGNALS:
    void signalReceiveMenuPageOk();         //传递给主界面，用户通知收到订单数据
    void signalSendToKitchenMenuPageInfo(); //发送数据包到厨房

private Q_SLOTS:
    void slotUpdateTableInfo(const QString &);
    void slotUpdataMenuInfo(const QStringList &menuinfo);

    //改变菜的状态
    void slotChangeMenuState(const QString &tableId,const QString menuName,
                         const QString &state);
    void slotDeleteTableInfo(const QString &tableId);

    //接收厨房缺菜信息，删除该菜
    void slotReceiveLackMenuInfo(const QString &);

    //接收订单数据包
    void slotReceiveMenuPageInfo(const QString &);

    void on_tableView_order_id_clicked(const QModelIndex &index);

    void on_tableView_order_id_customContextMenuRequested(const QPoint &pos);


    void on_pbnSendKitchen_clicked();

private:
    Ui::OrderInfoWidget *ui;
    QString m_CurMenuId;                    //当前菜谱编号
    QString m_currTableId;                  //当前台号

    QStringList m_tableIdList;
    QStringList m_menuIdList;
    //保存订单信息
    QMap<QString,QString>m_OrderInfoMap;

    QMap<QString,MenuInfo>m_menuInfoMap;

    QString ColumnNames[5];                    //列名数组声明
    int ColumnWidths[5];                       //列宽数组声明

    QString ColumnNamesTable[1];
    int ColumnWidthsTable[1];

    SqlHelp *m_pSqlHelp;
    QSqlQueryModel *m_PModel;

    QString m_orderMenuPageInfo;                //订单数据包
};

#endif // ORDERINFOWIDGET_H
