#include "menuwidget.h"
#include "ui_menuwidget.h"
#include "common/abstracttablewidget.h"
#include "common/abstractmenuwidget.h"
#include "orderlistwidget.h"
#include "sqlhelp.h"
#include "myhelper.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QDebug>
#include <QLabel>
#include <QSqlQuery>
#include <QPushButton>

#define MENU_NUM            27      //
#define PER_COLUMN_COUNT    3       //列数
#define PER_ROW_COUNT       3       //行数
//每页显示的菜单个数
#define PER_PAGE_COUNT      PER_COLUMN_COUNT * PER_ROW_COUNT

extern bool g_isConnect;        //是否和服务器连接
extern bool g_isCheckTable;     //是否已经选台

MenuWidget::MenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuWidget)
{
    ui->setupUi(this);

    initForm();
    //initData();
    initMenuInfo();
    initWidget();
    initConnect();
}

MenuWidget::~MenuWidget()
{
    delete ui;
}

void MenuWidget::initForm()
{
}

void MenuWidget::initData()
{
    m_pSqlHelp = new SqlHelp;
    QString sql = "SELECT menu_name FROM menu_info";
    m_textList = m_pSqlHelp->GetTableColumnValue(sql);
}

void MenuWidget::initWidget()
{
    m_pMainLayout = new QGridLayout;
    m_PSignalMapper = new QSignalMapper;

    m_pPbnNext = new QPushButton;
    m_pPbnNext->setText("下一页");
    m_pPbnNext->setFixedWidth(60);
    m_pPbnNext->setFlat(true);

    m_pPbnPrevious = new QPushButton;
    m_pPbnPrevious->setText("上一页");
    m_pPbnPrevious->setFixedWidth(70);
    m_pPbnPrevious->setFlat(true);

    m_pLabelTotalNum = new QLabel;
    m_pLabelTotalNum->setText("共3页");

    m_pLabelCurrentPage = new QLabel;
    m_pLabelCurrentPage->setText("当前第 1 页");

    QHBoxLayout *pbnLayout = new QHBoxLayout;
    pbnLayout->addStretch();
    pbnLayout->addWidget(m_pLabelTotalNum);
    pbnLayout->addSpacing(10);
    pbnLayout->addWidget(m_pLabelCurrentPage);
    pbnLayout->addSpacing(6);
    pbnLayout->addWidget(m_pPbnPrevious);
    pbnLayout->addSpacing(20);
    pbnLayout->addWidget(m_pPbnNext);
    pbnLayout->addStretch();
    pbnLayout->setContentsMargins(0,0,0,10);

    for (int i = 0;i <MENU_NUM;i++)
    {
        m_iconList <<QString(":/image/menu/%1.png").arg(i);
        m_MenuMap.insert(m_iconList.at(i),m_iconList.at(i));
    }

    //创建菜谱列表,并进行网格布局
    for (int i = 0; i < PER_PAGE_COUNT; i++)
    {
        AbstractMenuWidget *w = new AbstractMenuWidget(m_iconList.at(i),
                                                       m_textList.at(i),
                                                       m_priceList.at(i));
        m_menu_list.append(w);
        connect(w,SIGNAL(signalAddMenu()),m_PSignalMapper,SLOT(map()));
        m_PSignalMapper->setMapping(w,m_textList.at(i));
        m_pMainLayout->addWidget(w,i/PER_ROW_COUNT,i%PER_COLUMN_COUNT);
    }

    //菜谱总数
    int menu_list_count = m_iconList.count();
    //总共有几页
    page_count = menu_list_count /PER_PAGE_COUNT;
    //当前菜单的编号(0~8),8号
    page_count_point = PER_PAGE_COUNT;
    //当前页数
    current_page = 1;
    m_pPbnPrevious->setEnabled(false);


    connect(m_PSignalMapper,SIGNAL(mapped(QString)),this,SLOT(slotAddMenu(QString)));

    m_pMainLayout->setSpacing(15);
    m_pMainLayout->setContentsMargins(4,10,10,10);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(m_pMainLayout);
    rightLayout->addLayout(pbnLayout);

    m_pOrderWidget = new OrderListWidget;
    m_pLabelTableId = new QLabel;
    m_pLabelTableId->setStyleSheet("font-size:15px;");
    m_pLabelTableId->setAlignment(Qt::AlignCenter);

    m_pLeftLayout = new QVBoxLayout;
    m_pLeftLayout->addSpacing(6);
    m_pLeftLayout->addWidget(m_pLabelTableId);
    m_pLeftLayout->addWidget(m_pOrderWidget);
    m_pLeftLayout->setContentsMargins(0,0,0,0);

    //水平布局
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(m_pLeftLayout);
    layout->addLayout(rightLayout);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
}

