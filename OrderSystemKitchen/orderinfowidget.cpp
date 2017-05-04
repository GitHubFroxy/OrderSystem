#include "orderinfowidget.h"
#include "ui_orderinfowidget.h"
#include "sqlhelp.h"
#include "myhelper.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

OrderinfoWidget::OrderinfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderinfoWidget)
{
    ui->setupUi(this);

    initForm();
    init();
    initConnect();
}

OrderinfoWidget::~OrderinfoWidget()
{
    delete ui;
}

void OrderinfoWidget::initForm()
{
    ColumnNames[0] = "桌号";
    ColumnNames[1] = "菜名";
    ColumnNames[2] = "数量";
    ColumnNames[3] = "状态";
    ColumnNames[4] = "备注";

    ColumnWidths[0] = 100;
    ColumnWidths[1] = 150;
    ColumnWidths[2] = 100;
    ColumnWidths[3] = 100;
    ColumnWidths[4] = 150;

    //tableview默认没有焦点时，index为-1
    m_currentIndex = -1;

    ui->lineEdit_tableId->setEnabled(false);
    ui->lineEdit_menuName->setEnabled(false);
    ui->comboBox_statue->setDisabled(true);
    ui->lineEdit_remark->setEnabled(false);

    ui->pbnChangeState->setToolTip("改变菜的状态");
    ui->pbnLackMenu->setToolTip("向服务器发送缺菜信息");
    ui->pbnok->setToolTip("表示该菜已经做完");

    ui->tableView->setFocusPolicy(Qt::NoFocus);
}

void OrderinfoWidget::init()
{
    m_pSqlHelp = new SqlHelp;

    initData("menu_info",ColumnNames,ColumnWidths);
}

void OrderinfoWidget::initConnect()
{
}

void OrderinfoWidget::initData(QString tableName, QString columnNames[], int columnWidths[])
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

    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式单行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    ui->tableView->setStyleSheet( "QTableView::item:hover{background-color:rgb(98,188,214,60)}"
                                  "QTableView::item:selected{background-color:#0997C0}");
}

void OrderinfoWidget::refreshMenuInfo()
{
    m_model->setQuery("SELECT * FROM menu_info");
    ui->tableView->setModel(m_model);
}

void OrderinfoWidget::deleteMenuInfo(const QString &tableId)
{
    QString sql = "DELETE FROM [menu_info] WHERE [table_id]='"+tableId+"'";
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"缺菜时，删除催菜信息成功!";
    }else
    {
        qDebug()<<"缺菜时，删除催菜信息失败!";
    }
}

/*
 * 接收服务器端的信息：
 * 格式为：#2#011#红烧排骨#1##
 * 解释： 标志 台号  菜名  数量 备注
 * ("", "2","019", "红烧排骨", "1", "", "")
*/
void OrderinfoWidget::slotReceiveMenuInfo(const QString &info)
{
#if 0
    QStringList str = info.split('#');

    qDebug()<<"订单界面接收到的数据:"<<str;
    QString tableId = str.at(2);
    QString menuName = str.at(3);
    QString menuNum = str.at(4);
    QString menuRemark = str.at(5);

    QString sql = "INSERT INTO [menu_info] VALUES('"+tableId+"','"+menuName+"','未完成','";
    sql += menuRemark+"')";
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"插入数据成功！";
    }else
    {
        qDebug()<<"插入数据失败！";
    }

    refreshMenuInfo();
#endif
    /*
     * 2016年4月24日15:51:15
     * 修改：之前是一条一条接收菜单信息，现在是菜单信息打包后接收
     *格式：#2#鱼香肉丝#1#12#,#土豆丝#1#10#,#尖椒肉丝#1#10#
    */
    qDebug()<<"\n厨房端接收到服务器发送过来的订单数据包:"<<info<<endl;
    //将接收到的数据插入到临时数据库中
    QString id = info.split("#").at(2);
    m_currTableId = id;
    QStringList data = info.mid(4).split(",");
    qDebug()<<"分割后数据:"<<data;
    QStringList temp;
    int orderCount = data.size();       //获取记录数
    QSqlQuery query;
    query.prepare("INSERT INTO menu_info VALUES(?,?,?,?,?)");
    QVariantList tableId,menu_name,menu_num,menu_state,menu_remark;

    for (int i = 0; i < orderCount; i++)
    {
        temp = data.at(i).split("#");
        tableId <<id;
        menu_name <<temp.at(1);
        menu_num <<temp.at(2);
        menu_state <<"未做";
        menu_remark <<QVariant(QVariant::String);
    }

    query.addBindValue(tableId);
    query.addBindValue(menu_name);
    query.addBindValue(menu_num);
    query.addBindValue(menu_state);
    query.addBindValue(menu_remark);
    if (!query.execBatch())
    {
        qDebug()<<query.lastError();
    }else
    {
        refreshMenuInfo();
    }

}

