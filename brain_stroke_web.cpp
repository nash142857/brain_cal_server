#include "brain_stroke_web.h"
#include "QsLog.h"
#include "staticfilecontroller.h"
StaticFileController* brain_stroke_web::StaticFileController_ins=0;
//内部数据传输token
const char * inner_data_trans_key = "dc986724-7533-472d-8a62-a42cceec559";
brain_stroke_web::brain_stroke_web(QObject *parent) :
    HttpRequestHandler(parent){
    puts("init brain_stroke_web");
    QLOG_DEBUG() << "init brain_stroke_web";
}

void brain_stroke_web::service(HttpRequest &request, HttpResponse &response){
    QByteArray path = request.getPath();
    if(path =="/getallversion"){
        get_version_controller_ins.service(request,response);
        return;
    }
    else if (path == "/getfile") {
        download_version_controller_ins.service(request,response);
    }
    else if (path == "/rename"){
        rename_version_controller_ins.service(request,response);
        return;
    }
    else if(path.startsWith("/data")){
        QString key = request.getParameter("key");
        if(key != QString(inner_data_trans_key)){
            response.setStatus(401,"key error");
            response.write("key error");
        }
        else{
            StaticFileController_ins->service(request,response);
        }
    }
    else{
        response.setStatus(401,"no atched request");
        response.write("no matched request", false);
    }
}
