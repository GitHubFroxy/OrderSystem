#ifndef ORDERINFOWIDGET_H
#define ORDERINFOWIDGET_H

#include <QWidget>

class SqlHelp;
class QSqlQueryModel;

namespace Ui {
class OrderinfoWidget;
}

class OrderinfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderinfoWidget(QWidget *parent = 0);
    ~OrderinfoWidget();

private:
    void initForm();
    void init();
    void initConnect();
    void initData(QString tableName, QString columnNames[], int columnWidths[]);

    void refreshMenuInfo();
    void changeMenuState(const QString &tableId,const QString menuName,
                         const QString &state);                //改变菜的状态
    //删除某道菜
    void deleteMenuInfo(const QString &);

Q_SIGNALS:
    void signalSendToServiceLackMenu(const QString &);         //发送缺菜信息给服务器
    void signalSendFinishMenu(const QString &);                //发送完菜信息

private Q_SLOTS:
    void slotReceiveMenuInfo(const QString &);
    void slotReceiveHurryMenuInfo(const QString &);             //处理催菜信息

    void slotRefreshMenuInfo();

    void on_pbnok_clicked();

    void on_pbnLackMenu_clicked();

    void on_pbnChangeState_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_comboBox_statue_activated(int index);

private:
    Ui::OrderinfoWidget *ui;

    SqlHelp *m_pSqlHelp;
    QString ColumnNames[5];
    int ColumnWidths[5];
    QSqlQueryModel *m_model;

    int m_currentIndex;

    QString m_currTableId;
    QString m_currMenuName;
};

#endif // ORDERINFOWIDGET_H
