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
#ifndef STATISTICWIDGET_H
#define STATISTICWIDGET_H

#include <QWidget>
class SqlHelp;

namespace Ui {
class StatisticWidget;
}

class StatisticWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticWidget(QWidget *parent = 0);
    ~StatisticWidget();

private slots:
    void slotRefreshInfo();

    void on_pbnFind_clicked();

    void on_checkBox_time_toggled(bool checked);

    void on_checkBox_tableId_toggled(bool checked);

private:
    void initForm();
    void initConect();

private:
    Ui::StatisticWidget *ui;

    SqlHelp *m_pSqlhelp;                        //数据库操作对象
    QString ColumnNames[4];                    //列名数组声明
    int ColumnWidths[4];                       //列宽数组声明
};

#endif // STATISTICWIDGET_H
