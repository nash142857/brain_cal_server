#include "calculate_thread.h"
#include "SortDcmFile.h"
#include "QsLog.h"
#include <uuid/uuid.h>
const char * temp_filename = "tmp_data";


calculate_thread::calculate_thread(QObject *parent) :
    QThread(parent){
    dwi_para = NULL;
    pwi_para = NULL;
}

bool check_dir(const QString & relative_dir){
    static QDir dir;
    if(!dir.exists(relative_dir)) return false;
    if(!dir.exists(relative_dir + "/origin")) return false;
    if(!dir.exists(relative_dir + "/origin/DWI")) return false;
    if(!dir.exists(relative_dir + "/origin/PWI")) return false;
    return true;
}
void auto_mkdir(const QString & relative_dir, const QString  &version_guid){
    static QDir dir;
    if(!dir.exists(relative_dir + "/result")){
        dir.mkdir(relative_dir + "/result");
    }
    if(!dir.exists(relative_dir + "/result/" + version_guid)){
        dir.mkdir(relative_dir + "/result/" + version_guid);
    }
    QString  arr[9] = {"/ADC", "/MTT", "/bmpCBF", "/bmpMTT", "/bmpTmax", "/bmpCBV","/CBF", "/CBV","/Tmax"};
    for(int i = 0; i < 9; ++ i){
        if(!dir.exists(relative_dir + "/result/" + version_guid + "/" + arr[i])){
             dir.mkdir(relative_dir + "/result/" + version_guid + "/" + arr[i]);
        }
    }

}
bool calculate_thread::set(DWIConstTable * _dwi_para, PWIConstTable * _pwi_para, QString _data_dir, const QString & _version_guid){
    dwi_para = _dwi_para;
    pwi_para = _pwi_para;
    data_dir = _data_dir;
    //check data dir right format
    if(!check_dir(data_dir)){
        return false;
    }
    version_guid = _version_guid;
    //auto make dir by version_guid
    auto_mkdir(data_dir, version_guid);
    return true;
}

void calculate_thread::run(){
    QLOG_DEBUG() << "calculate_thread run" << QThread::currentThread();
    QLOG_DEBUG() << "data dir" << data_dir;
    puts("begin run");
    //TODO double free
    //PWI&DWI sort file
    SortDcmFile pwiSort;
    SortDcmFile dwiSort;
    pwiSort.sortFile((data_dir + "/origin/PWI/").toStdString().c_str());
    dwiSort.sortFile((data_dir + "/origin/DWI/").toStdString().c_str());
    pwi_para->PSetNum = pwiSort.PsetNum;
    pwi_para->PSliceNum = pwiSort.Pslice;
    //init and compute
    cal_algo.init(pwi_para);
    cal_algo.PWI_Compute((data_dir + "/origin/PWI/").toStdString(), data_dir.toStdString() + "/result/" + version_guid.toStdString() + "/",pwi_para);
    cal_algo.DWI_Compute((data_dir + "/origin/DWI/").toStdString(), data_dir.toStdString() + "/result/" + version_guid.toStdString() + "/ADC/",dwi_para);
    puts("end run");
}
