#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QMap>

class QLabel;
class AbstractTableWidget;
class AbstractMenuWidget;
class QGridLayout;
class OrderListWidget;
class QSignalMapper;
class SqlHelp;
class QVBoxLayout;
class QPushButton;

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = 0);
    ~MenuWidget();

private:
    void initForm();
    void initData();
    void initWidget();
    void initConnect();
    void initMenuInfo();            //加载菜谱信息

    void removeLayout();

Q_SIGNALS:
    void signalSendMenuInfo(const QString &);
    void signalSendToServiceMenuInfo(const QString &);
    void signalSendToServerMenuAccount(const QString&);

    void signalSendToServiceBackMenu(const QString &);  //退菜
    void signalSendToServiceHurryMenu(const QString&);  //催菜

    void signalSendMenuOk(const QString &);
    void signalSendTableId(const QString &);

    void signalEmptyMenuTemp();

    void signalSubmitOrderOk();

private Q_SLOTS:
    //显示上一页菜单
    void slotShowNextMenu();
    //显示下一页菜单
    void slotShowPreviousMenu();

    void slotAddMenu(const QString &);

    //接收台号信息，用于显示
    void slotReceiveTableId(const QString &);

    //重新设置提示台号的文本
    void slotChangeTableIdInfo();

private:
    Ui::MenuWidget *ui;

    QWidget *m_pPageWidget;
    QList<AbstractMenuWidget *>m_menu_list; //菜单
    QMap<QString,QString>m_MenuMap;         //保存菜谱信息，名称和编号
    QStringList m_iconList;                 //菜单图片
    QStringList m_textList;                 //菜单名称
    QStringList m_priceList;                //菜单价格
    OrderListWidget *m_pOrderWidget;        //订单列表
    QGridLayout *m_pMainLayout;             //主布局

    QLabel *m_pLabelTableId;                //台号
    QVBoxLayout *m_pLeftLayout;

    QSignalMapper *m_PSignalMapper;
    SqlHelp *m_pSqlHelp;

    QPushButton *m_pPbnNext;
    QPushButton *m_pPbnPrevious;
    QLabel *m_pLabelTotalNum;               //菜谱总数
    QLabel *m_pLabelCurrentPage;            //当前第几页

    int page_count;                         //总页数
    int page_count_point;                   //当前页号
    int current_page;                       //当前为第几页
};

#endif // MENUWIDGET_H
