#ifndef DOWNLOAD_VERSION_CONTROLLER_H
#define DOWNLOAD_VERSION_CONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
#include "staticfilecontroller.h"
class download_version_controller : public HttpRequestHandler{
    Q_OBJECT
public:
    explicit download_version_controller(QObject *parent = 0);
    void service(HttpRequest &request, HttpResponse &response);
signals:

public slots:

};

#endif // DOWNLOAD_VERSION_CONTROLLER_H
