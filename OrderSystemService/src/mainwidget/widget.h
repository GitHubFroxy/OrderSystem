#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QTimer;
class OrderInfoWidget;
class MenuInfoWidget;
class Mytcpserver;
class Protocol;
class AcountManageWidget;
class BillWidget;
class StatisticWidget;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    enum Widget_Index{
        ORDER_INFO_WIDGET = 2,
        MENU_WIDGET,
        ACCOUNT_WIDGET,
        BILL_WIDGET,
        STATISTIC_WIDGET
    };
    
public:
    static Widget *getInstance(const QString &name);
private:
    explicit Widget(const QString &name,QWidget *parent = 0);
    ~Widget();

    static Widget *m_pInstance;

    void initStle();

    void initConnect();

    void initWidget();

    void init();

    void initService();

protected:
    bool eventFilter(QObject *, QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

Q_SIGNALS:
    void signalUpdateTableInfo(const QString&);
    void signalUpdataMenuInfo(const QStringList &);
    void signalSendMenuPageInfo(const QString &);    //发送订单数据包信息，用于显示

    //发给显示界面，提示该菜的状态
    void signalChangeMenuState(const QString tableId,
                               const QString menuName,
                               const QString &state);
    //发送结账的台号信息给结账界面
    void signalSendAccountTableIdInfo(const QString &tableId);

    //向订单界面发送消息，删除厨房发送的缺菜 菜单信息
    void signalSendOrderInfoLackMenu(const QString &);

private Q_SLOTS:
    void slot_showMenu();
    void slot_showMax();
    void slot_showMin();
    void slot_close();
    void slotShowCurrentTime();

    void ClientReadData(int clientID,QString IP,int Port,QByteArray data);
    void ClientConnect(int clientID,QString IP,int Port);
    void ClientDisConnect(int clientID,QString IP,int Port);

    //发送结账信息给厨房端
    void slotSendToKitchenBillInfo(const QString &tableId);

    //发送给手持端，显示菜的状态
    void slotSendTOEndDeviceChangeMenuInfo(const QString&);

    //通知手持端，已经接收到所发送的订单数据包
    void slotSendTOEndDeviceReceiceMenuInfoOk();
    //通知手持端，已经完成所请求的结账请求
    void slotSendToEndDeviceReceiveAccountOk(const QString &);

    //向厨房发送当前的订单数据包
    void slotSendToKitchenMenuPageInfo();

    void on_tbn_menu_clicked();

    void on_tbnOrderInfo_clicked();

    void on_tbnStatistic_clicked();

    //消息处理相关函数
    void on_tbnAcount_clicked();

    void on_tbnAccount_clicked();

private:
    void execMessage(QByteArray);
    void execMessage();

    void execOpenTableInfo(QStringList info);       //处理开台信息
    void execOrderMenuInfo(QStringList info);       //处理菜单信息
    void execOrderMenuOkInfo(const QString &info);  //处理菜单确认信息
    void execHurryMenuInfo(QStringList info);       //处理催菜信息
    void execAddMenuInfo(QStringList info);         //处理加菜信息
    void execMenuAccount(QStringList info);         //处理结账信息
    void execLackMenuInfo(QStringList info);        //处理缺菜信息
    void execReturnMenu(QStringList info);          //处理退菜信息
    void execFinishMenu(QStringList info);          //处理完菜信息

private:
    Ui::Widget *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

private:

    QTimer*m_pTimer;

    OrderInfoWidget *m_pOrderInfoWidget;        //订单界面
    MenuInfoWidget *m_pMenuInfoWidet;           //菜谱界面
    AcountManageWidget *m_pAccountWidget;       //账户管理
    BillWidget *m_pBillWidget;                  //结账管理界面
    StatisticWidget *m_pStatisticWidget;        //历史消费记录界面

    Mytcpserver *m_pTcpService;
    Protocol *m_pProtocol;

    int m_kitchenClientId;
    QString m_kitchenIp;                  //设备地址，用于进行通讯
    int m_kitchenPort;

    int m_endDeviceClientId;
    QString m_endDeviceIp;
    int m_endDevicePort;
    QString m_currTableId;                  //当前台号
    QString m_menuPage;

};

#endif // WIDGET_H
