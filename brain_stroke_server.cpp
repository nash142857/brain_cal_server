#include "brain_stroke_server.h"
#include <string>
#include <iostream>
#include <QtCore>
#include "QsLog.h"
#include "QsLogDest.h"
#include "calculate_method.h"
#include "contable.h"
#include "config.h"
#include "tcp_deal.h"
#include <QTcpServer>
#include <QHostAddress>


brain_stroke_server::brain_stroke_server(QObject *parent) :
    QObject(parent){
    connect(&server, SIGNAL(newConnection()), this,SLOT(accept_new_connection()));
    server.listen(QHostAddress::Any, 8888);
    puts("listen");
}

void brain_stroke_server::accept_new_connection(){
    QTcpSocket * client = server.nextPendingConnection();
    puts("accept new");
    tcp_deal * thread = new tcp_deal(client, this);
    connect(thread, SIGNAL(finished()),thread,SLOT(deleteLater()));
    thread -> start();
    QLOG_DEBUG() << "accept" << QThread::currentThread();
}

brain_stroke_server::~brain_stroke_server(){
    server.close();
}

