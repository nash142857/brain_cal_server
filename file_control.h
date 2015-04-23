#ifndef FILE_CONTROL_H
#define FILE_CONTROL_H

//this class is to control data for new rename and so on
//manager by mutex
#include <QMutex>
#include <QMutexLocker>
#include <QDir>
class file_control
{
public:
    static file_control * single(){
        static file_control * fc = NULL;
        static QMutex double_lock;
        if(fc == NULL){
            double_lock.lock();
            if(fc == NULL){
                fc = new file_control();
                return fc;
            }
            double_lock.unlock();
        }
        return fc;
    }
    bool rename(QString base_name, QString old_name, QString new_name);
private:
    file_control();
    QMutex mutex;
};

#endif // FILE_CONTROL_H
