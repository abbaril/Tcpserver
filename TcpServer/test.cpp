#include "test.h"

Test::Test(QObject *parent) : QObject(parent)
{

}

void Test::doTest()
{
     m_source.setFileName("C:/Users/user/Desktop/e.zip");
     m_destination.setFileName("C:/Users/user/Desktop/f.zip");

     m_transfer.setSource(&m_source);
     m_transfer.setDestination(&m_source);

     m_transfer.setRate(2046);
     m_transfer.setSize(250);

     if(!m_source.open(QFile::ReadOnly)) {
         qCritical() << "Could not open the source";
         return;
     }

     if(!m_destination.open(QFile::WriteOnly)) {
         qCritical() << "Could not open the destination";
         m_source.close();
         return;
     }

     connect(&m_transfer, &RateTransfer::started, this, &Test::started);
     connect(&m_transfer, &RateTransfer::transfered, this, &Test::transfered);
     connect(&m_transfer, &RateTransfer::finished, this, &Test::finished);
     connect(&m_transfer, &RateTransfer::error, this, &Test::error);

     m_transfer.start();

}

void Test::started()
{
    qDebug() << this << "Transfer started";
}

void Test::transfered(int bytes)
{
    qDebug() << this << "Transfered " << bytes;
}

void Test::finished()
{
    qDebug() << this << "Finished";
    m_source.close();
    m_destination.close();

}

void Test::error()
{
    qDebug() << this << "Error= " << m_transfer.errorString();
}
