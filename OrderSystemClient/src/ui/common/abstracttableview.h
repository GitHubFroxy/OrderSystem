#ifndef ABSTRACTTABLEVIEW_H
#define ABSTRACTTABLEVIEW_H
#include <QTableView>

class AbstractTableView : public QTableView
{
    Q_OBJECT
public:
    explicit AbstractTableView(QWidget *parent = Q_NULLPTR);
    ~AbstractTableView();

Q_SIGNALS:
private Q_SLOTS:

private:
    void initForm();
};

#endif // ABSTRACTTABLEVIEW_H
