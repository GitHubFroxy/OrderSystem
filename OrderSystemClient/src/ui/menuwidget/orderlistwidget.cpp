/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-16
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:基于Qt的点餐系统的设计---订单临时显示列表的实现
Function:
数据格式：菜单数据打包发送，每个菜单之间用逗号隔开
比如:#2#鱼香肉丝#1#12#,#土豆丝#1#10#,#尖椒肉丝#1#10#
**************************************************/
#include "orderlistwidget.h"
#include "ui_orderlistwidget.h"
#include <QDebug>
#include "protocol.h"
#include "evaluatedialog.h"
#include "sqlhelp.h"
#include "myhelper.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlError>

extern bool g_isConnect;        //是否和服务器连接
extern bool g_isCheckTable;     //是否已经选台
bool g_isCheckMenu;             //是否已经点菜

OrderListWidget::OrderListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderListWidget)
{
    ui->setupUi(this);

    init();
    initForm();
}

OrderListWidget::~OrderListWidget()
{
    delete ui;
}

void OrderListWidget::initForm()
{
    setFixedWidth(260);

    g_isCheckMenu = false;

    ui->pbn_accounts->setEnabled(true);
    ui->pbn_backMenu->setEnabled(false);
    ui->pbn_hurryMenu->setEnabled(false);

    ColumnNames[0] = "菜名";
    ColumnNames[1] = "数量";
    ColumnNames[2] = "单价";

    ColumnWidths[0] = 100;
    ColumnWidths[1] = 60;
    ColumnWidths[2] = 63;

    initData("temp_menu",ColumnNames,ColumnWidths);

    m_isSubmitOrder = false;
}

void OrderListWidget::init()
{
    m_pEvaluateDialog = new EvaluateDialog;
    m_pSqlHelp = new SqlHelp;
}

void OrderListWidget::initData(QString tableName, QString columnNames[], int columnWidths[])
{
    model = new QSqlQueryModel(this);
    QString sql = "SELECT *FROM " + tableName;
    model->setQuery(sql);

    ui->tableView->setModel(model);

    //依次设置列标题、列宽等
    for (int i = 0;i < ui->tableView->model()->columnCount();i++)
    {
        model->setHeaderData(i,Qt::Horizontal,columnNames[i]);//设置列标题
        ui->tableView->setColumnWidth(i,columnWidths[i]);//设置列宽
    }

    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式单行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    ui->tableView->setStyleSheet( "QTableView::item:hover{background-color:rgb(98,188,214,60)}"
                                  "QTableView::item:selected{background-color:#0997C0}");
}

bool OrderListWidget::isContainValue(const QString &name)
{
    bool ok =false;
    foreach (QString str, m_menuNameList) {
        if (str.contains(name,Qt::CaseInsensitive))
            return true;
        else
            ok = false;
    }
    return ok;
}

bool OrderListWidget::valueIsExist(const QString &name)
{
    bool ok = false;
    QString sql = "SELECT 1 FROM [temp_menu] WHERE [menu_name] = '"+name+"' limit 1";
    QSqlQuery query;
    query.exec(sql);
    while (query.next())
    {
        if (query.isValid())
            ok = true;
        else
            ok = false;
    }
    return ok;
}

void OrderListWidget::refreshMenuInfo()
{
    //m_pSqlHelp->BindTable("temp_menu",ui->tableView,ColumnNames,ColumnWidths);
    model->setQuery("SELECT *FROM temp_menu");
    ui->tableView->setModel(model);
}

void OrderListWidget::deleteMenuInfo(const QString &name)
{
    QString sql = "DELETE FROM [temp_menu] WHERE [menu_name]='"+name+"'";
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"\n删除菜单 "<<name<<" 成功!\n";
    }else
    {
        qDebug()<<"\n删除菜单 "<<name<<" 失败!\n";
    }
}

