#include "tcpconnection.h"

TcpConnection::TcpConnection(QObject *parent) : QObject(parent)
{
    qDebug() << "Created!\n";
}

void TcpConnection::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
    connect(m_socket, &QTcpSocket::connected, this, &TcpConnection::connected);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpConnection::disconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &TcpConnection::readyRead);
    connect(m_socket, &QTcpSocket::bytesWritten, this, &TcpConnection::bytesWritten);
    connect(m_socket, &QTcpSocket::stateChanged, this, &TcpConnection::stateChanged);
    connect(m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>
            (&QTcpSocket::error), this, &TcpConnection::error);



}

QTcpSocket *TcpConnection::getSocket()
{
    if(!sender()) return nullptr;
    return static_cast<QTcpSocket*>(sender());

}

void TcpConnection::connected()
{
    if(!sender()) return;
    qDebug() << this << "Connected " << sender();
}

void TcpConnection::disconnected()
{
    if(!sender()) return;
    qDebug() << this << "Disconnected " << getSocket();
}

void TcpConnection::readyRead()
{
    if(!sender()) return;
    qDebug() << this << "Ready read" << getSocket();
}

void TcpConnection::bytesWritten(qint64 bytes)
{
    if(!sender()) return;
    qDebug() << this << "Bytes written " << getSocket() << " number of bytes = " << bytes;
}

void TcpConnection::stateChanged(QAbstractSocket::SocketState s_state)
{
    if(!sender()) return;
    qDebug() << this << "State Changed" << getSocket() << " socket state = " << s_state;

}

void TcpConnection::error(QAbstractSocket::SocketError s_error)
{
    if(!sender()) return;
    qDebug() << this << "Error " << getSocket() << " socket error" << s_error;
}
