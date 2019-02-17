#ifndef TCP_H
#define TCP_H

#include <QObject>

class Tcp : public QObject
{
    Q_OBJECT
public:
    explicit Tcp(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TCP_H
