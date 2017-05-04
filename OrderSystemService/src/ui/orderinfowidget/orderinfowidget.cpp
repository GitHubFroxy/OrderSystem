/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-16
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:点餐系统服务器---客户点餐订单
Function:列出了所有顾客当天所点的菜单，未结账
**************************************************/
#include "orderinfowidget.h"
#include "ui_orderinfowidget.h"
#include <QDebug>
#include "sqlhelp.h"
#include "myhelper.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

OrderInfoWidget::OrderInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderInfoWidget)
{
    ui->setupUi(this);

    init();

    initTempTable();

    initForm();

    initConnect();
}

OrderInfoWidget::~OrderInfoWidget()
{
    delete ui;
}

void OrderInfoWidget::init()
{
    m_pSqlHelp = new SqlHelp;
}

void OrderInfoWidget::initForm()
{
#if 0
    /**************************台号样式设置*******************/
    ui->tableWidget_order_id->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_order_id->setShowGrid(true);
    ui->tableWidget_order_id->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置表头字体加粗
    QFont font = ui->tableWidget_order_id->horizontalHeader()->font();
    font.setBold(true);
    ui->tableWidget_order_id->horizontalHeader()->setFont(font);
    ui->tableWidget_order_id->verticalHeader()->setVisible(false);
    //设置表头宽度
    ui->tableWidget_order_id->horizontalHeader()->setFixedHeight(40);
    //单行选中
    ui->tableWidget_order_id->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_order_id->setColumnCount(1);
    ui->tableWidget_order_id->setColumnWidth(0,120);
#endif

#if 0
    /*****************菜单样式设置********************/
    ui->tableWidget_menu->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_menu->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_menu->setShowGrid(true);
    ui->tableWidget_menu->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置表头字体加粗
    QFont font2 = ui->tableWidget_menu->horizontalHeader()->font();
    font2.setBold(true);
    ui->tableWidget_menu->horizontalHeader()->setFont(font2);
    ui->tableWidget_menu->verticalHeader()->setVisible(false);
    //设置表头宽度
    ui->tableWidget_menu->horizontalHeader()->setFixedHeight(40);
    //单行选中
    ui->tableWidget_menu->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_menu->setColumnCount(3);
    ui->tableWidget_menu->setColumnWidth(0,120);
    ui->tableWidget_menu->setColumnWidth(1,90);
    ui->tableWidget_menu->setColumnWidth(2,90);
#endif

    ui->tableView_order_id->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView_menu->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->label_table_id->setText("");
    ui->label->setText("所选菜谱信息");

    ColumnNames[0] = "菜名";
    ColumnNames[1] = "价格";
    ColumnNames[2] = "数量";
    ColumnNames[3] = "合计";
    ColumnNames[4] = "备注";

    ColumnWidths[0] = 120;
    ColumnWidths[1] = 80;
    ColumnWidths[2] = 100;
    ColumnWidths[3] = 80;
    ColumnWidths[4] = 180;

    ColumnNamesTable[0] = "台号";
    ColumnWidthsTable[0] = 120;

    //refreshTableInfo();
    loadTableInfo();
    refreshMenuInfo();
}

//初始化订单界面
void OrderInfoWidget::initTempTable()
{
    QString sql = "DELETE FROM [temp_order]";
    if (!m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"订单界面初始化失败!";
    }else
    {
        sql = "DELETE FROM [temp_table]";
        m_pSqlHelp->ExecuteSql(sql);
    }
}

void OrderInfoWidget::initConnect()
{
}

bool OrderInfoWidget::valueIsExist(const QString &name)
{
    bool ok = false;
    QString sql = "SELECT 1 FROM temp_order WHERE menu_name = '"+name+"'";
    sql +=" AND [table_id] ='" + m_currTableId + "' limit 1";
    QSqlQuery query;
    query.exec(sql);
    while(query.next())
    {
        if (query.isValid())
            ok = true;
        else
            ok = false;
    }
    return ok;
}

void OrderInfoWidget::insertTableInfo(const QString &tableId)
{
    QString sql = "INSERT INTO [temp_table] VALUES('"+tableId+"')";
    if(!m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"插入台号数据失败!";
    }else
    {
        qDebug()<<"插入台号数据成功!";
        refreshTableInfo();
    }
}

