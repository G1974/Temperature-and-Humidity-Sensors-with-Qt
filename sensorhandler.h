#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include <QObject>
#include <QSerialPort>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDebug>

class SensorHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString indoorTemp READ indoorTemp NOTIFY dataChanged)
    Q_PROPERTY(QString indoorHum READ indoorHum NOTIFY dataChanged)
    Q_PROPERTY(QString outdoorTemp READ outdoorTemp NOTIFY forecastChanged)
    Q_PROPERTY(QString outdoorIcon READ outdoorIcon NOTIFY forecastChanged)

public:
    explicit SensorHandler(QObject *parent = nullptr) : QObject(parent) {
        // Serial Setup
        serial = new QSerialPort(this);
        serial->setPortName("COM4");
        serial->setBaudRate(QSerialPort::Baud9600);
        connect(serial, &QSerialPort::readyRead, this, &SensorHandler::readSerial);

        if (serial->open(QIODevice::ReadWrite)) {
            serial->setDataTerminalReady(true);
            serial->setRequestToSend(true);
            qDebug() << "Serial: Connected to COM4";
        }

        // Network Setup
        networkManager = new QNetworkAccessManager(this);
        connect(networkManager, &QNetworkAccessManager::finished, this, &SensorHandler::onForecastReceived);

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &SensorHandler::updateForecast);
        timer->start(1800000); // 30 mins

        updateForecast();
    }

    QString indoorTemp() const { return m_indoorTemp; }
    QString indoorHum() const { return m_indoorHum; }
    QString outdoorTemp() const { return m_outdoorTemp; }
    QString outdoorIcon() const { return m_outdoorIcon; }

public slots:
    void updateForecast() {
        QUrl url("https://api.open-meteo.com/v1/forecast?latitude=37.91&longitude=23.71&current_weather=true");
        networkManager->get(QNetworkRequest(url));
    }

private slots:
    void readSerial() {
        while (serial->canReadLine()) {
            QString data = QString::fromUtf8(serial->readLine()).trimmed();
            QStringList list = data.split(",");
            if (list.size() == 2) {
                m_indoorTemp = list[0] + "°C";
                m_indoorHum = list[1] + "%";
                emit dataChanged();
            }
        }
    }

    void onForecastReceived(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject current = QJsonDocument::fromJson(reply->readAll()).object()["current_weather"].toObject();
            m_outdoorTemp = QString::number(current["temperature"].toDouble()) + "°C";
            m_outdoorIcon = mapWeatherCode(current["weathercode"].toInt());
            emit forecastChanged();
        }
        reply->deleteLater();
    }

private:
    QString mapWeatherCode(int code) {
        if (code == 0) return "☀️";
        if (code <= 3) return "🌤️";
        if (code <= 48) return "🌫️";
        if (code <= 67) return "🌧️";
        if (code <= 77) return "❄️";
        if (code <= 82) return "🌦️";
        if (code >= 95) return "⚡";
        return "☁️";
    }

    QSerialPort *serial;
    QNetworkAccessManager *networkManager;
    QString m_indoorTemp = "--°C", m_indoorHum = "--%", m_outdoorTemp = "--°C", m_outdoorIcon = "☁️";

signals:
    void dataChanged();
    void forecastChanged();
};

#endif