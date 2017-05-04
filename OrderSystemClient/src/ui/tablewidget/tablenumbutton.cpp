#include "tablenumbutton.h"
#include <qpushbutton.h>
#include <QVBoxLayout>

tableNumButton::tableNumButton(const QString &text, QFrame *parent)
    :QFrame(parent),m_text(text)
{
    initForm();
    initWidget();
    initConnect();
}

tableNumButton::~tableNumButton()
{

}

void tableNumButton::initForm()
{
    setMaximumSize(70,70);
    setObjectName("menuWidget");
}

void tableNumButton::initWidget()
{
    m_pbnNum = new QPushButton;
    m_pbnNum->setText(m_text);
    m_pbnNum->setToolTip(m_text);
//    m_pbnNum->setMinimumSize(68,68);
    m_pbnNum->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pbnNum->setObjectName("pbnGreen");
    m_pbnNum->setCursor(Qt::PointingHandCursor);
    m_pbnNum->setFont(QFont("Times", 14, QFont::Bold));

    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addWidget(m_pbnNum);
    m_pMainLayout->setContentsMargins(2,2,2,2);
    setLayout(m_pMainLayout);
}

void tableNumButton::initConnect()
{
    connect(m_pbnNum,SIGNAL(clicked(bool)),
            this,SIGNAL(signalAddTable()));
}
