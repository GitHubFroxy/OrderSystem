#include "abstracttableview.h"

AbstractTableView::AbstractTableView(QWidget *parent)
    :QTableView(parent)
{
    initForm();
}

AbstractTableView::~AbstractTableView()
{
}

void AbstractTableView::initForm()
{
    setRowHeight(0,60);
    setRowHeight(1,30);
}
