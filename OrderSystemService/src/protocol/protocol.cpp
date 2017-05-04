#include "protocol.h"
#include <QDebug>

Protocol::Protocol(QObject *parent)
    : QObject(parent)
{
    initData();
}

void Protocol::initData()
{
    m_sendBegin = BGN_RESD_MSG;

    m_protocolLen = PROTOCOL_BASIC_SIZE;

    m_data.clear();

    m_sendEnd = END_RESD_MSG;
}

void Protocol::packSendData(quint8 flag, const QString &data)
{
    //清空
    initData();
    //填充数据
    m_sendBegin = BGN_RESD_MSG;
    if (data.length() > 0)
    {
        m_protocolLen = PROTOCOL_BASIC_SIZE + data.length() - 1;
    }else
    {
        m_protocolLen = PROTOCOL_BASIC_SIZE;
    }
    m_flag = flag;
    m_data = data;
    m_sendEnd = END_RESD_MSG;
}

QByteArray Protocol::sendData()
{
    QByteArray str;
    str.append(m_sendBegin);
    str.append(m_protocolLen);
    str.append(m_flag);
    str.append(m_data);
    str.append(m_sendEnd);

    qDebug()<<m_flag;

    return str;
}

void Protocol::insertTableId(const QString &id)
{
    m_tableIdList <<id;
}

//检测客户端发送过来的台号是否已经被占用
bool Protocol::isContainTableId(const QString &id)
{
    //当前台号列表中不存在该台号，怎插入,否则不插入
    bool isContain = false;
    foreach (QString str, m_tableIdList) {
        if (str.contains(id,Qt::CaseInsensitive)){
            return true;
        }else
        {
            isContain = false;
        }
    }
    return isContain;
}
