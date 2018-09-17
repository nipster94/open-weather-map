/*!
* @file owmgui.h
* @brief This is the header file of the owmgui.cpp.
*
* This class will have the properties of QT library and the GUI needed for
* this application is defined in this class. The main purpose of this application
* is to communicate with the OpenWeatherMap server \ref<https://home.openweathermap.org/>
*
* @author Nipun C. Gammanage (mailtonipun94@gmail.com)
*
* @date Initial release - Spet/2018
*/

#ifndef OWMGUI_H
#define OWMGUI_H

#include <QMainWindow>
#include <QGroupBox>
#include <QTextCharFormat>
#include <QThread>

#include "includes/owmapi.h"

/*!
 *  \addtogroup UI Namespace
 *  @{
 */
namespace Ui {
    class OWMGUI;
}// End namespace Records
/*! @} End of Doxygen Groups*/

/**
 * \class OWMGUI
 * \brief A Base Class to handle user inputs (via an UI) and communicate
 * with the OpenWeatherMap APIs
 *
 * This application allows to get data from 3 diffrent APIs and 4 diffrent search
 * option for each API. This class will act as the bridge between the user and the
 * API itself.
 *
 * The class will inherits from QMainWindow to handle UI operations
 *
 */
class OWMGUI : public QMainWindow
{
    Q_OBJECT

    public:
        /**
         * \brief A Constructor
         *
         * To open UI window, to initialize the OWMAPI class and to define
         * trigger SIGNALS and SLOTS to send data between the two classes.
         *
         */
        explicit OWMGUI(int argc, char *argv[],QWidget *parent = 0);
        /**
         * \brief A Destructor that will safely shutdown the UI
         *
         */
        ~OWMGUI();
        /**
         * To start the process. The initial welcome message will be displayed
         * so that the users may use the applications without any difficulties
         * (TODO)
         */
        void execute(void);

        /** \brief ENUM defining search option of the user */
        enum OWMSearchOption{
            SO_IDEAL,   /**< The idealing state. Program has to switch to a diffrent state in order to get info from APIs*/
            BYCITY,     /**< By the city option - case 1*/
            BYCITYID,   /**< By the city id option - case 2*/
            BYLONNLAT,  /**< By the geographic coordinates options (Longitude and Latitude) - case 3*/
            BYZIPCODE   /**< By the zip-code option (Zip code and Country code) - case 4*/
        };

        /** \brief ENUM defining API option of the user */
        enum ApiOption{
            AO_IDEAL,       /**< The idealing state. Program has to switch to a diffrent state in order to get info from APIs*/
            CurrentWeather, /**< Will get data from http://openweathermap.org/current - case 1*/
            ForecastData,   /**< Will get data from http://openweathermap.org/forecast5 - case 2*/
            CMData          /**< Will get data from http://openweathermap.org/api/pollution/co - case 3*/
        };

    Q_SIGNALS:
        void startProcess();
        void startOwmAPI(int,int,QString,QString,QString,QString,QString,QString);

    private slots:
        /**
          * @defgroup radioButtonGroup1 Radio Buttons - API Options
          * All the slots in this group are related to the first group of
          * radio buttons, which associate with the API options.
          * Note that all buttons in this group are Exclusive from each other
          * @{
          */
        /**
          * \brief This slot will be triggered when the current weather
          * data option (http://openweathermap.org/current) is selected.
          */
        void on_curentWeatherData_clicked();
        /**
          * \brief This slot will be triggered when the forecate weather
          * data option (http://openweathermap.org/forecast5) is selected.
          */
        void on_forecastData_clicked();
        /**
          * \brief This slot will be triggered when the Carbon Monoxide
          * data option (http://openweathermap.org/api/pollution/co) is selected.
          */
        void on_cmData_clicked();
        /** @} */

        /**
          * @defgroup radioButtonGroup2 Radio Buttons - Search Options
          * All the slots in this group are related to the second group of
          * radio buttons, which associate with the Search options.
          * Note that all buttons in this group are Exclusive from each other
          * @{
          */
        /**
          * \brief This slot will be triggered when the "Search by City Name"
          * option is selected.
          */
        void on_cityName_clicked();
        /**
          * \brief This slot will be triggered when the "Search by City ID"
          * option is selected.
          */
        void on_cityID_clicked();
        /**
          * \brief This slot will be triggered when the "Search by geographic
          * coordinates options (Longitude and Latitude)" is selected.
          * User needs to enter both values of Longitude and Latitude.
          */
        void on_zipCode_clicked();
        /**
          * \brief This slot will be triggered when the "Search ZIP Code"
          * option is selected.
          * User needs to enter both values of ZIP code and the Country code.
          */
        void on_geographicCoordinates_clicked();
        /** @} */

        /**
          * \brief This slot will be triggered when the push button "OK"
          * option is pressed.
          *
          * In this function it will check weather all the options and data
          * are enterted correctly. If all data are correctly inserted a signal
          * will emit initiating the OWMAPI class. Will pops up an error
          * message box otherwise.
          */
        void on_pushButton_clicked();
        /**
          * \brief This slot will be triggered when the API received its data
          *
          * In this function it will display a message to main displaybox
          * regarding the results of the API search. Will also reset vaules so that
          * user may enter another option.
          */
        void getDetails(QString details);

    private:
        OWMAPI *owmAPI;
        Ui::OWMGUI *ui;
        OWMSearchOption optionSD = SO_IDEAL;
        ApiOption optionAD = AO_IDEAL;

        QString desString;
        QString cityName = NULL;
        QString cityID = NULL;
        QString lon = NULL;
        QString lat = NULL;
        QString zipCode = NULL;
        QString countryCode = NULL;

        void showErrorMsg(QString errorMsg);
};

#endif // OWMGUI_H
