#ifndef CALCULATE_THREAD_H
#define CALCULATE_THREAD_H

#include <QObject>
#include "contable.h"
#include "QThread"
#include "calculate_method.h"
class calculate_thread : public QThread
{
    Q_OBJECT
public:
    explicit calculate_thread(QObject *parent = 0);

    bool set(DWIConstTable * _dwi_para, PWIConstTable * _pwi_para, QString _data_dir, const QString & _version_guid);
    void run();
    ~calculate_thread(){
        puts("delete calculate thread");
        if(pwi_para != NULL)
            delete pwi_para;
        if(dwi_para != NULL)
            delete dwi_para;
    }

signals:

public slots:

private:
    calculate_method cal_algo;
    DWIConstTable * dwi_para;
    PWIConstTable * pwi_para;
    QString data_dir;
    QString version_guid;
};

#endif // CALCULATE_THREAD_H
