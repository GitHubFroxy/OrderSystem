#include "widget.h"
#include "ui_widget.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "tablewidget/tablewidget.h"
#include "menuwidget/menuwidget.h"
#include "protocol.h"
#include "sqlhelp.h"

#include <QTimer>
#include <QDateTime>
#include <QTcpSocket>
#include <QHostAddress>

bool g_isConnect;

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

void Widget::initStle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint |
                         Qt::WindowSystemMenuHint |
                         Qt::WindowMinMaxButtonsHint);
    //设置窗体固定大小
    setMinimumWidth(1080);
    setFixedHeight(680);
    //获取当前窗体的位置
    location = this->geometry();
    //默认没有最大化
    max = false;
    //默认鼠标没有按下
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    //ui->lab_Title->installEventFilter(this);

    //设置软件标题
    ui->lab_Title->setText("点餐系统手持端");
    //这里取消软件最大化标志
    ui->btnMenu_Max->setVisible(false);

    //状态栏提醒文本设置
    ui->label_remard->setText("");

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);

    //窗体居中显示
    QDesktopWidget *desk = QApplication::desktop();
    this->move((desk->width()-this->width())/2,(desk->height() -this->height())/2);

    m_pSqlHep = new SqlHelp;

}

void Widget::initTempTableInfo()
{
    QString sql = "DELETE FROM [temp_menu]";
    if( m_pSqlHep->ExecuteSql(sql))
    {
        qDebug()<<"临时信息初始化成功!";
    }else
    {
        qDebug()<<"临时信息初始化失败!";
    }

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

    /**************关联选台界面发送的信息********************/
//    //向服务器发送台号
//    connect(m_pTableWidget,SIGNAL(signalSendTableId(QString)),
//            this,SLOT(slotReceiveTableId(QString)));

    //向菜谱界面发送信号
    connect(m_pTableWidget,SIGNAL(signalSendTableId(QString)),
            m_pMenuWidget,SLOT(slotReceiveTableId(QString)));
    //选完台后进行页面跳转
    connect(m_pTableWidget,SIGNAL(signalSendTableId(QString)),
            this,SLOT(slotChangeWidget(QString)));


    //关联和服务器相关的信号
//    connect(m_pMenuWidget,SIGNAL(signalSendToServiceMenuInfo(QString)),
//            this,SLOT(slotReceiveMenuInfo(QString)));

    //接收顾客点餐完毕信息，开始发送给服务器
    connect(m_pMenuWidget,SIGNAL(signalSendMenuOk(QString)),
            this,SLOT(slotSendToServiceMenuOk(QString)));
    //结账
    connect(m_pMenuWidget,SIGNAL(signalSendToServerMenuAccount(QString)),
            this,SLOT(slotSendToServiceMenuAccount(QString)));

    //关联菜单订单列表信息
    connect(m_pMenuWidget,SIGNAL(signalSendToServiceBackMenu(QString)),
            this,SLOT(slotSendToServiceReturnMenu(QString)));
    connect(m_pMenuWidget,SIGNAL(signalSendToServiceHurryMenu(QString)),
            this,SLOT(slotSendToServiceHurryMenu(QString)));

    //结账成功后，清除相关的临时信息
    connect(this,SIGNAL(signalClearMenuTemp()),
            m_pMenuWidget,SIGNAL(signalEmptyMenuTemp()));
    connect(this,SIGNAL(signalClearMenuTemp()),
            m_pTableWidget,SLOT(slotSetTableEnable()));
    connect(this,SIGNAL(signalSubmitOrderOk()),
            m_pMenuWidget,SIGNAL(signalSubmitOrderOk()));
}

void Widget::initWidget()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),
            this,SLOT(slotShowCurrentTime()));
    m_pTimer->start(1000);

    m_pTableWidget = new TableWidget;
    m_pMenuWidget = new MenuWidget;

    ui->stackedWidget->addWidget(m_pTableWidget);
    ui->stackedWidget->addWidget(m_pMenuWidget);

    m_pProtocol = new Protocol;
}

void Widget::init()
{

    this->initService();

    this->initStle();

    this->initTempTableInfo();

    this->initWidget();

    this->initConnect();
}