void OrderinfoWidget::slotReceiveHurryMenuInfo(const QString &info)
{
    qDebug()<<"接收到服务器传递过来的催菜信息"<<info;
}

void OrderinfoWidget::slotRefreshMenuInfo()
{
    refreshMenuInfo();
}

//完成--表示该菜已经做完，发送相应的命令给服务器，通知
void OrderinfoWidget::on_pbnok_clicked()
{
    //确保当前tableview有焦点
    if(m_currentIndex != -1)
    {
        if (myHelper::ShowMessageBoxQuesion("是否确认?确认:则删除当前已经完成的菜单,否则取消!"))
        {

            //发送完成的菜的信息，格式为:台号#菜名
            QString data = m_currTableId +"#"+m_currMenuName;
            emit signalSendFinishMenu(data);

            QString sql = "DELETE FROM [menu_info] WHERE [table_id]='"+m_currTableId+"'";
            sql += " AND [menu_name]='"+m_currMenuName+"'";
            if (m_pSqlHelp->ExecuteSql(sql))
            {
                qDebug()<<"删除做完的菜成功!";
            }else
            {
                qDebug()<<"删除做完的菜失败!";
            }

            //刷新显示
            refreshMenuInfo();
        }
    }else
    {
        myHelper::ShowMessageBoxError("当前没有任何菜!");
        return;
    }
}

//缺菜
void OrderinfoWidget::on_pbnLackMenu_clicked()
{
    //确保当前tableview有焦点
    if(m_currentIndex != -1)
    {
        if (myHelper::ShowMessageBoxQuesion("是否确认?确定:删除当前缺菜信息,否则取消"))
        {
            //发送缺菜信息，格式为:台号#菜名
            QString data = m_currTableId +"#"+m_currMenuName;

            emit signalSendToServiceLackMenu(data);
            //同时删除该菜的信息
            deleteMenuInfo(m_currTableId);
        }
    }else;
    {
        myHelper::ShowMessageBoxError("当前没有选择任何菜!");
        return;
    }

}

//改变状态
void OrderinfoWidget::on_pbnChangeState_clicked()
{
    //确保当前tableview有焦点
    if(m_currentIndex != -1)
    {
        if (myHelper::ShowMessageBoxQuesion("是否确认?确定:改变当前菜的状态，否则取消"))
        {
            ui->comboBox_statue->setDisabled(false);
            ui->comboBox_statue->setEditable(false);
        }
    }else
    {
        myHelper::ShowMessageBoxError("当前没有选择任何菜!");
        return;
    }
}

void OrderinfoWidget::on_tableView_clicked(const QModelIndex &index)
{
    m_currentIndex = index.row();

    QString tableId = m_model->record(index.row()).value(0).toString();
    QString menuName = m_model->record(index.row()).value(1).toString();
    QString menuStatue = m_model->record(index.row()).value(2).toString();
    QString remark = m_model->record(index.row()).value(3).toString();

    m_currTableId = tableId;
    m_currMenuName = menuName;

    ui->lineEdit_tableId->setText(tableId);
    ui->lineEdit_menuName->setText(menuName);
    ui->lineEdit_remark->setText(remark);

    if (menuStatue == "未完成")
    {
        ui->comboBox_statue->setCurrentIndex(1);
    }else
    {
        ui->comboBox_statue->setCurrentIndex(0);
    }
}

void OrderinfoWidget::on_comboBox_statue_activated(int index)
{
    Q_UNUSED(index);
    //0:完成，1:未完成
    QString sql = "UPDATE [menu_info] SET [menu_state] = '"+ui->comboBox_statue->currentText()+"'";
    sql += " WHERE [menu_name]='"+m_currMenuName+"'";
    if (m_pSqlHelp->ExecuteSql(sql))
    {
        qDebug()<<"状态改变成功!";
    }else
    {
        qDebug()<<"状态改变失败!";
    }

    ui->comboBox_statue->setDisabled(true);
    //刷新显示界面
    refreshMenuInfo();
}
