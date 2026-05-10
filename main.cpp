#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "sensorhandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SensorHandler handler;
    QQmlApplicationEngine engine;

    // Σύνδεση της C++ με το QML
    engine.rootContext()->setContextProperty("sensorHandler", &handler);

    // Προσοχή στη διαδρομή του αρχείου σου!
   // const QUrl url(u"qrc:/qt/qml/Sensors/Main.qml"_qs);
    // Πιο απλός και σίγουρος τρόπος για να μην βγάζει warnings
    const QUrl url(QStringLiteral("qrc:/qt/qml/Sensors/Main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}