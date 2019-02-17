#include "httpserver.h"

HttpServer::HttpServer(QObject *parent) : TcpServer(parent)
{
    m_root = QCoreApplication::applicationDirPath();
    m_rate = 15360;
}

QString HttpServer::root()
{
    return m_root;
}

void HttpServer::setRoot(QString path)
{
    m_root = path;
    if(m_root.endsWith("/") || m_root.endsWith("\\")){

        m_root = m_root.mid(0, m_root.length() - 1);
    }
    qDebug() << this << "Root set to: " << m_root;
}

int HttpServer::rate()
{
    return m_rate;
}

void HttpServer::setRate(int value)
{
    m_rate = value;
    qDebug() << this << "Rate set to: " << m_rate;
}

void HttpServer::incominConnection(qintptr descriptior)
{
    qDebug() << this << "Incoming HTTP connection: " << descriptior;
    HttpConnection *connection = new HttpConnection();
    connection->setRate(m_rate);
    connection->setRoot(m_root);
    accept(descriptior, connection);
}
