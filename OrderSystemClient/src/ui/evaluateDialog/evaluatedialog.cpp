/*************************************************
Copyright:kevin
Author:Kevin LiQi
Date:2016-04-18
Email:kevinlq0912@163.com
QQ:936563422
Version:V1.0
Description:基于Qt点餐系统客户端---评价界面
Function:
**************************************************/
#include "evaluatedialog.h"
#include "ui_evaluatedialog.h"
#include "iconhelper.h"

//默认0星
int EvaluateDialog::m_starCount = 0;

EvaluateDialog::EvaluateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EvaluateDialog)
{
    ui->setupUi(this);
    initForm();
}

EvaluateDialog::~EvaluateDialog()
{
    delete ui;
}

void EvaluateDialog::initForm()
{
    setWindowFlags(Qt::FramelessWindowHint);

    //设置图形字体
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);

    ui->pbn1->SetCheck(false);
    ui->pbn2->SetCheck(false);
    ui->pbn3->SetCheck(false);
    ui->pbn4->SetCheck(false);
    ui->pbn5->SetCheck(false);
}

void EvaluateDialog::on_pbnOk_clicked()
{
    //判断星级
    this->close();
}

void EvaluateDialog::on_btnMenu_Close_clicked()
{
    this->close();
}

void EvaluateDialog::on_pbn1_clicked()
{
    if (ui->pbn1->GetCheck())
    {
        m_starCount++;
    }else
    {
        m_starCount--;
    }
    ui->label->setText(QString::number(m_starCount)+"星");
}

void EvaluateDialog::on_pbn2_clicked()
{
    if (ui->pbn2->GetCheck())
    {
        m_starCount++;
    }else
    {
        m_starCount--;
    }
    ui->label->setText(QString::number(m_starCount)+"星");
}

void EvaluateDialog::on_pbn3_clicked()
{
    if (ui->pbn3->GetCheck())
    {
        m_starCount++;
    }else
    {
        m_starCount--;
    }
    ui->label->setText(QString::number(m_starCount)+"星");
}

void EvaluateDialog::on_pbn4_clicked()
{
    if (ui->pbn4->GetCheck())
    {
        m_starCount++;
    }else
    {
        m_starCount--;
    }
    ui->label->setText(QString::number(m_starCount)+"星");
}

void EvaluateDialog::on_pbn5_clicked()
{
    if (ui->pbn5->GetCheck())
    {
        m_starCount++;
    }else
    {
        m_starCount--;
    }
    ui->label->setText(QString::number(m_starCount)+"星");
}