//获取顾客当前的订单列表
QString OrderListWidget::getAllOrdetMenuInfo()
{
    QString sql = "SELECT * FROM [temp_menu]";
    QString temp;
    m_menuInfo.clear();
    QSqlQuery query;
    if (!query.exec(sql))
        qDebug()<<query.lastError();
    while (query.next())
    {
        temp = "#"+query.value(0).toString()+"#"+query.value(1).toString();
        temp += "#"+query.value(2).toString()+",";
        m_menuInfo.append(temp);
    }

    return m_menuInfo.replace(m_menuInfo.length()-1,1,"#");
}

/*
 * 接收从菜谱界面发送过来的菜单信息，添加到本订单列表中
 * 水煮鱼#16
 * 菜名，价格
*/
void OrderListWidget::slotReceiveMenuInfo(const QString &info)
{
    qDebug()<<"添加菜单到列表中:"<<info;
    QStringList data = info.split('#');
    QString sql;

//    //先对表进行清空，在进行插入或者更新操作
//    QString sql = "DELETE FROM [temp_menu]";
//    if (!m_pSqlHelp->ExecuteSql(sql))
//    {
//        qDebug()<<"插入数据之前清空表失败!";
//    }

    //对菜单数量进行逻辑判断
    if (valueIsExist(data.at(0)))
    {
        g_isCheckMenu = true;
        qDebug()<<"存在该菜单";
        //进行数量更新
        sql = "UPDATE [temp_menu] SET [menu_num]=(";
        sql +="SELECT [menu_num] FROM [temp_menu] WHERE [menu_name]='"+data.at(0)+"') + 1";
        sql +=" WHERE [menu_name] = '" +data.at(0) + "'";
        if (!m_pSqlHelp->ExecuteSql(sql))
        {
            qDebug()<<"更新数据失败!";
        }else
        {
            qDebug()<<"更新数据成功!";
        }
    }else
    {
        qDebug()<<"该菜单不存在!";
        g_isCheckMenu = true;//表示已经点餐了
        sql = "INSERT INTO [temp_menu] VALUES('"+data.at(0)+"',";
        sql += +"1,"+data.at(1)+")";
        qDebug()<<sql;
        if (!m_pSqlHelp->ExecuteSql(sql))
        {
            qDebug()<<"插入数据失败!";
        }else
        {
            qDebug()<<"插入数据成功!";
        }
    }
    refreshMenuInfo();
}

//服务器结账后，手持端界面清除对应临时信息
void OrderListWidget::slotClearMenuInfo()
{
    QString sql = "DELETE FROM temp_menu";
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        g_isCheckMenu = false;      //清空后表示没有点餐
        m_isSubmitOrder = false;    //
        g_isCheckTable = false;
        g_isCheckMenu = false;

        ui->pbnOk->setEnabled(true);

        qDebug()<<"清空成功!";
        //刷新显示
        refreshMenuInfo();

        emit signalRestartOrderMenu();

        //进行对刚才的消费评价
        m_pEvaluateDialog->exec();
    }else
    {
        qDebug()<<"清空失败!";
    }
}

void OrderListWidget::slotReceiveTableId(const QString &tableId)
{
    m_tableId = tableId;
}

void OrderListWidget::slotSubmitOrderOk()
{
    ui->pbnOk->setEnabled(false);
}

/*
 * 退菜
 * 数据格式:#标志#菜名#
 * 提交了订单后不能直接退菜,只能发出退菜请求，看服务器是否同意
*/
void OrderListWidget::on_pbn_backMenu_clicked()
{
    if (g_isConnect)
    {
        if (ui->tableView->currentIndex().row() >= 0)
        {
            //若是已经提交了订单，则只能发出退菜请求，若没有提交，则直接在本地进行删除即可
            if(m_isSubmitOrder)
            {
                QString data = "#"+QString::number(RETURN_MENU)+"#"+m_currMenuName+"#";
                emit signalSendToServiceBackMenu(data);
                qDebug()<<"发送退菜信息给服务器，要求退菜"<<data;
                //                //删除对应所选的菜单
                //                deleteMenuInfo(m_currMenuName);
                //                //刷新界面
                //                refreshMenuInfo();
            }else
            {
                //直接删除本地订单对应菜单信息
                if (myHelper::ShowMessageBoxQuesion("你确定要退菜吗?"))
                {
                    //删除对应所选的菜单
                    deleteMenuInfo(m_currMenuName);
                    //刷新界面
                    refreshMenuInfo();
                }else
                {
                }
            }
        }else
        {
            myHelper::ShowMessageBoxError("没有选中任何菜!");
            return;
        }

    }else
    {
        myHelper::ShowMessageBoxError("未和服务器连接，请连接服务器!");
        return;
    }
}

