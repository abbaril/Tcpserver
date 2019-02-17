#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QMap>
#include <QStringList>
#include <QCoreApplication>
#include "tcpconnection.h"
#include "ratetransfer.h"



class HttpConnection : public TcpConnection
{
    Q_OBJECT
public:
    explicit HttpConnection(QObject *parent = nullptr);

    QString root();
    void setRoot(QString path);
    int rate();
    void setRate(int value);

protected:

    QFile *m_file;
    QString m_root;
    int m_rate;

    QMap<QString, QString> m_request;
    QMap<QString, QString> m_response;
    QMap<QString, QString> m_mime;
    RateTransfer *m_transfer;

    void processGet(QByteArray data);
    void handleRequest();
    void sendFile(QString file);
    QString getMimeType(QString path);



signals:

public slots:
    virtual void connected();
    virtual void disconnected();
    virtual void readyRead();
    virtual void bytesWritrten();
    virtual void stateChanged(QAbstractSocket::SocketState socketState);
    virtual void error(QAbstractSocket::SocketError error);


    void started();
    void transfered(qint64 bytes);
    void finished();
    void transferError();
};

#endif // HTTPCONNECTION_H
