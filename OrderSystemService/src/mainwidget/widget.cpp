#include "widget.h"
#include "ui_widget.h"
#include "iconhelper.h"
#include "myhelper.h"

#include "orderinfowidget.h"
#include "menuinfowidget.h"
#include "mytcpserver.h"
#include "protocol.h"
#include "acountmanagewidget.h"
#include "billwidget.h"
#include "statisticwidget.h"

#include <QTimer>
#include <QDateTime>

Widget *Widget::m_pInstance = NULL;

Widget *Widget::getInstance(const QString &name)
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new Widget(name);
    }
    return m_pInstance;
}

Widget::Widget(const QString &name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->init();
    ui->label_CurrentName->setText(name);
    if (name != "admin")
    {
        ui->tbnStatistic->setVisible(false);
        ui->tbn_menu->setVisible(false);
        ui->tbnAcount->setVisible(false);
        //        ui->tbn_menu->setEnabled(false);
        //        ui->tbnAcount->setEnabled(false);
        //        ui->tbnStatistic->setEnabled(false);
    }
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
    setMinimumWidth(850);
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
    ui->lab_Title->setText("点餐系统服务器端");
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

    ui->label_device->setText("无设备连接!");
    ui->label_info->setText("");
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

    //关联客户请求信号
    connect(m_pTcpService,SIGNAL(ClientConnect(int,QString,int)),
            this,SLOT(ClientConnect(int,QString,int)));
    connect(m_pTcpService,SIGNAL(ClientDisConnect(int,QString,int)),
            this,SLOT(ClientDisConnect(int,QString,int)));
    connect(m_pTcpService,SIGNAL(ClientReadData(int,QString,int,QByteArray)),
            this,SLOT(ClientReadData(int,QString,int,QByteArray)));

    connect(this,SIGNAL(signalUpdateTableInfo(QString)),
            m_pOrderInfoWidget,SLOT(slotUpdateTableInfo(QString)));
    connect(this,SIGNAL(signalUpdataMenuInfo(QStringList)),
            m_pOrderInfoWidget,SLOT(slotUpdataMenuInfo(QStringList)));


    /*****************订单界面相关的信号和槽连接*******************/
    //发送订单数据包信息，给订单界面，作为显示
    connect(this,SIGNAL(signalSendMenuPageInfo(QString)),
            m_pOrderInfoWidget,SLOT(slotReceiveMenuPageInfo(QString)));
    //订单界面接收到后返回接收成功信息
    connect(m_pOrderInfoWidget,SIGNAL(signalReceiveMenuPageOk()),
            this,SLOT(slotSendTOEndDeviceReceiceMenuInfoOk()));
    connect(this,SIGNAL(signalChangeMenuState(QString,QString,QString)),
            m_pOrderInfoWidget,SLOT(slotChangeMenuState(QString,QString,QString)));
    //发送缺菜信息给订单界面，提示是否删除缺菜的订单
    connect(this,SIGNAL(signalSendOrderInfoLackMenu(QString)),
            m_pOrderInfoWidget,SLOT(slotReceiveLackMenuInfo(QString)));

    //接收订单界面命令，向厨房发送订单数据包
    connect(m_pOrderInfoWidget,SIGNAL(signalSendToKitchenMenuPageInfo()),
            this,SLOT(slotSendToKitchenMenuPageInfo()));

    /****************************结账*****************************/
    //服务器收到手持端的结账请求，服务器同意后发送给结账界面信息(台号)
    connect(this,SIGNAL(signalSendAccountTableIdInfo(QString)),
            m_pBillWidget,SLOT(slotReceiveTableId(QString)));
    //结账界面结账成功后，发射的信号，通知厨房，进行结账后的显示工作
    connect(m_pBillWidget,SIGNAL(signalSendBillInfo(QString)),
            this,SLOT(slotSendToKitchenBillInfo(QString)));
    //通知订单显示界面类，清空当前台号信息
    connect(m_pBillWidget,SIGNAL(signalSendBillInfo(QString)),
            m_pOrderInfoWidget,SLOT(slotDeleteTableInfo(QString)));
    //通知手持端，已经完成了结账
    connect(m_pBillWidget,SIGNAL(signalSendBillInfo(QString)),
            this,SLOT(slotSendToEndDeviceReceiveAccountOk(QString)));
    //通知厨房，顾客已经结账，清楚临时信息
    connect(m_pBillWidget,SIGNAL(signalSendBillInfo(QString)),
            this,SLOT(slotSendToKitchenBillInfo(QString)));

    //通知历史账单界面，刷新显示数据
    connect(m_pBillWidget,SIGNAL(signalSendRefreshStatisticInfo()),
            m_pStatisticWidget,SLOT(slotRefreshInfo()));

    /***********************关联菜谱信息改变信号*******************/
    connect(m_pMenuInfoWidet,SIGNAL(signalSendChangeMenuInfo(QString)),
            this,SLOT(slotSendTOEndDeviceChangeMenuInfo(QString)));

}

