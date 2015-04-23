#ifndef BRAIN_STROKE_WEB_H
#define BRAIN_STROKE_WEB_H

#include <QObject>
#include "httprequesthandler.h"
#include "get_version_controller.h"
#include "rename_version_controller.h"
#include "download_version_controller.h"
#include "staticfilecontroller.h"
/*
 * /rename POST old_version_name= new_version_name= pid= 负责处理文件夹重命名 mutex
 * /getallversion?pid= GET 得到一个版本所有版本号
 * /getfile?version_name=&pid= GET 得到一个pid的版本内容
 * return:
 * {"ret_code":"1"..}
 * {"ret_code":"0", "msg":""}
 */
class brain_stroke_web : public HttpRequestHandler{
    Q_OBJECT
public:
    explicit brain_stroke_web(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response);
    static StaticFileController * StaticFileController_ins;
signals:

public slots:

private:
    get_version_controller get_version_controller_ins;
    rename_version_controller rename_version_controller_ins;
    download_version_controller download_version_controller_ins;
};

#endif // BRAIN_STROKE_WEB_H
