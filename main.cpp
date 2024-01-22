#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "FingerPrintMgr.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

//    FingerPrintMgr* fp = FINGER_PRINT_MGR;

//    fp->openFingerPrintPort();

    QQmlApplicationEngine engine;  
    QQmlContext* ctx = engine.rootContext();

    ctx->setContextProperty("fpMgr", FINGER_PRINT_MGR);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
