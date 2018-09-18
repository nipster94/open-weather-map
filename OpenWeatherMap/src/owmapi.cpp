/*!
* @file owmapi.cpp
* @brief This is the source file of the OWMAPI class.
*
* @author Nipun C. Gammanage (mailtonipun94@gmail.com)
*
* @date Initial release - Sept/2018
*/
#include "includes/owmapi.h"

OWMAPI::OWMAPI(QThread *parent)
    : QThread(parent)
{
    timer_weatherApiCloud = new QTimer();
    connect(timer_weatherApiCloud,SIGNAL(timeout()),this,SLOT(checkApiWeather()));
    timer_forcastApiCloud = new QTimer();
    connect(timer_forcastApiCloud,SIGNAL(timeout()),this,SLOT(checkApiForecast()));
    timer_cmIndexApiCloud = new QTimer();
    connect(timer_cmIndexApiCloud,SIGNAL(timeout()),this,SLOT(checkApiCMData()));

    mgr_weatherApiCloud = new QNetworkAccessManager();
    connect(mgr_weatherApiCloud,SIGNAL(finished(QNetworkReply*)),this,SLOT(apiWeatherInfoResults(QNetworkReply*)));
    mgr_forcastApiCloud = new QNetworkAccessManager();
    connect(mgr_forcastApiCloud,SIGNAL(finished(QNetworkReply*)),this,SLOT(apiForecastInfoResults(QNetworkReply*)));
    mgr_cmIndexApiCloud = new QNetworkAccessManager();
    connect(mgr_cmIndexApiCloud,SIGNAL(finished(QNetworkReply*)),this,SLOT(apiCMDataInfoResults(QNetworkReply*)));

    frequncy = 1000;
}

OWMAPI::~OWMAPI(){
    wait();
}

void OWMAPI::initOwmApi(int apiNum, int optionNum, QString cityName, QString cityID, QString lon, QString lat, QString zipCode, QString countryCode){
    bool ignore_ = false;
    bool use_weather_; false;
//    QString apiID = "d4b6588209d1b840fe2d8cb1cf86ac5b";
    QString apiID = "b6907d289e10d714a6e88b30761fae22";
    QString apiOption = "";

    switch (apiNum) {
    case 1:
        apiOption = "weather";
        use_weather_ = true;
        break;
    case 2:
        apiOption = "forecast";
        break;
    case 3:
        ignore_ = true;
        getOwmInfoAPI = QString("https://samples.openweathermap.org/pollution/v1/co/%1,%2/2016-12-25T01:04:08Z.json?appid=%3").arg(lat,lon,apiID);
        timer_cmIndexApiCloud->start(frequncy);
    default:
        break;
    }

    if (!ignore_){
        switch (optionNum) {
        case 1:
            getOwmInfoAPI = QString("https://samples.openweathermap.org/data/2.5/%1?q=%2&appid=%3").arg(apiOption,cityName,apiID);
            break;
        case 2:
            getOwmInfoAPI = QString("https://samples.openweathermap.org/data/2.5/%1?id=%2&appid=%3").arg(apiOption,cityID,apiID);
            break;
        case 3:
            getOwmInfoAPI = QString("https://samples.openweathermap.org/data/2.5/%1?lat=%2&lon=%3&appid=%4").arg(apiOption,lat,lon,apiID);
            lonlat_ = true;
            break;
        case 4:
            getOwmInfoAPI = QString("https://samples.openweathermap.org/data/2.5/%1?zip=%2,%3&appid=%4").arg(apiOption,zipCode,countryCode,apiID);
            break;
        default:
            break;
        }

        if (use_weather_) timer_weatherApiCloud->start(frequncy);
        else timer_forcastApiCloud->start(frequncy);
    }
}

void OWMAPI::checkApiWeather(){
    timer_weatherApiCloud->stop();
    connect(mgr_weatherApiCloud->get(QNetworkRequest(QUrl(getOwmInfoAPI))),
            SIGNAL(error(QNetworkReply::NetworkError)),
            this,
            SLOT(networkError(QNetworkReply::NetworkError)));
}

