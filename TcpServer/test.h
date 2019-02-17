#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QFile>
#include "ratetransfer.h"


class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr);

    void doTest();

private:

    QFile m_source;
    QFile m_destination;
    RateTransfer m_transfer;

signals:

public slots:
    void started();
    void transfered(int bytes);
    void finished();
    void error();
};

#endif // TEST_H
