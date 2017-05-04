#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAbstractSocket>

class QTimer;
class QTcpSocket;
class OrderinfoWidget;
class QTimer;
class SqlHelp;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    enum Widget_Index{
        ORDERINFO_WIDGET = 2
    };
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void initTempTable();

    void initStle();

    void initConnect();

    void initWidget();

    void init();

    void initService();

    QString getLocalAddress();

    void execHurryMenuInfo(const QString &);
    void execAccountInfo(const QString &);

protected:
    bool eventFilter(QObject *, QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

Q_SIGNALS:
    void signalSendMenuInfo(const QString &);
    void signalSendHurryMenuInfo(const QString &);
    void signalSendRefreshMenuInfo();
private slots:
    void slot_showMenu();
    void slot_showMax();
    void slot_showMin();
    void slot_close();
    void slotShowCurrentTime();

    void on_tbn_order_clicked();

    void SlotReadData();
    void SlotReadError(QAbstractSocket::SocketError);

    //向服务器发送缺菜信息
    void slotSendToServiceLackMenu(const QString &);
    //向服务器发送 完菜信息
    void slotSendToServiceFinishMenu(const QString &);

private:
    Ui::Widget *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

private:
    QTimer*m_pTimer;
    QTcpSocket *m_pTcpClient;
    OrderinfoWidget *m_pOrderInfoWidget;

    SqlHelp *m_pSqlHelp;

//    QTimer *m_pShowTimer;
//    QTimer *m_pStayTimer ;
//    QTimer *m_pCloseTimer;
};

#endif // WIDGET_H
