#ifndef TABLENUMBUTTON_H
#define TABLENUMBUTTON_H

#include <QFrame>
class QPushButton;
class QVBoxLayout;

class tableNumButton : public QFrame
{
    Q_OBJECT
public:
    explicit tableNumButton(const QString&text ,QFrame *parent = 0);
    ~tableNumButton();

Q_SIGNALS:
    void signalAddTable();

private:
    void initForm();
    void initWidget();
    void initConnect();

private:
    QPushButton *m_pbnNum;
    QVBoxLayout *m_pMainLayout;

    QString m_text;
};

#endif // TABLENUMBUTTON_H
