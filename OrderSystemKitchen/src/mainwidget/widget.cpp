/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-18
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:基于Qt的点餐系统的设计----厨房终端实现
Function:
**************************************************/
#include "widget.h"
#include "ui_widget.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "orderinfowidget.h"
#include "protocol.h"
#include "sqlhelp.h"

#include <QTimer>
#include <QDateTime>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <QTimer>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initTempTable()
{
    QString sql = "DELETE FROM [menu_info]";
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"临时表信息初始化成功!";
    }
}

void Widget::initStle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint |
                         Qt::WindowSystemMenuHint |
                         Qt::WindowMinMaxButtonsHint);

    m_pSqlHelp = new SqlHelp;
    //设置窗体固定大小
    setMinimumWidth(800);
    setFixedHeight(580);
    //获取当前窗体的位置
    location = this->geometry();
    //默认没有最大化
    max = false;
    //默认鼠标没有按下
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    //ui->lab_Title->installEventFilter(this);

    //设置软件标题
    ui->lab_Title->setText("点餐系统---厨房终端");
    ui->label_remard->setText("");

    //这里取消软件最大化标志
    ui->btnMenu_Max->setVisible(false);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);

    //窗体居中显示
    QDesktopWidget *desk = QApplication::desktop();
    this->move((desk->width()-this->width())/2,(desk->height() -this->height())/2);
}

void Widget::initConnect()
{
    connect(ui->btnMenu,SIGNAL(clicked()),this,
            SLOT(slot_showMenu()));
    connect(ui->btnMenu_Max,SIGNAL(clicked()),this,
            SLOT(slot_showMax()));
    connect(ui->btnMenu_Min,SIGNAL(clicked()),this,
            SLOT(slot_showMin()));
    connect(ui->btnMenu_Close,SIGNAL(clicked()),this,
            SLOT(slot_close()));

    //关联socket信号和槽
    connect(m_pTcpClient,SIGNAL(readyRead()),
            this,SLOT(SlotReadData()));
    connect(m_pTcpClient,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(SlotReadError(QAbstractSocket::SocketError)));

    //关联订单界面
    connect(this,SIGNAL(signalSendMenuInfo(QString)),
            m_pOrderInfoWidget,SLOT(slotReceiveMenuInfo(QString)));
    connect(this,SIGNAL(signalSendHurryMenuInfo(QString)),
            m_pOrderInfoWidget,SLOT(slotReceiveHurryMenuInfo(QString)));
    connect(m_pOrderInfoWidget,SIGNAL(signalSendToServiceLackMenu(QString)),
            this,SLOT(slotSendToServiceLackMenu(QString)));
    connect(m_pOrderInfoWidget,SIGNAL(signalSendFinishMenu(QString)),
            this,SLOT(slotSendToServiceFinishMenu(QString)));
    //结账成功后刷新界面显示
    connect(this,SIGNAL(signalSendRefreshMenuInfo()),
            m_pOrderInfoWidget,SLOT(slotRefreshMenuInfo()));
}

void Widget::initWidget()
{
    m_pOrderInfoWidget = new OrderinfoWidget;

    ui->stackedWidget->addWidget(m_pOrderInfoWidget);

    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),
            this,SLOT(slotShowCurrentTime()));
    m_pTimer->start(1000);
}

void Widget::init()
{
    this->initService();

    this->initStle();

    this->initTempTable();

    this->initWidget();

    this->initConnect();
}

void Widget::initService()
{
    m_pTcpClient = new QTcpSocket(this);
    m_pTcpClient->abort();
    m_pTcpClient->connectToHost(QHostAddress(getLocalAddress()),6666);
    if (m_pTcpClient->waitForConnected(1000))
    {
        ui->label->setText("连接服务器成功");
        myHelper::ShowMessageBoxInfo("连接服务器成功!");
        qDebug()<<"连接服务器成功!";
    }else
    {
        qDebug()<<"连接服务器失败!";
        ui->label->setText("连接服务器失败，请检查!");
        myHelper::ShowMessageBoxError("连接服务器失败,请检查服务器是否打开!");
        return;
    }
}

QString Widget::getLocalAddress()
{
    QString add = "";
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            //qDebug() << address.toString();
            if (!address.isNull())
            {
                add = address.toString();
            }
        }
    }
    return add;
}

//处理顾客的催菜信息
void Widget::execHurryMenuInfo(const QString &info)
{
    QStringList data = info.split('#');
    qDebug()<<"厨房端收到顾客催菜信息:"<<data;
    ui->label_remard->setText("顾客催菜:"+data.at(2));
}

//接收从服务器发送过来的结账台号，删除对应的台号菜单
void Widget::execAccountInfo(const QString &info)
{
    QStringList data = info.split('#');
    qDebug()<<"厨房接收到服务器发送过来的结账信息:"<<data;
    QString sql = "DELETE FROM [menu_info] WHERE [table_id]='"+data.at(2)+"'";
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"结账完毕:删除顾客临时信息成功!";

        //发射信号，刷新界面
        emit signalSendRefreshMenuInfo();
    }else
    {
        qDebug()<<"结账完毕:删除顾客临时信息失败!";
    }
}

//读取服务器数据
void Widget::SlotReadData()
{
    QByteArray buff = m_pTcpClient->readAll();
    if (!buff.isEmpty())
    {
        QString info = QString::fromLocal8Bit(buff);
        QStringList str = info.split('#');
        qDebug()<<"接收到的数据:"<<info;
        quint8 head = QString(str.at(1)).toUInt();
        switch (head) {
        case ORDER_OK:
            emit signalSendMenuInfo(info);
        case LACK_MENU:                 //缺菜
            emit signalSendHurryMenuInfo(info);
            break;
        case HURRY_MENU:                //催菜
            execHurryMenuInfo(info);
            break;
        case ACCOUNT_OK:              //结账
            execAccountInfo(info);
            break;
        default:
            break;
        }

    }
}

//输出连接服务器错误信息，方便进行调试
void Widget::SlotReadError(QAbstractSocket::SocketError)
{
    m_pTcpClient->disconnectFromHost();
    qDebug()<<"连接服务器失败,原因:"<<m_pTcpClient->errorString();
    ui->label->setText("服务器断开连接!");
}

void Widget::slotSendToServiceLackMenu(const QString &info)
{
    QString data = "#"+QString::number(LACK_MENU) + "#"+info+"#";
    m_pTcpClient->write(data.toLocal8Bit());
}

//表示已经做完该菜
void Widget::slotSendToServiceFinishMenu(const QString &info)
{
    QString data = "#"+QString::number(FINISH_MENU) + "#"+info+"#";
    m_pTcpClient->write(data.toLocal8Bit());
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->slot_showMax();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void Widget::slot_showMenu()
{
}

void Widget::slot_showMax()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }

    max = !max;
}

void Widget::slot_showMin()
{
    this->showMinimized();
}

void Widget::slot_close()
{
    qApp->quit();
}

void Widget::slotShowCurrentTime()
{
    ui->lab_Info->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}


//显示订单界面
void Widget::on_tbn_order_clicked()
{
    ui->stackedWidget->setCurrentIndex(ORDERINFO_WIDGET);
}
