#include "tablewidget.h"
#include "ui_tablewidget.h"
#include <tablenumbutton.h>
#include "myhelper.h"
#include "sqlhelp.h"

#include <QGridLayout>
#include <QSqlQuery>
#include <QSignalMapper>

extern bool g_isConnect;
bool g_isCheckTable;

TableWidget::TableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableWidget)
{
    ui->setupUi(this);

    initForm();
    initWidget();
    initConnect();
}

TableWidget::~TableWidget()
{
    delete ui;
}

void TableWidget::initForm()
{
    m_tableCount = 0;
    g_isCheckTable = false;
}

void TableWidget::initWidget()
{
    m_PSqlHelp = new SqlHelp;
    setTableNumber();           //读取数据库中餐桌号

    m_pMainGridLayout = new QGridLayout(this);
    m_pSignalMapper = new QSignalMapper;

    for (int i = 0; i < m_tableCount;i++)
    {
        tableNumButton *pbn = new tableNumButton(m_tableNumList.at(i));
        m_btnMap.insert(pbn,m_tableNumList.at(i));
        connect(pbn,SIGNAL(signalAddTable()),m_pSignalMapper,SLOT(map()));
        m_pSignalMapper->setMapping(pbn,m_tableNumList.at(i));
        m_pMainGridLayout->addWidget(pbn,i/8,i%8);
    }
    connect(m_pSignalMapper,SIGNAL(mapped(QString)),
            this,SLOT(slotGetClickTableNum(QString)));

    setLayout(m_pMainGridLayout);
}

void TableWidget::initConnect()
{
}

//读取数据库，获取餐桌数目和对应的编号
void TableWidget::setTableNumber()
{
    QString sql = "SELECT table_id FROM table_info";
    m_tableNumList = m_PSqlHelp->GetTableColumnValue(sql);
    m_tableCount = m_tableNumList.count();
}

void TableWidget::slotGetClickTableNum(const QString &num)
{
    if (g_isConnect)
    {
        g_isCheckTable = true;
        m_currTableID = num;
        emit signalSendTableId(num);

        //设置当前已经选中的台号样式
        m_btnMap.key(num)->setEnabled(false);
        m_btnMap.key(num)->setStyleSheet("color:red");

        qDebug()<<"当前选中的是:" <<num <<"台";
    }else
    {
        g_isCheckTable = false;
        myHelper::ShowMessageBoxError("和服务器连接失败!\n请重新连接服务器后再选台!");
        return;
    }
}

//当当前台 结账后，恢复为可以点击
void TableWidget::slotSetTableEnable()
{
    if (!m_currTableID.isEmpty())
    {
        m_btnMap.key(m_currTableID)->setEnabled(true);
        m_btnMap.key(m_currTableID)->setStyleSheet("color:#FFFFFF");
    }
}