void MenuWidget::initConnect()
{
    connect(this,SIGNAL(signalSendMenuInfo(QString)),
            m_pOrderWidget,SLOT(slotReceiveMenuInfo(QString)));

    connect(m_pOrderWidget,SIGNAL(signalSendMenuOk(QString)),
            this,SIGNAL(signalSendMenuOk(QString)));

    connect(m_pOrderWidget,SIGNAL(signalSendToServiceMenuAccount(QString)),
            this,SIGNAL(signalSendToServerMenuAccount(QString)));

    connect(m_pOrderWidget,SIGNAL(signalSendToServiceBackMenu(QString)),
            this,SIGNAL(signalSendToServiceBackMenu(QString)));

    connect(m_pOrderWidget,SIGNAL(signalSendToServiceHurryMenu(QString)),
            this,SIGNAL(signalSendToServiceHurryMenu(QString)));

    //向台号界面发送台号
    connect(this,SIGNAL(signalSendTableId(QString)),
            m_pOrderWidget,SLOT(slotReceiveTableId(QString)));
    connect(this,SIGNAL(signalEmptyMenuTemp()),
            m_pOrderWidget,SLOT(slotClearMenuInfo()));

    connect(m_pOrderWidget,SIGNAL(signalRestartOrderMenu()),
            this,SLOT(slotChangeTableIdInfo()));
    connect(this,SIGNAL(signalSubmitOrderOk()),
            m_pOrderWidget,SLOT(slotSubmitOrderOk()));

    //菜单翻页显示关联
    connect(m_pPbnPrevious,SIGNAL(clicked(bool)),
            this,SLOT(slotShowPreviousMenu()));
    connect(m_pPbnNext,SIGNAL(clicked(bool)),
            this,SLOT(slotShowNextMenu()));
}

//从数据库中加载菜谱信息:菜名，价格
void MenuWidget::initMenuInfo()
{
    QSqlQuery query;
    QString sql = "SELECT [menu_name],[menu_price] FROM [menu_info]";
    query.exec(sql);
    while (query.next())
    {
        m_textList <<query.value(0).toString();
        m_priceList <<query.value(1).toString();
    }
}

//清除layout中的内容
void MenuWidget::removeLayout()
{
    while (m_pMainLayout->count() > 0)
    {
        QWidget *w = m_pMainLayout->itemAt(0)->widget();
        m_pMainLayout->removeWidget(w);
        delete w;
    }
}

