#include "download_version_controller.h"
#include "config.h"
#include "QsLog.h"
#include "staticfilecontroller.h"
#include <QDir>
#include <QProcess>
#include <QStringList>
//getfile?version_name=&pid= GET 得到一个pid的版本内容
extern char * inner_data_trans_key;
download_version_controller::download_version_controller(QObject *parent):
    HttpRequestHandler(parent){
}
void download_version_controller::service(HttpRequest &request, HttpResponse &response){
    QString version_name = request.getParameter("version_name");
    QString pid = request.getParameter("pid");
    if(pid == "" || version_name == ""){
        QLOG_ERROR() << "input para wrong pid, version" << "download_version_controller";
        response.setStatus(401,"para error");
        response.write("para error");
        return;
    }
    QDir result_dir = Config::single() -> mp["pdata_dir"] + "/" + pid + "/result/" + version_name;
    if(!result_dir.exists()){
        response.setStatus(401,"data not exists");
        response.write("data not exists");
        return;
    }
    QProcess process;
    process.setWorkingDirectory(Config::single() -> mp["pdata_dir"] + "/" + pid + "/result");
    process.start("zip -r " + version_name + ".zip " + version_name);
    bool ret_code = process.waitForFinished();
    if(!ret_code){
        response.setStatus(401,"zip error");
        response.write("zip error");
    }
    response.redirect(("/data/"+ pid + "/result/" + version_name + ".zip?key=" + QString(inner_data_trans_key)).toUtf8()) ;
}

