#include "tcpconnections.h"

TcpConnections::TcpConnections(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
}

int TcpConnections::count()
{
    QReadWriteLock lock;
    lock.lockForRead();
    int value = m_connection.count();

    lock.unlock();

    return value;
}

void TcpConnections::removeSocket(QTcpSocket *socket)
{
    if(!socket) return;
    if(!m_connection.contains(socket)) return;

    qDebug() << this << "Removing socket = " << socket;

    if(socket->isOpen()){
        qDebug() << this << "Socket is open, attemting to close it  " << socket;
        socket->disconnect();
        socket->close();
    }

    qDebug() << this << "Deleting socket" << socket;
    m_connection.remove(socket);
    socket->deleteLater();

    qDebug() << this << "Client count" << m_connection.count();

}

void TcpConnections::disconnected()
{
    if(!sender()) return;
    qDebug() << this << "Disconnecting socket..." << sender();
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if(!socket) return;

    removeSocket(socket);
}

void TcpConnections::error(QAbstractSocket::SocketError socketError)
{
    if(!sender()) return;
    qDebug() << this << "Error in socket" << sender() << " error = " << socketError;
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    removeSocket(socket);
}

void TcpConnections::start()
{
    qDebug() << this << "Connection started on " << QThread::currentThread();
}

void TcpConnections::quit()
{
    if(!sender()) return;
    qDebug() << this << "Connections quitting";
    foreach (QTcpSocket *socket, m_connection.keys()) {
       qDebug() << this << "Closing socket" << socket;
       removeSocket(socket);
    }

    qDebug() << this << "Finishing";
    emit finished();
}

void TcpConnections::accept(qintptr handle, TcpConnection *connection)
{
    QTcpSocket *socket = new QTcpSocket(this);

    if(!socket->setSocketDescriptor(handle))
    {
        qWarning() << this << "Could not accept connection" << handle;
        connection->deleteLater();
        return;

    }

    connect(socket, &QTcpSocket::disconnected, this, &TcpConnections::disconnected);
    connect(socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>
            (&QTcpSocket::error), this, &TcpConnections::error);

    connection->moveToThread(QThread::currentThread());
    connection->setSocket(socket);
     m_connection.insert(socket, connection);

    qDebug() << this << "Clients = " << m_connection.count();

    emit socket->connected();

}