/*
 * 催菜
 * 格式:#标志#菜名#
*/
void OrderListWidget::on_pbn_hurryMenu_clicked()
{
    if (g_isConnect)
    {
        if (ui->tableView->currentIndex().row() >= 0)
        {
            if (m_isSubmitOrder)
            {
                QString data = "#"+QString::number(HURRY_MENU)+"#"+m_currMenuName+"#";
                emit signalSendToServiceHurryMenu(data);
                qDebug()<<"发送催菜信息给服务器，要求催菜:"<<data;
            }else
            {
                myHelper::ShowMessageBoxError("您还没有提交订单，请先提交订单!");
                return;
            }
        }else
            myHelper::ShowMessageBoxError("没有选中任何菜!");
        return;
    }else
    {
        myHelper::ShowMessageBoxError("未和服务器连接，请连接服务器!");
        return;
    }
}

//结账
void OrderListWidget::on_pbn_accounts_clicked()
{
    if (g_isConnect)
    {
        if (g_isCheckMenu)
        {
            if (m_isSubmitOrder)
            {
                if (myHelper::ShowMessageBoxQuesion("你确定要结账吗?"))
                {
                    qDebug()<<"结账按钮按下，顾客要结账:"<<m_tableId;
                    emit signalSendToServiceMenuAccount(m_tableId);
                    myHelper::ShowMessageBoxInfo("结账信息已经提交给服务器，请到前台去结账!");
                    //m_pEvaluateDialog->exec();
                }else
                {
                    //
                }
            }else
            {
                myHelper::ShowMessageBoxError("您还没有提交订单，请先提交订单!");
                return;
            }
        }else
        {
            myHelper::ShowMessageBoxError("抱歉，您还没有选菜!");
            return;
        }
    }else
    {
        myHelper::ShowMessageBoxError("未和服务器连接，请连接服务器!");
        return;
    }
}

//确认信息，向服务器发送点菜完毕信息
/*
 * 修改：现在顾客点击确认后，将顾客所选的菜单全部发送给服务器
 *      之前是添加一条发送一条
 * 发送格式：每道菜之间使用逗号隔开
 * #2#鱼香肉丝#1#12#,#土豆丝#1#10#,#尖椒肉丝#1#10#
*/
void OrderListWidget::on_pbnOk_clicked()
{
    if (g_isConnect)
    {
        if (g_isCheckMenu)
        {
            //确认完毕后才能进行结账
            //ui->pbn_accounts->setEnabled(true);
            //QString data = "#"+QString::number(ORDER_OK)+"#";
            //emit signalSendMenuOk(data);
            //slotClearMenuInfo();
            if (myHelper::ShowMessageBoxQuesion("您确定要提交吗？提交后不可更改订单信息!"))
            {
                QString data = "#"+m_tableId+getAllOrdetMenuInfo();
                emit signalSendMenuOk(data);
                m_isSubmitOrder = true;
            }else
            {
                qDebug()<<"顾客取消，不提交订单!";
            }
        }else
        {
            m_isSubmitOrder = false;
            myHelper::ShowMessageBoxError("抱歉，您还没有选菜!");
            return;
        }

    }else
    {
        myHelper::ShowMessageBoxError("未和服务器连接，请连接服务器!");
        return;
    }
}

void OrderListWidget::on_tableView_clicked(const QModelIndex &index)
{
    QString menuName = model->record(index.row()).value(0).toString();
    m_currMenuName = menuName;
    qDebug()<<menuName;
    ui->pbn_hurryMenu->setEnabled(true);
    ui->pbn_backMenu->setEnabled(true);
}
