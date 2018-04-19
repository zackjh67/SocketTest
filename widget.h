#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <stdint.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void okayToConnect();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

public slots:
    void sendJson();

    void sendConnectIntent();


private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    quint32 size;
    bool connectionSuccessful = false;
};

#endif // WIDGET_H
