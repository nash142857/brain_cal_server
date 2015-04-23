#include "rename_version_controller.h"
#include "config.h"
#include "QsLog.h"
#include <QDir>
#include "file_control.h"
#include "common.h"
#include <QProcess>
//重命名文件夹
rename_version_controller::rename_version_controller(QObject *parent) :
    HttpRequestHandler(parent){

}

//rename POST old_version_name= new_version_name= pid= 负责处理文件夹重命名 mutex
void rename_version_controller::service(HttpRequest& request, HttpResponse& response){
    QString old_version_name, new_version_name, pid;
    old_version_name = request.getParameter("old_version_name");
    new_version_name = request.getParameter("new_version_name");
    pid = request.getParameter("pid");
    if(old_version_name == "" || new_version_name == "" || pid == ""){
        QLOG_ERROR() << "input para with no old_version_name or new_version_name" << "rename_version_controller";
        response.setStatus(401,"para error");
        response.write("para error");
        return;
    }
    //change
    bool flg = file_control::single() -> rename(Config::single() -> mp["pdata_dir"] + "/" + pid + "/" + "result", old_version_name,new_version_name);
    response.write(commutil::make_ret_msg(flg,"").toUtf8());
}
