#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QMap>

class tableNumButton;
class QGridLayout;
class SqlHelp;
class QSignalMapper;

namespace Ui {
class TableWidget;
}

class TableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableWidget(QWidget *parent = 0);
    ~TableWidget();
private:
    void initForm();
    void initWidget();
    void initConnect();

    void setTableNumber();      //设置餐桌数目

Q_SIGNALS:
    void signalSendTableId(const QString &);             //发送台号

private Q_SLOTS:
    void slotGetClickTableNum(const QString &);

    //设置台号可用
    void slotSetTableEnable();

private:
    Ui::TableWidget *ui;

    QMap<tableNumButton *,QString>m_btnMap;

    QGridLayout *m_pMainGridLayout;

    SqlHelp *m_PSqlHelp;

    int m_tableCount;               //餐桌总数
    QStringList m_tableNumList;     //餐桌编号
    QString m_currTableID;

    QSignalMapper *m_pSignalMapper;
};

#endif // TABLEWIDGET_H
