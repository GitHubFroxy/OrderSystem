/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-16
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:菜谱信息界面
Function:
**************************************************/
#include "menuinfowidget.h"
#include "ui_menuinfowidget.h"
#include "sqlhelp.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "myhelper.h"
#include "protocol.h"

MenuInfoWidget::MenuInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuInfoWidget)
{
    ui->setupUi(this);

    initForm();
    init();
    initConnect();
}

MenuInfoWidget::~MenuInfoWidget()
{
    delete ui;
}

void MenuInfoWidget::initForm()
{
    ui->tableView->verticalHeader()->setVisible(false);

    ui->lineEdit_id->setEnabled(false);
    ui->lineEdit_name->setEnabled(false);
    ui->lineEdit_price->setEnabled(false);
    ui->lineEdit_remark->setEnabled(false);

    m_isAdd = false;
    m_isModify = false;
    ui->pbnSave->setEnabled(false);

    m_pSqlHelp = new SqlHelp;
}

void MenuInfoWidget::init()
{

    ColumnNames[0] = "编号";
    ColumnNames[1] = "菜名";
    ColumnNames[2] = "价格";
    ColumnNames[3] = "备注";

    ColumnWidths[0] = 80;
    ColumnWidths[1] = 140;
    ColumnWidths[2] = 100;
    ColumnWidths[3] = 150;

    //m_pSqlHelp->BindTable("menu_info",ui->tableView,ColumnNames,ColumnWidths);
    initData("menu_info",ColumnNames,ColumnWidths);
}

void MenuInfoWidget::initConnect()
{

}

void MenuInfoWidget::initData(QString tableName,QString columnNames[],
                              int columnWidths[])
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

    ui->tableView->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式单行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    ui->tableView->setStyleSheet( "QTableView::item:hover{background-color:rgb(98,188,214,60)}"
                                  "QTableView::item:selected{background-color:#0997C0}");
}

void MenuInfoWidget::refreshMenuInfo()
{
    initData("menu_info",ColumnNames,ColumnWidths);

    ui->lineEdit_id->setEnabled(false);
    ui->lineEdit_name->setEnabled(false);
    ui->lineEdit_price->setEnabled(false);
    ui->lineEdit_remark->setEnabled(false);
}

//添加菜谱
void MenuInfoWidget::on_pbnAdd_clicked()
{
    ui->lineEdit_id->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_price->clear();
    ui->lineEdit_remark->clear();

    qDebug()<<m_pSqlHelp->GetTableMaxId("menu_info","menu_id");

    //ui->lineEdit_id->setText(QString::number((m_pSqlHelp->GetTableMaxId("menu_info","menu_id").toInt() + 1)));
    ui->pbnAdd->setEnabled(false);
    ui->pbnSave->setEnabled(true);

    ui->lineEdit_name->setEnabled(true);
    ui->lineEdit_price->setEnabled(true);
    ui->lineEdit_remark->setEnabled(true);

    m_isAdd = true;
}

//删除菜谱
void MenuInfoWidget::on_pbnDelete_clicked()
{
    QString id  = ui->lineEdit_id->text();
    QString sql = "DELETE FROM [menu_info] WHERE [menu_id] ='"+id+"'";

    if (myHelper::ShowMessageBoxQuesion("你确定要删除该信息吗?"))
    {
        if (m_pSqlHelp->ExecuteSql(sql))
        {
            QString data = "#"+QString::number(DELETE_MENU_INFO)+"#"+id+"#";
            emit signalSendChangeMenuInfo(data);
            myHelper::ShowMessageBoxInfo("删除成功!");
            //刷新显示
            refreshMenuInfo();
        }else
        {
            myHelper::ShowMessageBoxInfo("删除失败!");
        }
    }
}

//修改菜谱信息
void MenuInfoWidget::on_pbnModify_clicked()
{
    //编号默认不能修改
    ui->lineEdit_id->setEnabled(false);
    ui->lineEdit_name->setEnabled(true);
    ui->lineEdit_price->setEnabled(true);
    ui->lineEdit_remark->setEnabled(true);

    ui->pbnSave->setEnabled(true);
    ui->pbnModify->setEnabled(false);
    m_isModify = true;
}

void MenuInfoWidget::on_tableView_clicked(const QModelIndex &index)
{
    QString id = model->record(index.row()).value(0).toString();
    QString name = model->record(index.row()).value(1).toString();
    QString price = model->record(index.row()).value(2).toString();

    ui->lineEdit_id->setText(id);
    ui->lineEdit_name->setText(name);
    ui->lineEdit_price->setText(price);

    ui->pbnDelete->setEnabled(true);
}

//保存数据
void MenuInfoWidget::on_pbnSave_clicked()
{
    QString id = ui->lineEdit_id->text();
    QString name = ui->lineEdit_name->text();
    QString price = ui->lineEdit_price->text();
    QString remark = ui->lineEdit_remark->text();
    QString sql;

    if (m_isAdd)
    {
        sql = "INSERT INTO [menu_info] VALUES((SELECT MAX(menu_id)+1 FROM [menu_info]),";
        sql +="'"+name+"','"+price+"','"+remark+"')";
        if (m_pSqlHelp->ExecuteSql(sql))
        {
            QString data = "#"+QString::number(ADD_NEW_MENU_INFO)+"#";
            data +=id+"#"+name+"#"+price+"#"+remark+"#";
            emit signalSendChangeMenuInfo(data);
            myHelper::ShowMessageBoxInfo("添加菜谱成功!");

            //刷新显示
            refreshMenuInfo();
        }else
        {
            myHelper::ShowMessageBoxError("添加菜谱失败!");
            return;
        }
        m_isAdd = false;
    }else if (m_isModify)
    {
        sql = "UPDATE [menu_info] SET [menu_name]='"+name+"',[menu_price]='"+price+"',";
        sql +="[menu_remark]='"+remark+"' WHERE [menu_id]='"+id+"'";
        if (m_pSqlHelp->ExecuteSql(sql))
        {
            QString data = "#"+QString::number(MIDIFY_MENU_INFO)+"#"+id+"#"+name+"#"+price+"#"+remark+"#";
            emit signalSendChangeMenuInfo(data);
            myHelper::ShowMessageBoxInfo("更新菜谱信息成功!");

            //刷新显示
            refreshMenuInfo();
        }else
        {
            myHelper::ShowMessageBoxError("更新菜谱信息失败!");
            return;
        }
        m_isModify = false;
    }

    ui->pbnAdd->setEnabled(true);
    ui->pbnDelete->setEnabled(true);
    ui->pbnModify->setEnabled(true);
    ui->pbnSave->setEnabled(false);
    qDebug()<<sql;
}


//当此编辑框编辑完成时，我们这里自动加载菜谱编号的最大值
void MenuInfoWidget::on_lineEdit_name_editingFinished()
{
    QString sql = "SELECT MAX(menu_id)+1 FROM [menu_info]";
    ui->lineEdit_id->setText(m_pSqlHelp->GetUserInfo(sql));
}