void OrderInfoWidget::loadTableInfo()
{
    m_PModel = new QSqlQueryModel(this);
    QString sql = "SELECT *FROM [temp_table]";
    m_PModel->setQuery(sql);
    ui->tableView_order_id->setModel(m_PModel);

    //依次设置列标题、列宽等
    for (int i = 0;i < ui->tableView_order_id->model()->columnCount();i++)
    {
        m_PModel->setHeaderData(i,Qt::Horizontal,ColumnNamesTable[i]);//设置列标题
        ui->tableView_order_id->setColumnWidth(i,ColumnWidthsTable[i]);//设置列宽
    }

    ui->tableView_order_id->verticalHeader()->setVisible(false);
    ui->tableView_order_id->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮
    ui->tableView_order_id->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式单行选中
    ui->tableView_order_id->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    ui->tableView_order_id->setStyleSheet( "QTableView::item:hover{background-color:rgb(98,188,214,60)}"
                                           "QTableView::item:selected{background-color:#0997C0}");
}

void OrderInfoWidget::refreshMenuInfo()
{
    QString sql = "SELECT [menu_name],[menu_price],[menu_num],";
    sql += "[menu_sum],[menu_remark] FROM [temp_order] WHERE 1 = 1";
    sql +=" AND table_id = '"+m_currTableId+"'";
    m_pSqlHelp->BindTableWhere(sql,ui->tableView_menu,ColumnNames,ColumnWidths);
}

void OrderInfoWidget::refreshTableInfo()
{
    //m_pSqlHelp->BindTable("temp_table",ui->tableView_order_id,ColumnNamesTable,ColumnWidthsTable);
    m_PModel->setQuery("SELECT *FROM temp_table");
    ui->tableView_order_id->setModel(m_PModel);
}

void OrderInfoWidget::slotChangeMenuState(const QString &tableId,
                                          const QString menuName,
                                          const QString &state)
{
    QString sql = "UPDATE [temp_order] SET [menu_remark]='"+state+"' WHERE [table_id]='";
    sql +=tableId+"' AND [menu_name]='"+menuName+"'";
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"更改菜的状态成功!";
    }else
    {
        qDebug()<<"更改菜的状态失败!";
    }
    //刷新显示
    refreshMenuInfo();
}

//结账后，清除对应的台号
void OrderInfoWidget::slotDeleteTableInfo(const QString &tableId)
{
    QString sql = "DELETE FROM [temp_table] WHERE [table_id]='"+tableId+"'";
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"删除对应的台号信息成功!";
        //刷新显示
        refreshTableInfo();
        refreshMenuInfo();
    }else
    {
        qDebug()<<"删除对应的台号信息失败!";
    }
}

/*
 * 处理催菜信息
 * #台号#
*/
void OrderInfoWidget::slotReceiveLackMenuInfo(const QString &info)
{
    QStringList temp = info.split("#");
    QString sql = "DELETE FROM  [temp_order] WHERE [table_id]='"+temp.at(1)+"'";

    qDebug()<<sql;
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"催菜时，删除该菜成功!";

        //刷新显示
        refreshMenuInfo();
    }else
    {
        qDebug()<<"催菜时，删除该菜失败!";
    }
}

//接收订单数据包，并进行解析，显示在tableview中
void OrderInfoWidget::slotReceiveMenuPageInfo(const QString &info)
{
    qDebug()<<"\n服务器订单界面接收到的订单数据包:"<<info<<endl;
    m_orderMenuPageInfo = info;
    QString id = info.split("#").at(2);
    m_currTableId = id;
    //插入台号表中
    insertTableInfo(id);

    QStringList data = info.mid(4).split(",");
    qDebug()<<"分割后数据:"<<data;
    QStringList temp;
    int orderCount = data.size();       //获取记录数
    QSqlQuery query;
    query.prepare("INSERT INTO temp_order VALUES(?,?,?,?,?,?);");
    QVariantList tableId,name,price,num,sum,remark;
    for (int i = 0; i < orderCount; i++)
    {
        temp = data.at(i).split("#");
        tableId <<id;
        name <<temp.at(1);
        num <<temp.at(2);
        price <<temp.at(3);
        sum << num.at(i).toDouble() *price.at(i).toDouble();
        remark<<"";
    }
    query.addBindValue(tableId);
    query.addBindValue(name);
    query.addBindValue(price);
    query.addBindValue(num);
    query.addBindValue(sum);
    query.addBindValue(remark);

    if (!query.execBatch())
    {
        qDebug()<<query.lastError();
    }else
    {
        refreshMenuInfo();
        //数据插入成功后，通知手持端，点餐成功!
        emit signalReceiveMenuPageOk();
        qDebug()<<"插入数据成功!";
    }
}

