/*!
* @file owmapi.h
* @brief This is the header file of the owmapi.cpp.
*
* This class will have the properties of QT Network library.
* The main purpose of this class is to communicate with the
* OpenWeatherMap server \ref<https://home.openweathermap.org/>
*
* This class will grab data from the floowing APIs,
*   1) \ref<http://openweathermap.org/current>         ---> Results will be displayed in the UI
*   2) \ref<http://openweathermap.org/forecast5>       ---> Results will be written in to a excle file
*   3) \ref<http://openweathermap.org/api/pollution/co>---> Results will be written in to a excle file
*
* @author Nipun C. Gammanage (mailtonipun94@gmail.com)
*
* @date Initial release - Spet/2018
*/

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

/**
 * \class OWMAPI
 * \brief A Class to handle communications with the OpenWeatherMap APIs
 *
 * This class contain functions which allows the user to grab data from
 * the OpenWeatherMap server. Function will grab data in the format of JSON.
 *
 * The class will inherits from QThread
 *
 */
class OWMAPI : public QThread{

    Q_OBJECT
    public:
        /**
         * \brief A Constructor
         *
         * To initialize the timers and the network managers.
         *
         */
        OWMAPI(QThread *parent = 0);
        /**
         * \brief A Destructor
         *
         */
        ~OWMAPI();

    Q_SIGNALS:
        /**
          * \brief Q_SIGNAL to send the status/results of the process.
          *
          * \return This function will return a QString which will be
          * handle in the OWMGUI class.
          */
        void sendDetails(QString);

    private slots:
        /**
          * \brief Initiate the QUrl for the API according to user perference.
          *
          * This function will assign the correct url value for the API. Once
          * the url value is set via this function it will start the timer
          * according to the user perference.
          *     \param apiNum (int)                 ---> API option
          *     \param optionNum (int)              ---> Search option
          *     \param cityName (QString) = NULL    ---> ---------------------------------
          *     \param cityID (QString) = NULL      ---> All these 6 values are set to NULL,
          *     \param lon (QString) = NULL         ---> and as per user perference only one
          *     \param lat (QString) = NULL         ---> (or mximum of two values) will be
          *     \param zipCode (QString) = NULL     ---> set
          *     \param countryCode (QString) = NULL ---> ----------------------------------
          *
          */
        void initOwmApi(int apiNum, int optionNum, QString cityName = NULL,
                        QString cityID = NULL,
                        QString lon = NULL , QString lat = NULL,
                        QString zipCode = NULL, QString countryCode = NULL);

        /**
          * @defgroup qNetworkFunctionGroup1 Current weather data - API Callback Functions
          * All the slots in this group are related to the API callbacks
          * of the \ref<http://openweathermap.org/current>
          * @{
          */
        /**
          * \brief This slot will be triggered from the initOwmApi()
          * function. This will call the Current Weather Data API.
          * It will check if there is any QNetworkReply::NetworkError
          * via the slot networkError()
          */
        void checkApiWeather();
        /**
          * \brief Function to grab the Current weather API data
          * \remark This SLOT will be called once the \link mgr_weatherApiCloud \endlink
          *         network manager has a reply from the API. The function will then grab
          *         the necessary data accordingly.
          */
        void apiWeatherInfoResults(QNetworkReply *rep);
        /** @} */

        /**
          * @defgroup qNetworkFunctionGroup2 Forecast Weather Data - API Callback Functions
          * All the slots in this group are related to the API callbacks
          * of the \ref<http://openweathermap.org/forecast5>
          * @{
          */
        /**
          * \brief This slot will be triggered from the initOwmApi()
          * function. This will call the Forecast Weather Data API.
          * It will check if there is any QNetworkReply::NetworkError
          * via the slot networkError()
          */
        void checkApiForecast();
        /**
          * \brief Function to grab the Forecast Weather API data
          * \remark This SLOT will be called once the \link mgr_forcastApiCloud \endlink
          *         network manager has a reply from the API. The function will then grab
          *         the necessary data accordingly.
          */
        void apiForecastInfoResults(QNetworkReply *rep);
        /** @} */

        /**
          * @defgroup qNetworkFunctionGroup1 Carbon Monoxide Data - API Callback Functions
          * All the slots in this group are related to the API callbacks
          * of the \ref<http://openweathermap.org/api/pollution/co>
          * @{
          */
        /**
          * \brief This slot will be triggered from the initOwmApi()
          * function. This will call the Carbon Monoxide Data API.
          * It will check if there is any QNetworkReply::NetworkError
          * via the slot networkError()
          */
        void checkApiCMData();
        /**
          * \brief Function to grab the Carbon Monoxide API data
          * \remark This SLOT will be called once the \link mgr_cmIndexApiCloud \endlink
          *         network manager has a reply from the API. The function will then grab
          *         the necessary data accordingly.
          */
        void apiCMDataInfoResults(QNetworkReply *rep);
        /** @} */

        /**
          * \brief Function will handle netwok errors.
          * (TODO)
          */
        void networkError(QNetworkReply::NetworkError error);

    private:
        QNetworkAccessManager *mgr_weatherApiCloud;                         /**< Network manager for Current Weather data API */
        QTimer *timer_weatherApiCloud;                                      /**< Timer for Current Weather data API */
        QNetworkAccessManager *mgr_forcastApiCloud;                         /**< Network manager for Forecast Weather data API */
        QTimer *timer_forcastApiCloud;                                      /**< Timer for Forecast Weather data API */
        QNetworkAccessManager *mgr_cmIndexApiCloud;                         /**< Network manager for Carbon Monoxide data API */
        QTimer *timer_cmIndexApiCloud;                                      /**< Timer for Carbon Monoxide data API */

        QString getOwmInfoAPI;                                              /**< URL String */
        int frequncy;                                                       /**< Timer frequncy */
        bool lonlat_;                                                       /**< Bool to track the search option state */

        std::string path_ = "/home/nipun/CTK/OpenWeatherMap/lists/";        /**< path for the Excel file */
        std::string cmDataFileName = "Carbon Monoxide Data.csv";            /**< Carbon Monoxide Data file name */
        std::string forecastDataFileName = "Forecast Weather Data.csv";     /**< Forecast Weather Data file name*/
};
#endif
