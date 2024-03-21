# This project demonstrates how to use QtWebAppLib by including the
# sources into this project.

TARGET = SpeedTestServer
TEMPLATE = app
QT = core network
CONFIG += console

HEADERS += requestmapper.h

SOURCES += main.cpp \
           requestmapper.cpp

OTHER_FILES += SpeedTestServer.ini

#---------------------------------------------------------------------------------------
# The following lines include the sources of the QtWebAppLib library
#---------------------------------------------------------------------------------------

include(../QtWebApp/logging/logging.pri)
include(../QtWebApp/httpserver/httpserver.pri)
include(../QtWebApp/templateengine/templateengine.pri)

