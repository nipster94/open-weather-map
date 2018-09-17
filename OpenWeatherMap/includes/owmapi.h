#ifndef OWMAPI_H
#define OWMAPI_H

#include <QJsonDocument>
#include <QNetworkReply>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QThread>
#include <string>
#include <iostream>

class OWMAPI : public QThread{

    Q_OBJECT
    public:
        OWMAPI(QThread *parent = 0);
        ~OWMAPI();

    Q_SIGNALS:
        void sendDetails(QString);

    private slots:
        void initOwmApi(int apiNum, int optionNum, QString cityName = NULL,
                        QString cityID = NULL,
                        QString lon = NULL , QString lat = NULL,
                        QString zipCode = NULL, QString countryCode = NULL);

        void checkApiWeather();
        void apiWeatherInfoResults(QNetworkReply *rep);
        void checkApiForecast();
        void apiForecastInfoResults(QNetworkReply *rep);
        void checkApiCMData();
        void apiCMDataInfoResults(QNetworkReply *rep);
        void networkError(QNetworkReply::NetworkError error);

    private:
        QNetworkAccessManager *mgr_weatherApiCloud;
        QTimer *timer_weatherApiCloud;

        QNetworkAccessManager *mgr_forcastApiCloud;
        QTimer *timer_forcastApiCloud;

        QNetworkAccessManager *mgr_cmIndexApiCloud;
        QTimer *timer_cmIndexApiCloud;

        QString getOwmInfoAPI;

        int frequncy;
        bool lonlat_;

        std::string path_ = "/home/nipun/CTK/OpenWeatherMap/lists/";
        std::string cmDataFileName = "Carbon Monoxide Data.csv";
        std::string forecastDataFileName = "Forecast Weather Data.csv";
};
#endif
