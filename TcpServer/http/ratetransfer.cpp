#include "ratetransfer.h"

RateTransfer::RateTransfer(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created!";
    setDefaults();
}

int RateTransfer::rate()
{
    return m_rate;
}

void RateTransfer::setRate(int value)
{
    m_rate = value;
    qDebug() << this << "Rate set to: " << value;
}

int RateTransfer::size()
{
    return m_size;
}

void RateTransfer::setSize(int value)
{
    m_size = value;
    qDebug() << this << "Size set to: " << value;
}

qint64 RateTransfer::maximum()
{
    return m_maximum;
}

void RateTransfer::setMaximum(qint64 value)
{
    m_maximum = value;
    qDebug() <<  this << "Maximum set to: " << value;
}

QIODevice *RateTransfer::source()
{
    return m_source;
}

void RateTransfer::setSource(QIODevice *device)
{
    m_source = device;
    qDebug() << this << "Source set to: " << device;
    if(m_source->isSequential()) connect(m_source, &QIODevice::readyRead, this, &RateTransfer::readyRead);

}

QIODevice *RateTransfer::destination()
{
    return m_destination;
}

void RateTransfer::setDestination(QIODevice *device)
{
    m_destination = device;
    qDebug() << this << "Destination set to: " << device;
    if(m_destination->isSequential()) connect(m_source, &QIODevice::bytesWritten, this, &RateTransfer::bytesWritten);
}

bool RateTransfer::isTransfering()
{
    return m_transfering;
}

QString RateTransfer::errorString()
{
    return m_error;
}

void RateTransfer::setDefaults()
{
    qDebug() << this << "Setting the defaults";
    m_rate = 0;
    m_size = 1024; //default byte size
    m_maximum = 0;
    m_transfering = false;
    m_transfered = 0;
    m_source = nullptr;
    m_destination = nullptr;
    m_error = "";
    m_scheduled = false;
    m_timer.setInterval(5);

}

bool RateTransfer::checkDevices()
{
    if(!m_source) {
        m_transfering = false;
        m_error = "No source device";
        qDebug() << this << m_error;
        emit error();
        return false;
    }
    if(!m_destination) {
        m_transfering = false;
        m_error = "No destination device";
        qDebug() << this << m_error;
        emit error();
        return false;
    }
    if(!m_source->isOpen() || !m_source->isReadable()) {
        m_transfering = false;
        m_error = "Source device is not open or is not readable!";
        qDebug() << this << m_error;
        emit error();
        return false;
    }
    if(!m_destination->isOpen() || !m_destination->isWritable()) {
        m_transfering = false;
        m_error = "Destination device is not open or is not writtable!";
        qDebug() << this << m_error;
        emit error();
        return false;
    }

    return true;
}

bool RateTransfer::checkTransfer()
{
    if(!m_destination) {
        m_error = "Not transferring aborting!";
        qDebug() << this << m_error;
        emit error();
        return false;
    }
    if((!m_transfered) >= m_rate) {
        m_transfering = false;
        m_error = "Rate exceeded! not allowed to transfer!";
        qDebug() << this << m_error;
        emit error();
        return false;
    }
    return true;
}

void RateTransfer::scheduleTransfer()
{
    qDebug() <<  this << "ScheduleTransfer called";
    if(m_scheduled) {
        qWarning() << this << "Exiting scheduleTransfer due to: waiting on timer";
        return;
    }

    if(!m_transfering) {
        qWarning() << this << "Exiting scheduleTransfer due to: not transfering";
        return;
    }

    if(m_source->bytesAvailable() <= 0) {
        qWarning() << this << "Exiting scheduleTransfer due to: not bytes available to be read";
        return;
    }

    qint64 prediction =  m_transfered + m_size;
    if(prediction <= m_rate) {
        qDebug() << this << "Calling transfer from scheduleTransfer";
        transfer();
    } else {
        int current = QTime::currentTime().msec();
        int delay = 1000 - current;
        qDebug() << this << "Rate limit(" << m_rate << ") exceeded in prediction(" << m_transfered << " to " << prediction << "), delaying transfer for " << delay << " ms";
        m_transfered = 0;
        m_scheduled = true;
        m_timer.singleShot(delay, this, SLOT(transfer()));
    }
}

void RateTransfer::start()
{
    qDebug() <<  this << "start called";
    if(m_transfering) {
        qDebug() << "Already transfering";
        return;
    }

    m_error = "";
    if(!checkDevices()) return;

    m_transfering = true;
    m_transfered = 0;
    emit started();

    if(!m_source->isSequential() && m_source->bytesAvailable() > 0) {
        qDebug() << this << "Starting transfer by calling scheduleTransfer";
        scheduleTransfer();
    }
}

void RateTransfer::stop()
{
    qDebug() << this << "Stoping the transfer";
    m_timer.stop();
    m_transfering = false;
}

void RateTransfer::transfer()
{
    m_scheduled = false;
    qDebug() << this << "Transfering at a maximum of: " << m_rate << " bytes per second";
    m_error = "";

    if(!checkDevices()) return;
    if(!checkTransfer()) return;

    qDebug() << this << "Reading from source";
    QByteArray buffer;
    buffer = m_source->read(m_size);

    qDebug() << "Written to destination: " << buffer.length();
    m_destination->write(buffer);
    m_transfered += buffer.length();
    emit transfered(m_transfered);

    if(m_maximum > 0 && m_transfered >= m_maximum) {

        qDebug() << this << "Stopping due to maximum limit reached";
        emit finished();
        stop();
    }

    if(!m_source->isSequential() && m_source->bytesAvailable() == 0) {
        qDebug() << this << "Stopping due to end of file";
        emit finished();
        stop();
    }

    if(m_transfering == false) return;
    if(!m_source->isSequential() && m_source->bytesAvailable() > 0) {
        qDebug() << this << "Source still have bytes, rescheduling a transfer";
        scheduleTransfer();

    }
}

void RateTransfer::readyRead()
{
    qDebug() << this << "readyRead() signaled ";
    scheduleTransfer();
}

void RateTransfer::bytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);
    qDebug() << this << "readyRead() signaled ";
    scheduleTransfer();
}
