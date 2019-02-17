#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->btn_stop->setEnabled(false);
}

Dialog::~Dialog()
{
    delete ui;
}

//start button
void Dialog::on_btn_start_clicked()
{
    if(m_server.listen(QHostAddress::Any, ui->spinBox->value())){
        qDebug() << this << "Server started!";
        m_server.setRate(153600);
        m_server.setRoot("C:/Users/user/Desktop/web");

        setStarted(true);

    } else {
        qCritical() << m_server.errorString();
        setStarted(false);
    }

}

//stop button
void Dialog::on_btn_stop_clicked()
{
    m_server.close();
    setStarted(false);
}

//set tyhe states of buttons here
void Dialog::setStarted(bool started)
{
    if(started){
        ui->btn_start->setEnabled(false);
        ui->btn_stop->setEnabled(true);

    }else{
        ui->btn_start->setEnabled(true);
        ui->btn_stop->setEnabled(false);
    }
}
