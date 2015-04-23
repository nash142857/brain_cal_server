#ifndef BRAIN_CAL_THREAD_H
#define BRAIN_CAL_THREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "calculate_thread.h"
/*
 * client -> send pwi dwi para  -> server
 * server deal cal input and return a status which represents a failure or success
 * when server ended calculating, send a  {"status": "version_guid":} to client
 * or client will send a stop signal to tell server to stop calculating
 */
class tcp_deal : public QThread
{
    Q_OBJECT
public:
    explicit tcp_deal(QTcpSocket * _socket, QObject *parent = 0);
    void run();
    static const int MAX_LEN = 1000;
    ~tcp_deal(){
        puts("delete tcp deal");
    }

signals:

public slots:
    void deal_input();
    void disconnect();
private:
    QTcpSocket * socket;
    bool deal_cal_input(std::string input, calculate_thread * & ret_cal_thread);
    void deal_stop_input();
    calculate_thread * cal_thread;
    QString version_guid;
};

#endif // BRAIN_CAL_THREAD_H
