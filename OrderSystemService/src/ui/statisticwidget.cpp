/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-18
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:基于Qt的点餐系统的设计----历史账单统计实现
Function:
**************************************************/
#include "statisticwidget.h"
#include "ui_statisticwidget.h"
#include "sqlhelp.h"
#include  "myhelper.h"
#include <QDateTime>

StatisticWidget::StatisticWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticWidget)
{
    ui->setupUi(this);

    initForm();
    initConect();
}

StatisticWidget::~StatisticWidget()
{
    delete ui;
}

//刷新界面显示
void StatisticWidget::slotRefreshInfo()
{
    m_pSqlhelp->SetControlTable(ui->tableView,ui->labInfo,ui->btnFirst,ui->btnPre,ui->btnNext,ui->btnLast);
    m_pSqlhelp->BindData("bill_data","bill_id","",11,ColumnNames,ColumnWidths);

    m_pSqlhelp->BindDataToCombox("bill_data","table_id",ui->comboBox_tableId);
}

void StatisticWidget::initForm()
{
    m_pSqlhelp = new SqlHelp;

    ui->dateTimeEdit_from->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_to->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_from->setEnabled(false);
    ui->dateTimeEdit_to->setEnabled(false);
    ui->comboBox_tableId->setEditable(false);
    ui->comboBox_tableId->setDisabled(true);

    ColumnNames[0] = "流水号";
    ColumnNames[1] = "台号";
    ColumnNames[2] = "金额";
    ColumnNames[3] = "日期";

    ColumnWidths[0] = 140;
    ColumnWidths[1] = 100;
    ColumnWidths[2] = 100;
    ColumnWidths[3] = 220;

    m_pSqlhelp->SetControlTable(ui->tableView,ui->labInfo,ui->btnFirst,ui->btnPre,ui->btnNext,ui->btnLast);
    m_pSqlhelp->BindData("bill_data","bill_id","",11,ColumnNames,ColumnWidths);

    m_pSqlhelp->BindDataToCombox("bill_data","table_id",ui->comboBox_tableId);
}

void StatisticWidget::initConect()
{

}

//组织查询语句，进行显示
void StatisticWidget::on_pbnFind_clicked()
{
    QString sql = "SELECT * FROM [bill_data] WHERE 1 = 1";
    if (ui->checkBox_time->isChecked())
    {
        sql += " AND [bill_date] >= '" + ui->dateTimeEdit_from->dateTime().toString("yyyyMMddhhmss")+"'";
        sql +=" AND [bill_date] <= '" + ui->dateTimeEdit_to->dateTime().toString("yyyyMMddhhmss")+"'";
    }else if (ui->checkBox_tableId->isCheckable())
    {
        {
            sql += " AND table_id = '"+ui->comboBox_tableId->currentText()+"'";
        }
    }

    qDebug()<<sql;

    m_pSqlhelp->BindTableWhere(sql,ui->tableView,ColumnNames,ColumnWidths);

}

void StatisticWidget::on_checkBox_time_toggled(bool checked)
{
    ui->dateTimeEdit_from->setEnabled(checked);
    ui->dateTimeEdit_to->setEnabled(checked);
}
void StatisticWidget::on_checkBox_tableId_toggled(bool checked)
{
    ui->comboBox_tableId->setDisabled(!checked);
}