void Widget::initWidget()
{
    m_pProtocol = new Protocol;

    m_pOrderInfoWidget = new OrderInfoWidget;
    m_pMenuInfoWidet = new MenuInfoWidget;
    m_pAccountWidget = new AcountManageWidget;
    m_pBillWidget = new BillWidget;
    m_pStatisticWidget = new StatisticWidget;

    ui->stackedWidget->addWidget(m_pOrderInfoWidget);
    ui->stackedWidget->addWidget(m_pMenuInfoWidet);
    ui->stackedWidget->addWidget(m_pAccountWidget);
    ui->stackedWidget->addWidget(m_pBillWidget);
    ui->stackedWidget->addWidget(m_pStatisticWidget);

    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),
            this,SLOT(slotShowCurrentTime()));
    m_pTimer->start(1000);
}

void Widget::init()
{
    this->initService();

    this->initStle();

    this->initWidget();

    this->initConnect();
}

void Widget::initService()
{
    m_pTcpService = new Mytcpserver(this);
    //进行套接字监听
    bool ok = m_pTcpService->listen(QHostAddress::Any,6666);
    if (ok)
    {
        myHelper::ShowMessageBoxInfo("服务器开启成功!");
    }else
    {
        myHelper::ShowMessageBoxInfo("服务器开启失败!");
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

//当客户端请求连接时，获取发送的内容
void Widget::ClientReadData(int clientID, QString IP, int Port, QByteArray data)
{
    Q_UNUSED(clientID);
    Q_UNUSED(IP);
    Q_UNUSED(Port);

    if (!data.isEmpty())
    {
        qDebug()<<QString::fromLocal8Bit(data);
        QString info = QString::fromLocal8Bit(data);
        QStringList str = info.split('#');
        qDebug()<<str;
        quint8 head = QString(str.at(1)).toUInt();
        //0:开台,1:点菜,2:确认菜单,3:退菜,4:催菜
        switch (head) {
        //        case BEG_TABLE:
        //            execOpenTableInfo(str);
        //            break;
        //        case ORDER_DISH:
        //            execOrderMenuInfo(str);
        //            break;
        case ORDER_OK:
            execOrderMenuOkInfo(info);
            break;
        case HURRY_MENU:                //催菜
            execHurryMenuInfo(str);
            break;
        case ADD_MENU:
            execAddMenuInfo(str);
            break;
        case RETURN_MENU:
            execReturnMenu(str);
            break;
        case MENU_ACCOUNT:              //结账
            execMenuAccount(str);
            break;
        case LACK_MENU:                 //缺菜
            execLackMenuInfo(str);
            break;
        case FINISH_MENU:
            execFinishMenu(str);
            break;
        default:
            break;
        }

        //execMessage(data);
        //        QString temp = myHelper::ByteArrayToHexStr(data);
        //        qDebug()<<"receive data:"<<temp;
        //        qDebug()<<"temp.size():"<<temp.size();
        //        qDebug()<<"client_id:"<<clientID;
        //        qDebug()<<"IP:"<<IP;
        //        qDebug()<<"port:"<<Port;
    }
}

//客户端断开连接后进行一些处理以及状态显示
void Widget::ClientConnect(int clientID, QString IP, int Port)
{
    qDebug()<<"客户端"<<clientID
           <<"ip:"<<IP<<"port:"<<Port<<"上线";
    QStringList ipInfo = IP.split(':');
    if ("1" == IP.right(1))
    {
        ui->label_device->setText("手持端设备上线!");
        m_endDeviceClientId = clientID;
        m_endDeviceIp = ipInfo.at(1);
        m_endDevicePort = Port;
        qDebug() <<"手持端设备上线!";
    }else
    {
        ui->label_device->setText("厨房端设备上线!");
        m_kitchenClientId = clientID;
        m_kitchenIp =  ipInfo.at(1);
        m_kitchenPort = Port;
        qDebug() <<"厨房端设备上线";
    }
}

//
void Widget::ClientDisConnect(int clientID, QString IP, int Port)
{
    qDebug()<<"客户端"<<clientID
           <<"ip:"<<IP<<"port:"<<Port<<"下线";
    if ("1" == IP.right(1))
    {
        ui->label_device->setText("手持端设备下线!");
        qDebug()<<"手持端设备下线!";
    }else
    {
        ui->label_device->setText("厨房端设备下线!");
        qDebug()<<"厨房端设备下线";
    }
}

/*
 * 结账完成后发送给厨房，清除此刻结账顾客订单信息
 * 发送结账的台号给厨房，通知清除该台号对应的菜单信息
*/
void Widget::slotSendToKitchenBillInfo(const QString &tableId)
{
    QString data = "#"+QString::number(ACCOUNT_OK)+"#"+tableId+"#";
    m_pTcpService->SendData(m_kitchenClientId,
                            m_kitchenIp,
                            m_kitchenPort,data.toLocal8Bit());
}

void Widget::slotSendTOEndDeviceChangeMenuInfo(const QString &tableId)
{
}

//向手持端发送接收订单完毕信息
void Widget::slotSendTOEndDeviceReceiceMenuInfoOk()
{
    QString data = "#"+QString::number(ORDER_OK)+"#";
    m_pTcpService->SendData(m_endDeviceClientId,
                            m_endDeviceIp,
                            m_endDevicePort,data.toLocal8Bit());
}

//发送信息给手持端，提示已经完成了结账请求
void Widget::slotSendToEndDeviceReceiveAccountOk(const QString &tableId)
{
    QString data = "#"+QString::number(ACCOUNT_OK)+"#"+tableId+"#";
    m_pTcpService->SendData(m_endDeviceClientId,
                            m_endDeviceIp,
                            m_endDevicePort,data.toLocal8Bit());
}

//发送订单数据包给厨房
void Widget::slotSendToKitchenMenuPageInfo()
{
    QString data = m_menuPage;
    m_pTcpService->SendData(m_kitchenClientId,
                            m_kitchenIp,
                            m_kitchenPort,data.toLocal8Bit());
}


//菜谱管理
void Widget::on_tbn_menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(MENU_WIDGET);
}

//订单信息
void Widget::on_tbnOrderInfo_clicked()
{
    ui->stackedWidget->setCurrentIndex(ORDER_INFO_WIDGET);
}

//统计数据---后期增加
void Widget::on_tbnStatistic_clicked()
{
    ui->stackedWidget->setCurrentIndex(STATISTIC_WIDGET);
}

void Widget::execMessage(QByteArray by)
{
    QString temp = myHelper::ByteArrayToHexStr(by);
    qDebug()<<temp;
    bool ok;

    if (BGN_RESD_MSG == temp.left(2).toUInt(&ok,16))
    {
        m_pProtocol->setProtocolLen(temp.mid(2,2).toUInt(&ok,16));
        m_pProtocol->setFlag(temp.mid(4,2).toUInt(&ok,16));
        //如果是台号
        if (m_pProtocol->getFlag() == TABLE_ID)
        {
            QString data = "";
            quint8 te;
            for (int i = 6;i < 12;i+=2)
            {
                te = temp.mid(i,2).toUInt(&ok,16) - DATA_TO_ASCII;
                data += QString::number(te);
            }

            if (!m_pProtocol->isContainTableId(data))
            {
                qDebug()<<"not contain";
                m_pProtocol->insertTableId(data);
                m_pProtocol->setData(data);
            }else
            {
                m_pProtocol->setData(""
                                     "");
                qDebug()<<"contain";
            }
        }else if (m_pProtocol->getFlag() == MENU_ID)
        {
            //或者是菜单信息，只发送菜单的编号，因为菜单的编号在数据库中是唯一的
            QString menuId;
            quint8 t;
            int len = m_pProtocol->getProtocolLen();
            if (len == 5)
            {
                menuId = QString::number(temp.mid(6,2).toUInt(&ok,16) - DATA_TO_ASCII);
            }else if (len == 6)
            {
                for (int i = 6;i <9;i+=2)
                {
                    t = temp.mid(i,2).toUInt(&ok,16) -DATA_TO_ASCII;
                    menuId += QString::number(t);
                }
            }
            m_pProtocol->setData(menuId);
        }
        execMessage();

    }else
    {
        return;
    }
}

void Widget::execMessage()
{
    switch( m_pProtocol->getFlag())
    {
    case TABLE_ID:
        emit signalUpdateTableInfo(m_pProtocol->getData());
        break;
    case MENU_ID:
        //emit signalUpdataMenuInfo(m_pProtocol->getData());
        break;
    default:
        break;
    }
}

//"","0","011",""
void Widget::execOpenTableInfo(QStringList info)
{
    QString tableId = info.at(2);
    m_currTableId = tableId;
    qDebug()<<"执行台号相关操作函数,台号为:"<<tableId;
    emit signalUpdateTableInfo(tableId);
}

//"","1","鱼香肉丝","12","1","",""
//#特殊编号#菜名#价格#数量#备注#
void Widget::execOrderMenuInfo(QStringList info)
{
    //这里直接发送菜谱的信息,在菜谱界面中进行相应的解析
    emit signalUpdataMenuInfo(info);
    /*向厨房发送菜单信息
     * 格式:#台号#菜名#数量#备注#
     * 新的格式： #点餐#台号#菜名#数量#备注#
    */
    QString sendData = "#"+QString::number(ORDER_DISH)+"#"+m_currTableId+"#"+info.at(2)+"#1#"+"#";
    m_pTcpService->SendData(m_kitchenClientId,m_kitchenIp,
                            m_kitchenPort,sendData.toLocal8Bit());
}

/*接收点菜完毕信息
 * #2#
 * 2016年4月23日21:17:01
 * 修改:顾客点击确认按钮后发送订单列表信息，可能包含好几条菜单信息，
 *     这里先不解析，发送给订单界面后解析
*/
void Widget::execOrderMenuOkInfo(const QString &info)
{
    qDebug()<<info;
    m_menuPage = info;
    emit signalSendMenuPageInfo(info);
    //界面跳转到订单界面
    ui->stackedWidget->setCurrentIndex(ORDER_INFO_WIDGET);

    //    //同时将菜单信息发送给厨房
    //    m_pTcpService->SendData(m_kitchenClientId,
    //                            m_kitchenIp,
    //                            m_kitchenPort,info.toLocal8Bit());
}

/*
 * 处理手持端的催菜信息，解析后发送给厨房端,提示手持端要催菜
*/
void Widget::execHurryMenuInfo(QStringList info)
{
    qDebug()<<"处理催菜信息，准备发送给厨房"<<info;
    QString data = "#"+QString::number(HURRY_MENU)+"#"+info.at(2)+"#";

    m_pTcpService->SendData(m_kitchenClientId,
                            m_kitchenIp,
                            m_kitchenPort,data.toLocal8Bit());
}

//处理加菜信息
void Widget::execAddMenuInfo(QStringList info)
{
    Q_UNUSED(info);
}

/*
 * 处理客户端结账请求
 * ("", "6", "020", "")
*/
void Widget::execMenuAccount(QStringList info)
{
    //计算客户总消费，保存消费记录
    qDebug()<<"用户要结账:"<<info;
    /*主界面显示结账的台号，进行结账提示
     * 现在修改为：手持端发送结账请求后，服务器端弹出提示框，提示要结账了
    */
    QString tableId = info.at(2);
    if (myHelper::ShowMessageBoxQuesion("接收到手持端【"+tableId+"】号台要结账,现在是否要进行结账处理?"))
    {
        //进行结账处理，跳转到结账界面
        emit signalSendAccountTableIdInfo(tableId);
        ui->stackedWidget->setCurrentIndex(BILL_WIDGET);
    }else
    {
        //不执行结账，也就是不同意，同时状态栏显示要结账的台号信息
        ui->label_info->setText(tableId+"号桌要结账");
    }
}

/*
 * 处理缺菜信息
 * 格式:("", "7", "010", "红烧排骨", "")
*/
void Widget::execLackMenuInfo(QStringList info)
{
    //厨房端，发送的缺菜信息，现在要发送给手持端
    QString data = "#"+QString::number(LACK_MENU)+"#"+info.at(2)+"#"+info.at(3)+"#";
    qDebug()<<"服务器端接收到缺菜信息:"<<info;
    m_pTcpService->SendData(m_endDeviceClientId,
                            m_endDeviceIp,
                            m_endDevicePort,data.toLocal8Bit());
    //同时服务器端改变菜的状态
    emit signalChangeMenuState(info.at(2),info.at(3),"缺菜");

    myHelper::ShowMessageBoxInfo("该菜缺菜，将要删除该菜信息!");

    //现在修改为：直接删除厨房端发送的缺菜菜单信息(只发送台号)
    emit signalSendOrderInfoLackMenu("#"+info.at(2)+"#");
}

/*
 * 处理退菜信息
 * 格式： #标志#菜名#
*/
void Widget::execReturnMenu(QStringList info)
{
    //删除本地数据记录，刷新页面,并向厨房发送要删除的菜名
    QString data = "#"+QString::number(RETURN_MENU)+"#"+info.at(3)+"#";
    m_pTcpService->SendData(m_kitchenClientId,
                            m_kitchenIp,
                            m_kitchenPort,data.toLocal8Bit());
}

/*
 * 处理完菜信息
 * 格式为:#标志#台号#菜名#
 * ("", "8", "012", "宫保鸡丁", "")
*/
void Widget::execFinishMenu(QStringList info)
{
    qDebug()<<"服务器收到厨房端发送的完菜提示"<<info;
    //删除本地完成的菜名,是否要通知手持端？任务书上没有写，这里也就没有实现
    QString tablId = info.at(2);
    QString menuName = info.at(3);
    ui->label_info->setText("【"+tablId+"】号台 "+menuName+"已经做完");

    //这里更改所完成才的状态
    emit signalChangeMenuState(tablId,menuName,"完成");
    //发送手持端，提示这道菜已经做完
    QString data = "#"+QString::number(FINISH_MENU)+"#"+tablId+"#"+menuName+"#";
    m_pTcpService->SendData(m_endDeviceClientId,
                            m_endDeviceIp,
                            m_endDevicePort,data.toLocal8Bit());
}

//账号管理
void Widget::on_tbnAcount_clicked()
{
    ui->stackedWidget->setCurrentIndex(ACCOUNT_WIDGET);
}

//结账显示界面
void Widget::on_tbnAccount_clicked()
{
    ui->stackedWidget->setCurrentIndex(BILL_WIDGET);
}
