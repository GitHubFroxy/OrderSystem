#ifndef ABSTRACTMENUWIDGET_H
#define ABSTRACTMENUWIDGET_H

#include <QWidget>
#include <QFrame>

class QToolButton;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;

class AbstractMenuWidget : public QFrame
{
    Q_OBJECT
public:
    explicit AbstractMenuWidget(const QString &icon,const QString &text,
                                const QString &price,QFrame *parent = 0);
    ~AbstractMenuWidget();

Q_SIGNALS:
    //添加该菜到订单中
    void signalAddMenu();

public Q_SLOTS:

private:
    void initForm();
    void initWidget();
    void initConnect();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseMoveEvent(QMouseEvent*);

private:
    QToolButton *m_pTbnIcon;
    QLabel *m_pLabelIcon;
    QToolButton *m_pTbnCheck;
    QLabel *m_pLabelText;
    QLabel *m_pLabelPrice;
    QVBoxLayout *m_pMainLayout;

    QString m_icon;
    QString m_text;
    QString m_price;            //菜的价格
};

#endif // ABSTRACTMENUWIDGET_H
