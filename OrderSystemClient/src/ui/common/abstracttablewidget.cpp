#include "abstracttablewidget.h"
#include <QHeaderView>

AbstractTableWidget::AbstractTableWidget(const QString &icon,
                                         const QString &text, QWidget *parent)
    :QTableWidget(parent),iconName(icon),iconText(text)
{
    initForm();

    QTableWidgetItem *iconItem = new QTableWidgetItem;
    iconItem->setIcon(QIcon(QString("%1").arg(iconName)));
    iconItem->setToolTip(text);
    iconItem->setTextAlignment(Qt::AlignVCenter |Qt::AlignCenter);
    setItem(0,0,iconItem);

    QTableWidgetItem *textItem = new QTableWidgetItem;
    textItem->setText(iconText);
    textItem->setTextAlignment(Qt::AlignHCenter);
    setItem(1,0,textItem);
}

AbstractTableWidget::~AbstractTableWidget()
{

}

void AbstractTableWidget::initForm()
{

    setRowCount(2);                     //设置行数
    setColumnCount(1);
    //设定行高,这里因为固定显示2行，
    //0：列出菜的照片
    //1:对应的菜名
    setRowHeight(0,60);
    setRowHeight(1,50);
    setContentsMargins(0,0,0,0);
    setColumnWidth(0,100);
    // setFrameShape(QFrame::NoFrame);    //设置边框
    setShowGrid(false);                   //不显示网格
    //取消自带的编号
    QHeaderView *view = this->verticalHeader();
    view->setHidden(true);
    //隐藏表头
    QHeaderView *horHead = this->horizontalHeader();
    horHead->setHidden(true);

    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus); //无焦点
    setSelectionMode(QAbstractItemView::SingleSelection);   //设置单行选中
    setEditTriggers(QAbstractItemView::NoEditTriggers);     //不可编辑

//    resizeColumnsToContents();
//    resizeRowsToContents();

    //设置背景透明
    QPalette pll;
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    setPalette(pll);

}
