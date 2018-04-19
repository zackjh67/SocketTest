#include "widget.h"
#include "ui_widget.h"
#include <QByteArray>
#include <stdint.h>
#include <QJsonDocument>
#include <QtEndian>
#include <QJsonObject>
#include <QJsonArray>

bool sizeRead = false;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);
    connect(tcpSocket, &QAbstractSocket::connected, this, &Widget::sendConnectIntent); //fires when socket connected (i think)
    connect(tcpSocket, &QIODevice::readyRead, this, &Widget::okayToConnect); //fires when socket has bytes written to it

    in.setDevice(tcpSocket);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    sendJson();
}
//not really okayToConnect method- fires whenever there is data to be read
void Widget::okayToConnect()
{
    ui->label->setText("Something was sent back");



    QByteArray buffer;

    if(!sizeRead){

        buffer = tcpSocket->read(qint32(4)); //read first 4 bytes

        QDataStream ds(buffer);

        ds >> size;
        qDebug() << "size of header:";
        qDebug() << size;
        sizeRead = true;
    } else {
        //read the rest of the bytes
        buffer = tcpSocket->read(size);

        QDataStream ds(buffer);
        QString messageReceived(buffer);
        //ds >> test;
        qDebug() << "message was received";
        qDebug() << messageReceived;
        sizeRead = false;

        //successful connection response
        if(messageReceived.compare("404 OK") == 0){
            ui->label->setText("Connected");
            ui->pushButton->setEnabled(true);
        } else {
            ui->statusLabel->setText(messageReceived);
        }
    }







}

//connect
void Widget::on_pushButton_2_clicked()
{
    //connect to socket
    tcpSocket->connectToHost(ui->ipBox->toPlainText(),
                                 ui->portBox->toPlainText().toInt());
}

void Widget::sendJson(){
    //TODO JSON lowercase 'who' lowercase 'message'


    QJsonObject reply;
    //    recordObject.insert("FirstName", QJsonValue::fromVariant("John"));
    //    recordObject.insert("LastName", QJsonValue::fromVariant("Doe"));
    //    recordObject.insert("Age", QJsonValue::fromVariant(43));


        reply.insert("who", "Zack Hern");
        reply.insert("message", "This shit isn't that hard!");

        QJsonDocument doc(reply);
            //qDebug() << doc.toJson().simplified();

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

        out << doc.toJson().simplified();

        tcpSocket->write(block, block.size());
}

//sends "Connect" message
void Widget::sendConnectIntent()
{

    QString connstr = "Connect";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << connstr.toUtf8(); //send in utf encoding- size is appendedto front automtically

    //write to socket
    tcpSocket->write(block, block.size());
}
