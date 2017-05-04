/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-18
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:基于Qt的点餐系统的设计----账号信息管理
Function:
**************************************************/
#include "acountmanagewidget.h"
#include "ui_acountmanagewidget.h"
#include <QSqlQuery>
#include "sqlhelp.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include "myhelper.h"

AcountManageWidget::AcountManageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AcountManageWidget)
{
    ui->setupUi(this);

    initForm();
    init();
    initConnect();
}

AcountManageWidget::~AcountManageWidget()
{
    delete ui;
}

void AcountManageWidget::initForm()
{
    m_isModify = false;
    ui->pbnOk->setEnabled(false);
    ui->comboBox_role->setEditable(false);
    ui->comboBox_role->setDisabled(true);
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_r_pwd->setEchoMode(QLineEdit::Password);

    ui->lineEdit_name->setEnabled(false);
    ui->lineEdit_pwd->setEnabled(false);
    ui->lineEdit_r_pwd->setEnabled(false);
}

void AcountManageWidget::init()
{
    m_pSqlHelp = new SqlHelp;

    ColumnNames[0] = "编号";
    ColumnNames[1] = "用户名";
    ColumnNames[2] = "密码";
    ColumnNames[3] = "角色";

    ColumnWidths[0] = 70;
    ColumnWidths[1] = 140;
    ColumnWidths[2] = 120;
    ColumnWidths[3] = 120;

    initData("v_userinfo",ColumnNames,ColumnWidths);
}

void AcountManageWidget::initConnect()
{
}

void AcountManageWidget::initData(QString tableName, QString columnNames[], int columnWidths[])
{
    m_model = new QSqlQueryModel(this);
    QString sql = "SELECT * FROM " + tableName;
    m_model->setQuery(sql);
    ui->tableView->setModel(m_model);

    //依次设置列标题、列宽等
    for (int i = 0;i < ui->tableView->model()->columnCount();i++)
    {
        m_model->setHeaderData(i,Qt::Horizontal,columnNames[i]);//设置列标题
        ui->tableView->setColumnWidth(i,columnWidths[i]);//设置列宽
    }

    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式单行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    ui->tableView->setStyleSheet( "QTableView::item:hover{background-color:rgb(98,188,214,60)}"
                                  "QTableView::item:selected{background-color:#0997C0}");
}

//确认修改
void AcountManageWidget::on_pbnOk_clicked()
{
    QString name = ui->lineEdit_name->text();
    QString role ;//= ui->comboBox_role->currentText();
    QString pwd = ui->lineEdit_pwd->text();
    QString rPwd = ui->lineEdit_r_pwd->text();
    QString sql = "";

    if(ui->comboBox_role->currentIndex() == 0)
        role = "0";
    else if (ui->comboBox_role->currentIndex() == 1)
        role = "1";

    if(pwd != rPwd)
    {
        myHelper::ShowMessageBoxError("两次输入的密码不一致,请重新输入");
        return;
    }
    //修改用户账号
    if (m_isModify)
    {
        sql = "UPDATE user_info SET [user_name]='"+name+"',user_pwd='"+pwd+"'";
        sql +=" WHERE [user_id]="+m_currUserId;
        if (m_pSqlHelp->ExecuteSql(sql))
        {
            m_isModify = false;
            ui->pbnOk->setEnabled(false);
            myHelper::ShowMessageBoxInfo("修改信息成功!");
            initData("v_userinfo",ColumnNames,ColumnWidths);
            return;
        }else
        {
            myHelper::ShowMessageBoxError("修改信息失败!");
            return;
        }
    }else
    {
        //添加用户信息
        sql = "INSERT INTO [user_info] VALUES((SELECT MAX(user_id)+1 FROM user_info),";
        sql += "'"+name +"','"+pwd+"','"+role+"')";
        qDebug()<<sql;
        if (m_pSqlHelp->ExecuteSql(sql))
        {
            ui->pbnOk->setEnabled(false);
            myHelper::ShowMessageBoxInfo("添加信息成功!");
            initData("v_userinfo",ColumnNames,ColumnWidths);
            return;
        }else
        {
            myHelper::ShowMessageBoxError("添加信息失败!");
            return;
        }
    }
    ui->pbnOk->setEnabled(false);
    ui->comboBox_role->setDisabled(true);

    ui->lineEdit_name->setEnabled(false);
    ui->lineEdit_pwd->setEnabled(false);
    ui->lineEdit_r_pwd->setEnabled(false);

}

//添加
void AcountManageWidget::on_pbnAdd_clicked()
{
    ui->lineEdit_name->clear();
    ui->lineEdit_pwd->clear();
    ui->lineEdit_r_pwd->clear();
    ui->comboBox_role->setDisabled(false);

    ui->lineEdit_name->setEnabled(true);
    ui->lineEdit_pwd->setEnabled(true);
    ui->lineEdit_r_pwd->setEnabled(true);

    ui->pbnOk->setEnabled(true);
}

//修改
void AcountManageWidget::on_pbnModify_clicked()
{
    m_isModify = true;
    //默认管理员账号不能修改
    if (ui->lineEdit_name->text() == "admin")
    {
        ui->comboBox_role->setDisabled(true);
        ui->lineEdit_pwd->setEnabled(false);
        ui->lineEdit_r_pwd->setEnabled(false);
    }

    ui->lineEdit_name->setEnabled(true);
    ui->lineEdit_pwd->setEnabled(true);
    ui->lineEdit_r_pwd->setEnabled(true);
    ui->pbnOk->setEnabled(true);
}

void AcountManageWidget::on_tableView_clicked(const QModelIndex &index)
{
    QString id = m_model->record(index.row()).value(0).toString();
    QString name = m_model->record(index.row()).value(1).toString();
    QString pwd = m_model->record(index.row()).value(2).toString();
    QString role = m_model->record(index.row()).value(3).toString();

    m_currUserId = id;

    if (role == "管理员"){
        ui->comboBox_role->setCurrentIndex(0);
    }else if(role == "收银员")
        ui->comboBox_role->setCurrentIndex(1);

    ui->lineEdit_name->setText(name);
    ui->lineEdit_pwd->setText(pwd);
    ui->lineEdit_r_pwd->setText(pwd);
    ui->pbnOk->setEnabled(false);
}

void AcountManageWidget::on_lineEdit_r_pwd_editingFinished()
{
    QString pwd = ui->lineEdit_pwd->text();
    QString rPwd = ui->lineEdit_r_pwd->text();
    if (rPwd != pwd)
    {
        myHelper::ShowMessageBoxError("两次输入的密码不一致,请重新输入");
    }
}
