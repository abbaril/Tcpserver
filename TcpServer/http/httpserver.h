#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include "tcpserver.h"
#include "httpconnection.h"


class HttpServer : public TcpServer
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);

    QString root();
    void setRoot(QString path);
    int rate();
    void setRate(int value);

protected:
    QString m_root;
    int m_rate;
    virtual void incominConnection(qintptr description);
signals:

public slots:
};

#endif // HTTPSERVER_H
