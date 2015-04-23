#ifndef GET_VERSION_CONTROLLER_H
#define GET_VERSION_CONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
class get_version_controller : public HttpRequestHandler{
    Q_OBJECT
public:
    explicit get_version_controller(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response);

signals:

public slots:

};

#endif // GET_VERSION_CONTROLLER_H
