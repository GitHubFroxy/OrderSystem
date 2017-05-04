/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：系统登录文件
 *简要描述：从数据库中匹配输入的密码，进行登录身份的判断
 *
 *当前版本：V2.0
 *作者：Kelvin Li
 *创作日期：2014/10
 *说明：输入密码正确进入系统，错误三次后系统自动退出
******************************************************************/
#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "sqlhelp.h"
#include "widget.h"

#include <QMouseEvent>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->InitForm();
    this->InitParameter();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::InitForm()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("登录");
    //    this->setWindowIcon(QIcon(DEFAULT_ICON));

    //安装事件监听器,让标题栏识别鼠标双击
    ui->Login_widget_title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min,QChar(0xf068),10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf179), 12);

    ui->head->setStyleSheet("border-image: url(:/image/loginwidget/3.gif);");

    location = this->geometry();
    mousePressed = false;

    m_pSqlhelp = new SqlHelp(this);
}

void LoginWidget::InitParameter()
{
    //设置登录名和密码，方便调试
    ui->txtUserPwd->setText("admin");
    ui->txtUserName->setText("admin");

    ui->btnLogin->setToolTip("登录");
}

void LoginWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton))
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void LoginWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void LoginWidget::on_btnLogin_clicked()
{
    QString userName = ui->txtUserName->text();
    QString userPwd = ui->txtUserPwd->text();
    QString sql = "SELECT [user_pwd] FROM [user_info] WHERE [user_name]='"+userName+"'";
    if (userName.isEmpty() || userPwd.isEmpty())
    {
        myHelper::ShowMessageBoxError("用户名或密码不能为空!");
        return;
    }else
    {
        if (userPwd == m_pSqlhelp->GetUserInfo(sql))
        {
            qDebug()<<"登录成功!";
            this->hide();
            Widget::getInstance(userName)->show();
        }else
        {
            myHelper::ShowMessageBoxError("用户名或者密码错误，\n请检查后重新登录!");
            ui->txtUserPwd->clear();
            return;
        }
    }
}

void LoginWidget::on_btnMenu_Close_clicked()
{
    this->close();
}

void LoginWidget::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void LoginWidget::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void LoginWidget::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_Return) || ( e->key() == Qt::Key_Enter))
    {
        this->on_btnLogin_clicked();
    }
}