//台号信息
void OrderInfoWidget::slotUpdateTableInfo(const QString &id)
{
    //插入数据
    insertTableInfo(id);
    //刷新界面
    refreshTableInfo();

    m_currTableId = id;     //更新当前台号信息
    ui->label_table_id->setText(id);

    qDebug()<<"receive table id is:"<<id;
}

//接收手持终端发送过来的订单信息
//#特殊编号#菜名#价格#数量#备注#
void OrderInfoWidget::slotUpdataMenuInfo(const QStringList &menuinfo)
{
    //m_menuIdList << menuId;
    //m_CurMenuId = menuinfo.at(1);
    QString menu_name = menuinfo.at(2);
    QString menu_price = menuinfo.at(3);
    QString menu_num = menuinfo.at(4);
    QString menu_remark = menuinfo.at(5);

    double menu_sum = menu_price.toDouble()*menu_num.toDouble();

    if (valueIsExist(menu_name))
    {
        qDebug()<<"改菜单存在!";
        //进行数量的更新
        QString sql = "UPDATE [temp_order] SET [menu_num]=(";
        sql +="SELECT [menu_num] FROM [temp_order] WHERE [menu_name]='"+menu_name+"') +1";
        sql +=" WHERE [menu_name]='"+menu_name+"'";
        if (!m_pSqlHelp->ExecuteSql(sql))
        {
            qDebug()<<"更新数据失败!";
        }else
        {
            qDebug()<<"更新数据成功!";
        }
    }else
    {
        qDebug()<<"菜单不存在!";
        QString sql ="INSERT INTO [temp_order] VALUES('"+m_currTableId+"','"+menu_name+"',";
        sql += menu_price+","+menu_num+","+QString::number(menu_sum)+",'"+menu_remark+"')";
        if (!m_pSqlHelp->ExecuteSql(sql))
        {
            qDebug()<<"insert data faild!";
        }
    }
    qDebug()<<"receive menu name is:"<<menu_name;
    qDebug()<<"receive menu price is:"<<menu_price;
    qDebug()<<"receive menu_num is :"<<menu_num;
    qDebug()<<"receive menu remark is:"<<menu_remark;

    //刷新菜单信息
    refreshMenuInfo();
}

void OrderInfoWidget::on_tableView_order_id_clicked(const QModelIndex &index)
{
    QString tableId =  index.data(Qt::DisplayRole).toString();
    qDebug()<<tableId;
    m_currTableId = tableId;

    ui->label_table_id->setText(m_currTableId);
    refreshMenuInfo();
}

void OrderInfoWidget::on_tableView_order_id_customContextMenuRequested(const QPoint &pos)
{
    QPoint pt = ui->tableView_order_id->mapFromGlobal(pos);
    int height = ui->tableView_order_id->horizontalHeader()->height();
    int width = ui->tableView_order_id->verticalHeader()->width();
    QPoint pt2(width,height);
    pt += pt2;

    qDebug()<<ui->tableView_order_id->indexAt(pt).row();
}

//将从手持终端  中接收到的菜单数据包发送给厨房
void OrderInfoWidget::on_pbnSendKitchen_clicked()
{
    if (ui->tableView_menu->currentIndex().row() >= 0)
    {
        emit signalSendToKitchenMenuPageInfo();
    }else
    {
        myHelper::ShowMessageBoxError("当前订单为空，无法发送!");
        return;
    }
}
