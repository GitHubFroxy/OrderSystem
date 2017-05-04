#include "abstractmenuwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QBitmap>
#include <QPainter>

AbstractMenuWidget::AbstractMenuWidget(const QString &icon,
                                       const QString &text,
                                       const QString &price,
                                       QFrame *parent)
    :QFrame(parent),m_icon(icon),m_text(text),m_price(price)
{
    initForm();
    initWidget();
    initConnect();
}

AbstractMenuWidget::~AbstractMenuWidget()
{
}

void AbstractMenuWidget::initForm()
{
    setObjectName("menuWidget");
    //setStyleSheet("QWidget{border:2px solid #FFFFFF;border-radius 10px;padding:4px 2px;}");

}

void AbstractMenuWidget::initWidget()
{
    // m_pTbnIcon = new QToolButton(this);
    //m_pTbnIcon->setObjectName("tbnGreen");
    //m_pTbnIcon->setIcon(QIcon(QString("%1").arg(m_icon)));
    //    m_pTbnIcon->setIconSize(QPixmap(QString("%1").arg(m_icon)).size());
    //m_pTbnIcon->setIconSize(QSize(80,80));
    //m_pTbnIcon->setText(m_text);
    //m_pTbnIcon->setStyleSheet("color:#FAA40B;font-size:14pt;font-family:ї¬Ме");
    //m_pTbnIcon->setFixedSize(120,140);
    //m_pTbnIcon->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //m_pTbnIcon->setAutoRaise(true);

    m_pLabelIcon = new QLabel;
    m_pLabelIcon->setPixmap(QPixmap(QString("%1").arg(m_icon)));
    m_pLabelIcon->resize(QPixmap(QString("%1").arg(m_icon)).width(),
                         QPixmap(QString("%1").arg(m_icon)).height());
    m_pLabelIcon->setAlignment(Qt::AlignHCenter);
    m_pLabelIcon->setToolTip(m_text);

    m_pTbnCheck = new QToolButton;
    m_pTbnCheck->setText("想吃");
    m_pTbnCheck->setCursor(Qt::PointingHandCursor);
    m_pTbnCheck->setObjectName("tbnGreen");

    m_pLabelText = new QLabel;
    m_pLabelText->setText(m_text);
    m_pLabelText->setAlignment(Qt::AlignCenter);

    //јЫёс
    m_pLabelPrice = new QLabel;
    m_pLabelPrice->setText(m_price);
    m_pLabelPrice->setAlignment(Qt::AlignCenter);
    m_pLabelPrice->setFont(QFont("Times", 12, QFont::Bold));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_pLabelText);
    layout->addWidget(m_pLabelPrice);
    layout->addWidget(m_pTbnCheck);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addWidget(m_pLabelIcon);
    m_pMainLayout->addLayout(layout);
    m_pMainLayout->setContentsMargins(5,5,5,5);
    setLayout(m_pMainLayout);
}

void AbstractMenuWidget::initConnect()
{
    connect(m_pTbnCheck,SIGNAL(clicked(bool)),
            this,SIGNAL(signalAddMenu()));
}

void AbstractMenuWidget::paintEvent(QPaintEvent *)
{
}

void AbstractMenuWidget::mouseMoveEvent(QMouseEvent *)
{
}
