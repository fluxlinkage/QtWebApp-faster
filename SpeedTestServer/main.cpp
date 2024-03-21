/**
  @file
  @author Stefan Frings
*/

#include <QCoreApplication>
#include <QDir>
#include <httplistener.h>
#include <filelogger.h>
#include "requestmapper.h"

using namespace stefanfrings;

/** Search the configuration file */
QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QString fileName("SpeedTestServer.ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/..");
    searchList.append(binDir+"/../"+appName);     // for development with shadow build (Linux)
    searchList.append(binDir+"/../../"+appName);  // for development with shadow build (Windows)
    searchList.append(QDir::rootPath());

    foreach (const QString &dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (const QString &dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return QString();
}


/**
  Entry point of the program.
*/
int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
    app.setApplicationName("SpeedTestServer");

    // Find the configuration file
    QString configFileName=searchConfigFile();

    // Configure logging into a file
    QSettings* logSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    logSettings->beginGroup("logging");
    FileLogger* logger=new FileLogger(logSettings,10000,&app);
    logger->installMsgHandler();

    // Configure and start the TCP listener
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings,new RequestMapper(&app),&app);

    qWarning("Application has started");
    app.exec();
    qWarning("Application has stopped");
}
