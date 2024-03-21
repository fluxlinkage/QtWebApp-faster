/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include "requestmapper.h"

RequestMapper::RequestMapper(QObject* parent)
    :HttpRequestHandler(parent)
{
    qDebug("RequestMapper: created");
}


RequestMapper::~RequestMapper()
{
    qDebug("RequestMapper: deleted");
}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    response.setHeader("Content-Type", "text/plain; charset=UTF-8");
    QByteArray body("Hello, world!");

    if (path.startsWith("/fast"))
    {
        // Fast method
        response.writeSingle(body);
    }

    else
    {
        // Common method
        response.write(body,true);
    }

    qDebug("RequestMapper: finished request");
}
