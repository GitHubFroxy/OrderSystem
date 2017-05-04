#ifndef ABSTRACTTABLEWIDGET_H
#define ABSTRACTTABLEWIDGET_H

#include <QTableWidget>

class AbstractTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit AbstractTableWidget(const QString &icon,
                                 const QString &text,
                                 QWidget *parent = Q_NULLPTR);
    ~AbstractTableWidget();

private:
    void initForm();

private:
    QString iconName;
    QString iconText;
};

#endif // ABSTRACTTABLEWIDGET_H