void MenuWidget::slotShowNextMenu()
{
    removeLayout();

    if (current_page == 1)
    {
        for (int i = 9; i < 18;i++)
        {
            AbstractMenuWidget *w = new AbstractMenuWidget(m_iconList.at(i),
                                                           m_textList.at(i),
                                                           m_priceList.at(i));
            m_menu_list.append(w);
            connect(w,SIGNAL(signalAddMenu()),m_PSignalMapper,SLOT(map()));
            m_PSignalMapper->setMapping(w,m_textList.at(i));
            m_pMainLayout->addWidget(w,i/PER_ROW_COUNT,i%PER_COLUMN_COUNT);
        }
        current_page = 2;
        m_pPbnPrevious->setEnabled(true);
    }
    else if (current_page == 2)
    {
        for (int i = 18; i < 27;i++)
        {
            AbstractMenuWidget *w = new AbstractMenuWidget(m_iconList.at(i),
                                                           m_textList.at(i),
                                                           m_priceList.at(i));
            m_menu_list.append(w);
            connect(w,SIGNAL(signalAddMenu()),m_PSignalMapper,SLOT(map()));
            m_PSignalMapper->setMapping(w,m_textList.at(i));
            m_pMainLayout->addWidget(w,i/PER_ROW_COUNT,i%PER_COLUMN_COUNT);
        }
        current_page = 3;
    }
    if (current_page == 3)
    {
        m_pPbnNext->setEnabled(false);
    }else
    {
        m_pPbnNext->setEnabled(true);
    }
    m_pLabelCurrentPage->setText(QString("当前第 %1 页").arg(QString::number(current_page)));
}

void MenuWidget::slotShowPreviousMenu()
{
    removeLayout();

    if (current_page == 2)
    {
        for (int i = 0; i < 9;i++)
        {
            AbstractMenuWidget *w = new AbstractMenuWidget(m_iconList.at(i),
                                                           m_textList.at(i),
                                                           m_priceList.at(i));
            m_menu_list.append(w);
            connect(w,SIGNAL(signalAddMenu()),m_PSignalMapper,SLOT(map()));
            m_PSignalMapper->setMapping(w,m_textList.at(i));
            m_pMainLayout->addWidget(w,i/PER_ROW_COUNT,i%PER_COLUMN_COUNT);
        }
        current_page = 1;
        m_pPbnPrevious->setEnabled(true);
    }
    else if (current_page == 3)
    {
        for (int i = 9; i < 18;i++)
        {
            AbstractMenuWidget *w = new AbstractMenuWidget(m_iconList.at(i),
                                                           m_textList.at(i),
                                                           m_priceList.at(i));
            m_menu_list.append(w);
            connect(w,SIGNAL(signalAddMenu()),m_PSignalMapper,SLOT(map()));
            m_PSignalMapper->setMapping(w,m_textList.at(i));
            m_pMainLayout->addWidget(w,i/PER_ROW_COUNT,i%PER_COLUMN_COUNT);
        }
        current_page = 2;
    }
    if (current_page == 1)
    {
        m_pPbnPrevious->setEnabled(false);
        m_pPbnNext->setEnabled(true);
    }else
    {
        m_pPbnPrevious->setEnabled(true);
    }

    m_pLabelCurrentPage->setText(QString("当前第 %1 页").arg(QString::number(current_page)));
}

//获取所选择的菜单，添加到左边的列表中
void MenuWidget::slotAddMenu(const QString &name)
{
    if (g_isConnect)
    {
        if (g_isCheckTable)
        {
            //发送菜单编号给服务器,先获取菜名对应的ID
            QString sql = "SELECT [menu_price] FROM [menu_info] WHERE [menu_name]='"+name+"'";
            QString menu_price = m_pSqlHelp->getValue(sql);
            //发送给左边订单列表中，显示当前菜单和对应的价格
            emit signalSendMenuInfo(name+"#"+menu_price);
            //这里取消直接发送给服务器的方式，而是等顾客点餐完毕后按下确认按钮后 一起打包发送
            //emit signalSendToServiceMenuInfo(name+"#"+menu_price);
        }else
        {
            myHelper::ShowMessageBoxError("您还没有选台，请选台后在选菜!");
            return;
        }
    }else
    {
        myHelper::ShowMessageBoxError("和服务器连接失败！\n请重新连接服务器后再选菜!");
        return;
    }

}

void MenuWidget::slotReceiveTableId(const QString &tableId)
{
    emit signalSendTableId(tableId);
    m_pLabelTableId->setText("当前您选择的台号是:"+tableId+"号");
}

void MenuWidget::slotChangeTableIdInfo()
{
    m_pLabelTableId->setText("请重新开台");
}

