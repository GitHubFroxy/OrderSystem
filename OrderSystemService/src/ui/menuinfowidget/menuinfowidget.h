#ifndef MENUINFOWIDGET_H
#define MENUINFOWIDGET_H

#include <QWidget>

class SqlHelp;
class QSqlQueryModel;

namespace Ui {
class MenuInfoWidget;
}

class MenuInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuInfoWidget(QWidget *parent = 0);
    ~MenuInfoWidget();

private:
    void initForm();
    void init();
    void initConnect();

    void initData(QString tableName, QString columnNames[],
                  int columnWidths[]);

    void refreshMenuInfo();

Q_SIGNALS:
    //发送改变菜谱信息给终端，用于同步菜谱(包括增加菜谱，修改菜谱信息)
    void signalSendChangeMenuInfo(const QString&info);
private slots:
    void on_pbnAdd_clicked();

    void on_pbnDelete_clicked();

    void on_pbnModify_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pbnSave_clicked();

    void on_lineEdit_name_editingFinished();

private:
    Ui::MenuInfoWidget *ui;

    SqlHelp *m_pSqlHelp;

     QSqlQueryModel *model;

    QString ColumnNames[4];                    //列名数组声明
    int ColumnWidths[4];                       //列宽数组声明

    bool m_isAdd;
    bool m_isModify;
};

#endif // MENUINFOWIDGET_H