void OWMAPI::apiWeatherInfoResults(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts);
    if ((str != "") )
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject obj = jsonResponse.object();

        if (!obj.isEmpty()){
            float lon_ = obj["coord"].toObject()["lon"].toDouble();
            float lat_ = obj["coord"].toObject()["lat"].toDouble();

            QString country = obj["sys"].toObject()["country"].toString();
            double sunrise = obj["sys"].toObject()["sunrise"].toDouble();
            double sunset = obj["sys"].toObject()["sunset"].toDouble();

            QString wmain = obj["weather"].toArray()[0].toObject()["main"].toString();
            QString wdescription = obj["weather"].toArray()[0].toObject()["description"].toString();

            double temp = obj["main"].toObject()["temp"].toDouble();
            double humidity = obj["main"].toObject()["humidity"].toDouble();
            double temp_min = obj["main"].toObject()["temp_min"].toDouble();
            double temp_max = obj["main"].toObject()["temp_max"].toDouble();
            double pressure = obj["main"].toObject()["pressure"].toDouble();

            double speed = obj["wind"].toObject()["speed"].toDouble();
            double deg = obj["wind"].toObject()["deg"].toDouble();

            QString extra_ = "";
            if(lonlat_){
                double sea_level = obj["main"].toObject()["sea_level"].toDouble();
                double grnd_level= obj["main"].toObject()["grnd_level"].toDouble();
                extra_ = "\nSea level : " + QString::number(sea_level) + " Ground level : " + QString::number(grnd_level);
            }
            else {
                double visibility = obj["visibility"].toDouble();
                extra_ = "\nVisibility : " + QString::number(visibility);
            }


            QString detailedDescription = "Country : " + country + "\n" +
                                          "City : " + obj["name"].toString() + "\n" +
                                          "Longitude : " + QString::number(lon_) +  " and Latitude : " + QString::number(lat_) + "\n" +
                                          "Weather summary : " + wmain + " Detailed description : " + wdescription + "\n" +
                                          "Temperature : " + QString::number(temp) + " Max temperature : " + QString::number(temp_max) + "\n" +
                                          "Min temperature : " + QString::number(temp_min) + "\n" +
                                          "Humidity : " + QString::number(humidity) + " Pressure : " + QString::number(pressure) + "\n" +
                                          "Wind speed : " + QString::number(speed) + " and angle " + QString::number(deg) + "\n" +
                                          "Sunrise : " + QString::number(sunrise) + " and Sunset : " + QString::number(sunset);

            emit sendDetails(detailedDescription + extra_);
            lonlat_ = false;
        }
    }
}

void OWMAPI::checkApiForecast(){
    connect(mgr_forcastApiCloud->get(QNetworkRequest(QUrl(getOwmInfoAPI))),
            SIGNAL(error(QNetworkReply::NetworkError)),
            this,
            SLOT(networkError(QNetworkReply::NetworkError)));
}

void OWMAPI::apiForecastInfoResults(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts);

    if ((str != "") )
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject obj = jsonResponse.object();
        QFile forecastdataFile(QString::fromStdString(path_ + forecastDataFileName));

        if (!obj.isEmpty()){
            QString name_ = obj["city"].toObject()["name"].toString();
            float lon_ = obj["city"].toObject()["coord"].toObject()["lon"].toDouble();
            float lat_ = obj["city"].toObject()["coord"].toObject()["lat"].toDouble();
            QString country_ = obj["city"].toObject()["country"].toString();
            QString cod_ = obj["cod"].toString();
            float message_ = obj["message"].toDouble();
            float cnt_ = obj["cnt"].toDouble();


            QTextStream stream_(&forecastdataFile);
            if(forecastdataFile.open(QFile::WriteOnly)){
                stream_ << "Country "  << "\t" << country_    << "\n"
                        << "City "     << "\t" << name_       << "\n"
                        << "Location " << "\t" << "Longitude" << "\t" << lon_ << "\n"
                                       << "\t" << "Latitude " << "\t" << lat_ << "\n"
                        << "COD "      << "\t" << cod_        << "\n"
                        << "Message "  << "\t" << message_    << "\n"
                        << "CNT "      << "\t" << cnt_        << "\n";
            }

            stream_ << "List ";

            QJsonArray list = obj["list"].toArray();
            for(int i = 0; i < list.size(); i++){
                double dt_ =  list[i].toObject()["dt"].toDouble();
                double temp_ = list[i].toObject()["main"].toObject()["temp"].toDouble();
                double humidity_ = list[i].toObject()["main"].toObject()["humidity"].toDouble();
                double temp_min_ = list[i].toObject()["main"].toObject()["temp_min"].toDouble();
                double temp_max_ = list[i].toObject()["main"].toObject()["temp_max"].toDouble();
                double pressure_ = list[i].toObject()["main"].toObject()["pressure"].toDouble();
                double grnd_level_ = list[i].toObject()["main"].toObject()["grnd_level"].toDouble();
                double sea_level_ = list[i].toObject()["main"].toObject()["sea_level"].toDouble();
                double temp_kf_ = list[i].toObject()["main"].toObject()["temp_kf"].toDouble();

                QString wmain_ = list[i].toObject()["weather"].toArray()[0].toObject()["main"].toString();
                QString wdescription_ = list[i].toObject()["weather"].toArray()[0].toObject()["description"].toString();

                double clouds_ = list[i].toObject()["clouds"].toObject()["all"].toDouble();
                double speed_ = list[i].toObject()["wind"].toObject()["speed"].toDouble();
                double deg_ = list[i].toObject()["wind"].toObject()["deg"].toDouble();
                QString dt_txt_ = list[i].toObject()["dt_txt"].toString();

                stream_ << "\t" << "dt "           << "\t"   << dt_                    << "\n"
                        << "\t" << "Main Details " << "\t"   << "Temperature"          << "\t" << temp_         << "\n"
                                                   << "\t\t" << "Min Temperature"      << "\t" << temp_min_     << "\n"
                                                   << "\t\t" << "Max Temperature"      << "\t" << temp_max_     << "\n"
                                                   << "\t\t" << "Humidity "            << "\t" << humidity_     << "\n"
                                                   << "\t\t" << "Pressure "            << "\t" << pressure_     << "\n"
                                                   << "\t\t" << "Temp_kf "             << "\t" << temp_kf_      << "\n"
                                                   << "\t\t" << "Sea Level"            << "\t" << sea_level_    << "\n"
                                                   << "\t\t" << "Ground Level"         << "\t" << grnd_level_   << "\n"
                        << "\t" << "Weather "      << "\t"   << "Weather Summary"      << "\t" << wmain_        << "\n"
                                                   << "\t\t" << "Detailed description" << "\t" << wdescription_ << "\n"
                        << "\t" << "Cloudiness"    << "\t"   << clouds_                << "\n"
                        << "\t" << "Wind"          << "\t"   << "Wind Speed"           << "\t" << speed_        << "\n"
                                                   << "\t\t" << "Wind Direction"       << "\t" << deg_          << "\n"
                        << "\t" << "Data/time "    << "\t"   << dt_txt_                << "\n";
            }

            forecastdataFile.close();
        }
    }
    timer_forcastApiCloud->stop();
    emit sendDetails("Weather forecast of 5 days has been written \n OpenWeatherMap/lists/Forecast Weather Data.csv");
}

