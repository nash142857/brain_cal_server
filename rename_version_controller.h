#ifndef RENAME_VERSION_CONTROLLER_H
#define RENAME_VERSION_CONTROLLER_H

#include <QObject>
#include "httprequesthandler.h"
class rename_version_controller : public HttpRequestHandler
{
    Q_OBJECT
public:
    explicit rename_version_controller(QObject *parent = 0);
    void service(HttpRequest& request, HttpResponse& response);

signals:

public slots:

};

#endif // RENAME_VERSION_CONTROLLER_H
