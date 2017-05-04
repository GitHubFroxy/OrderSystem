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
#ifndef EVALUATEDIALOG_H
#define EVALUATEDIALOG_H

#include <QDialog>

namespace Ui {
class EvaluateDialog;
}

class EvaluateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EvaluateDialog(QWidget *parent = 0);
    ~EvaluateDialog();

private:
    void initForm();

private slots:
    void on_pbnOk_clicked();

    void on_btnMenu_Close_clicked();

    void on_pbn1_clicked();

    void on_pbn2_clicked();

    void on_pbn3_clicked();

    void on_pbn4_clicked();

    void on_pbn5_clicked();

private:
    Ui::EvaluateDialog *ui;

    static int m_starCount;         //星级数目
};

#endif // EVALUATEDIALOG_H
