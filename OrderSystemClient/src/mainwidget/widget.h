#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAbstractSocket>

class TableWidget;
class MenuWidget;
class QTimer;
class QTcpSocket;
class Protocol;
class SqlHelp;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    enum Widget_Index{
        TABLE_WIDGET = 2,
        MENU_WIDGET
    };
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void initStle();

    void initTempTableInfo();

    void initConnect();

    void initWidget();

    void init();

    void initService();         //连接服务器

protected:
    bool eventFilter(QObject *, QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

Q_SIGNALS:
    void signalClearMenuTemp();     //通知顾客选菜界面，清空当前订单信息
    void signalSubmitOrderOk();     //表示提交订单OK
private Q_SLOTS:
    void slot_showMenu();
    void slot_showMax();
    void slot_showMin();
    void slot_close();
    void slotShowCurrentTime();

    //读取服务器发送过来的数据
    void SlotReadData();
    void SlotReadError(QAbstractSocket::SocketError);

    void slotReceiveTableId(const QString &);
    void slotReceiveMenuInfo(const QString &);

    //向服务器发送顾客的订单信息
    void slotSendToServiceMenuOk(const QString&);
    void slotSendToServiceMenuAccount(const QString &);
    void slotSendToServiceReturnMenu(const QString &);
    void slotSendToServiceHurryMenu(const QString &);

    void slotChangeWidget(QString);

    void on_tbn_table_clicked();

    void on_tbn_dish_clicked();

private:
    void execLackMenuInfo(QStringList info);
    //接收服务器返回的提交订单成功信息
    void getServiceMenuInfo();
    //接收服务器返回的提交 结账Ok信息
    void getServiceAccountOkInfo(const QString &);
    //接收服务器发送的某到菜做完消息
    void execFinishMenuInfo(const QString &);

private:
    Ui::Widget *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

private:
    TableWidget *m_pTableWidget;
    MenuWidget *m_pMenuWidget;

    QTimer*m_pTimer;
    QTcpSocket *m_pTcpClient;
    Protocol *m_pProtocol;
    SqlHelp*m_pSqlHep;
};

#endif // WIDGET_H
