#include "get_version_controller.h"
#include "QsLog.h"
#include "config.h"
#include "common.h"
#include <QDir>
#include "common.h"
#include <QJsonArray>
get_version_controller::get_version_controller(QObject *parent) :
    HttpRequestHandler(parent){

}
void get_version_controller::service(HttpRequest& request, HttpResponse& response){
    QString pid;
    try{
        pid = request.getParameter("pid");
    }
    catch(...){
        QLOG_ERROR() << "input para with no pid in class" << "get_version_controller";
        response.setStatus(401,"para error");
        response.write("para error");
        return;
    }
    QString result_dir = Config::single()->mp["pdata_dir"] + "/" + pid + "/result";
    QLOG_DEBUG() << "debug result dir" << result_dir;
    QDir dir(result_dir);
    if(!dir.exists()){
        response.write(commutil::make_ret_msg(false,"dir not exixts").toUtf8());
        return;
    }
    QJsonArray json_arr;
    foreach(QFileInfo file, dir.entryInfoList()){
        if(file.isDir()){
            if(!file.fileName().startsWith(".")){
                //ignore tmp guid dir data
                if(file.fileName().size() != 36){
                    json_arr.append(file.fileName());
                }
            }
        }
    }
    QJsonDocument document;
    document.setArray(json_arr);
    response.write(commutil::make_ret_msg(true,document.toJson(QJsonDocument::Compact)).toUtf8());
}