void OWMAPI::checkApiCMData(){
    qDebug() << getOwmInfoAPI;
    connect(mgr_cmIndexApiCloud->get(QNetworkRequest(QUrl(getOwmInfoAPI))),
            SIGNAL(error(QNetworkReply::NetworkError)),
            this,
            SLOT(networkError(QNetworkReply::NetworkError)));

}

void OWMAPI::apiCMDataInfoResults(QNetworkReply *rep){
    QByteArray bts = rep->readAll();
    QString str(bts);

    if ((str != "") )
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());
        QJsonObject obj = jsonResponse.object();
        QFile CMdataFile(QString::fromStdString(path_ + cmDataFileName));

        if (!obj.isEmpty()){
            QString time_ = obj["time"].toString();
            double lon_ = obj["location"].toObject()["longitude"].toDouble();
            double lat_ = obj["location"].toObject()["latitude"].toDouble();

            QTextStream stream_(&CMdataFile);
            if(CMdataFile.open(QFile::WriteOnly)){
                stream_ << "Time " << "\t" << time_ << "\n"
                        << "Location " << "\t" << "Longitude" << "\t" << lon_ << "\n"
                                       << "\t" << "Latitude " << "\t" << lat_ << "\n" ;
            }
            stream_ << "Data ";

            QJsonArray list = obj["data"].toArray();
            for(int i = 0; i < list.size(); i++){
                double pressure_ =  list[i].toObject()["pressure"].toDouble();
                float precision_ = list[i].toObject()["precision"].toDouble();
                double value_ = list[i].toObject()["value"].toDouble();

                stream_ << "\t" << "Index " << "\t" <<i << "\n"
                        << "\t" << "Pressure" << "\t" << pressure_ << "\n"
                        << "\t" << "Precision " << "\t" << precision_ << "\n"
                        << "\t" << "Value " << "\t" << value_ << "\n" ;
            }

            CMdataFile.close();
        }
    }
    timer_cmIndexApiCloud->stop();
    emit sendDetails("Carbon Monoxide Data has been written \n OpenWeatherMap/lists/Carbon Monoxide Data.csv");
}

void OWMAPI::networkError(QNetworkReply::NetworkError error){
    if(error != QNetworkReply::NetworkError::NoError){
        qDebug() << error;
        emit sendDetails("ERROR " + error);
    }

    disconnect(QObject::sender(),
               SIGNAL(error(QNetworkReply::NetworkError)),
               this,
               SLOT(networkError(QNetworkReply::NetworkError)));
}