void Widget::initService()
{
    //通讯服务
    m_pTcpClient = new QTcpSocket(this);
    m_pTcpClient->abort();              //取消已有的连接
    g_isConnect = false;

    m_pTcpClient->connectToHost(QHostAddress("127.0.0.1"),6666);
    if (m_pTcpClient->waitForConnected(1000))
    {
        g_isConnect = true;
        ui->label_remind->setText("连接服务器成功");
        myHelper::ShowMessageBoxInfo("连接服务器成功!");
        qDebug()<<"连接服务器成功!";
    }else
    {
        g_isConnect = false;
        qDebug()<<"连接服务器失败!";
        ui->label_remind->setText("连接服务器失败，请检查!");
        myHelper::ShowMessageBoxError("连接服务器失败,请检查服务器是否打开!");
        return;
    }
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

//读取服务器数据
void Widget::SlotReadData()
{
    QByteArray buff = m_pTcpClient->readAll();
    if (!buff.isEmpty())
    {
        QString info = QString::fromLocal8Bit(buff);
        qDebug()<<"手持端接收到的数据:"<<info;
        QStringList str = info.split('#');
        quint8 head = QString(str.at(1)).toUInt();
        switch (head) {
        case LACK_MENU:                     //缺菜
            execLackMenuInfo(str);
            break;
        case ORDER_OK:                      //订单提交OK
            getServiceMenuInfo();
            break;
        case ACCOUNT_OK:                    //结账OK
            getServiceAccountOkInfo(info);
            break;
        case FINISH_MENU:                   //某到菜已经做完
            execFinishMenuInfo(info);
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
    ui->label_remind->setText("服务器断开连接!");
    g_isConnect = false;
}

//接收顾客点击的台号,用户向服务器发送
void Widget::slotReceiveTableId(const QString &id)
{
    QString data = "#" + QString::number(BEG_TABLE) + "#"+ id + "#";
    //m_pProtocol->packSendData(TABLE_ID,id);
    //m_pTcpClient->write(m_pProtocol->sendData());
    m_pTcpClient->write(data.toLocal8Bit());
}

/*接收菜单界面，向服务器发送菜单信息
 * 一条完整的菜谱信息帧:#1#鱼香肉丝#12#1#""#
 * 解释:#特殊编号#菜名#价格#数量#备注#
*/
void Widget::slotReceiveMenuInfo(const QString &menuInfo)
{
    QString data = "#"+QString::number(ORDER_DISH)+"#"+menuInfo+"#1#"+""+"#";
    //m_pProtocol->packSendData(MENU_ID,menuId);
    //m_pTcpClient->write(m_pProtocol->sendData());
    m_pTcpClient->write(data.toLocal8Bit());
    qDebug()<<"send menu id:"<<data;
}

/*
 * 点餐完毕
*/
void Widget::slotSendToServiceMenuOk(const QString &menuOk)
{
    QString data = "#"+QString::number(ORDER_OK)+menuOk;
    m_pTcpClient->write(data.toLocal8Bit());
    //ui->tbn_table->setEnabled(true);
    qDebug()<<"\n发送点餐完毕信息给服务器:"<<data<<endl;
    ui->label_remard->setText("点餐完毕信息正在发送中……");
}

/*
 * 发送结账信息给服务器
*/
void Widget::slotSendToServiceMenuAccount(const QString &tableId)
{
    QString data = "#"+QString::number(MENU_ACCOUNT)+"#"+tableId+"#";
    m_pTcpClient->write(data.toLocal8Bit());
    qDebug()<<"\n发送结账(台号)信息给服务器"<<data<<endl;
    qDebug()<<"结账的台号是："<<tableId;
    ui->label_remard->setText("结账信息已经发送给服务器,请前去结账!");
}

//向服务器发送退菜信息
//数据格式:#标志#菜名#
void Widget::slotSendToServiceReturnMenu(const QString &info)
{
    m_pTcpClient->write(info.toLocal8Bit());
    qDebug()<<"向服务器发送退菜信息"<<info;
    ui->label_remard->setText("正在发送退菜信息到服务器……");
}

/*
 * 想服务器发送催菜信息
 * 格式:#标志#菜名#
*/
void Widget::slotSendToServiceHurryMenu(const QString &info)
{
    m_pTcpClient->write(info.toLocal8Bit());
    qDebug()<<"向服务器发送催菜信息"<<info;
    ui->label_remard->setText("催菜信息已发达服务器,请您耐心等待");
}


/*
 * 接收到服务器数据，提示缺菜
 * 格式：("", "7", "010", "红烧排骨", "")
*/
void Widget::execLackMenuInfo(QStringList info)
{
    qDebug()<<"手持终端接收到服务器发送的缺菜信息:"<<info;
    ui->label_remard->setText("您所点的【"+info.at(3)+"】缺菜");
}

void Widget::getServiceMenuInfo()
{
    qDebug()<<"接收到服务器的确认信息，订单提交成功!";
    ui->label_remard->setText("订单提交成功!");
    ui->tbn_table->setEnabled(true);

    emit signalSubmitOrderOk();
}

//接收到服务器结账成功后的信息
void Widget::getServiceAccountOkInfo(const QString &info)
{
    Q_UNUSED(info);
    emit signalClearMenuTemp();
    ui->label_remard->setText("结账完毕!");
}

//解析服务器发送的  某到菜做完信息
void Widget::execFinishMenuInfo(const QString &info)
{
    QStringList temp = info.split("#");
    QString tableId = temp.at(2);
    QString menuName = temp.at(3);

    ui->label_remard->setText("【"+tableId+"】台 "+menuName+" 已经完成");
}

void Widget::slotChangeWidget(QString)
{
    ui->stackedWidget->setCurrentIndex(MENU_WIDGET);
    ui->tbn_table->setEnabled(false);
}

//显示开台界面
void Widget::on_tbn_table_clicked()
{
    ui->stackedWidget->setCurrentIndex(TABLE_WIDGET);
}

//菜单界面
void Widget::on_tbn_dish_clicked()
{
    ui->stackedWidget->setCurrentIndex(MENU_WIDGET);
}
