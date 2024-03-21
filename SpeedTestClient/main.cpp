#include <QCoreApplication>
#include <QTimer>
#include <QElapsedTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QByteArray syncGet(QNetworkAccessManager &manager,const QString &url)
{
    QNetworkRequest req;
    req.setUrl(url);
    QNetworkReply *reply=manager.get(req);
    QEventLoop eventLoop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    QObject::connect(&timer,&QTimer::timeout,&eventLoop, &QEventLoop::quit);
    timer.start(20000);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    if(!timer.isActive()){
        delete reply;
        throw std::runtime_error("HTTP request timeout! ");
    }
    int code=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray ret=reply->readAll();
    if(code!=200&&!ret.isEmpty()){
        delete reply;
        throw std::runtime_error(ret.toStdString());
    }
    if(reply->error()!=QNetworkReply::NoError){
        std::string errStr=reply->errorString().toStdString();
        delete reply;
        throw std::runtime_error(errStr);
    }
    return ret;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const int TIMES=100;

    const QString urlBase("http://127.0.0.1:8080");
    const QString urlCommon(urlBase+"/common");
    const QString urlFast(urlBase+"/fast");

    QElapsedTimer timer;
    QNetworkAccessManager manager;
    // Warm-up
    syncGet(manager,urlCommon);
    syncGet(manager,urlFast);
    // Test for /common
    timer.start();
    for(int i=0;i<TIMES;i++){
        syncGet(manager,urlCommon);
    }
    qInfo("Avg. time cost for /common = %f ms",timer.nsecsElapsed()*0.001*0.001/TIMES);
    // Test for /fast
    timer.start();
    for(int i=0;i<TIMES;i++){
        syncGet(manager,urlFast);
    }
    qInfo("Avg. time cost for /fast = %f ms",timer.nsecsElapsed()*0.001*0.001/TIMES);

    return 0;
}
