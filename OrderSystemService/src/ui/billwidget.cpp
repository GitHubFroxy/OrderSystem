/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-18
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:基于Qt的点餐系统的设计----账单查询
Function:
**************************************************/

#include "billwidget.h"
#include "ui_billwidget.h"
#include "sqlhelp.h"
#include "myhelper.h"
#include <QSqlQueryModel>
#include <QtDebug>
#include <QSqlRecord>
#include <QDateTime>

BillWidget::BillWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BillWidget)
{
    ui->setupUi(this);

    initForm();
}

BillWidget::~BillWidget()
{
    delete ui;
}

void BillWidget::initForm()
{
    m_pSqlHelp = new SqlHelp;

    ui->lineEdit_totalPrice->setEnabled(false);

    ColumnNamesTable[0] = "台号";
    ColumnNamesTable[1] = "菜名";
    ColumnNamesTable[2] = "价格";
    ColumnNamesTable[3] = "数量";
    ColumnNamesTable[4] = "总价";
    ColumnNamesTable[5] = "备注";

    ColumnWidthsTable[0] = 80;
    ColumnWidthsTable[1] = 120;
    ColumnWidthsTable[2] = 80;
    ColumnWidthsTable[3] = 100;
    ColumnWidthsTable[4] = 100;
    ColumnWidthsTable[5] = 160;

    QString sql = "SELECT * FROM [temp_order]";
    initData(sql,ColumnNamesTable,ColumnWidthsTable);

}

void BillWidget::initData(QString sql,QString columnNames[], int columnWidths[])
{
    model = new QSqlQueryModel(this);
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

void BillWidget::refreshData(const QString &sql)
{
    model->setQuery(sql);
    ui->tableView->setModel(model);
}

//结账
void BillWidget::on_pbnAccount_clicked()
{
    QString pay = ui->lineEdit_pay->text();
    QString total = ui->lineEdit_totalPrice->text();
    QString currentdate =QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString srandId = QString::number(qrand()%1000);
    QString sql;
    if(pay.toDouble() < total.toDouble())
    {
        myHelper::ShowMessageBoxError("实际收款【"+pay + "】小于应收款【" + total+"】，不允许结账!");
    }else{
        if (!pay.isEmpty())
        {
            if (myHelper::ShowMessageBoxQuesion("你确定要结账吗?"))
            {
                sql = "INSERT INTO [bill_data] VALUES('"+srandId.append(currentdate.right(8))+"',";
                sql += "'"+m_currTableId+"',"+total+",'"+currentdate+"')";
                qDebug()<<sql;
                if (m_pSqlHelp->ExecuteSql(sql))
                {
                    //结账成功后，从当前列表中删除对应的记录
                    //sql = "DELETE FROM [temp_order] WHERE [table_id]='"+m_currTableId+"'";
                    sql = "DELETE FROM [temp_order];";
                    if (m_pSqlHelp->ExecuteSql(sql))
                    {
                        qDebug()<<"删除账单信息成功!";
                    }
                    //发射信号，给历史账单界面，作为显示
                    emit signalSendRefreshStatisticInfo();

                    sql = "SELECT * FROM [temp_order]";
                    initData(sql,ColumnNamesTable,ColumnWidthsTable);
                    emit signalSendBillInfo(m_currTableId);
                    myHelper::ShowMessageBoxInfo("结账成功!");

                    ui->lineEdit_pay->clear();
                    ui->lineEdit_totalPrice->clear();

                    qDebug()<<"结账成功!";
                }
            }else
            {
                myHelper::ShowMessageBoxError("结账失败");
                qDebug()<<"结账失败!";
            }
        }
    }
}


//条件查找显示
void BillWidget::on_pbnFind_clicked()
{
    QString text = ui->lineEdit_find->text().trimmed();
    QString sql = "SELECT *FROM [temp_order] WHERE 1 = 1";
    if (!text.isEmpty())
    {
        sql += " AND table_id='"+text+"'";

    }else
    {
        sql += "";
    }

    refreshData(sql);
    //initData(sql,ColumnNamesTable,ColumnWidthsTable);
}

void BillWidget::on_tableView_clicked(const QModelIndex &index)
{
    QString table_id = model->record(index.row()).value(0).toString();
    m_currTableId = table_id;
    QString sql = "SELECT SUM(menu_sum) FROM temp_order WHERE table_id='"+table_id+"'";
    ui->lineEdit_totalPrice->setText(m_pSqlHelp->GetUserInfo(sql));
}

void BillWidget::on_lineEdit_find_textChanged(const QString &arg1)
{
    QString pay = ui->lineEdit_pay->text();
    qDebug()<<arg1;
}

//显示要结账的顾客订单详情
void BillWidget::slotReceiveTableId(const QString &tableId)
{
    QString sql = "SELECT *FROM [temp_order] WHERE table_id='"+tableId+"'";
    refreshData(sql);
}
