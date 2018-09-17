#include "includes/owmgui.h"
#include "ui_owmgui.h"

OWMGUI::OWMGUI(int argc, char *argv[],QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OWMGUI)
{   
    ui->setupUi(this);

    owmAPI = new OWMAPI();
    connect(this,SIGNAL(startOwmAPI(int,int,QString,QString,QString,QString,QString,QString)),owmAPI,
            SLOT(initOwmApi(int,int,QString,QString,QString,QString,QString,QString)));
    connect(owmAPI,SIGNAL(sendDetails(QString)),this,SLOT(getDetails(QString)));

}

OWMGUI::~OWMGUI()
{
    delete ui;
}

void OWMGUI::execute(){

    std::string temp = "*** Open Weather Map *** \n \
    The following application was created inorder to  \n \
    collect WEATHER data from OpenWeatherData website \n \
    Please select what Weather Data Type you  \n \
    wish to obtain and what is your Search Option. \n\n \
    Please note that these are these are Exclusive values, \n \
    therefore you may only select one value from each group \n\n \
    Once finished please press OK";

    desString = QString::fromStdString(temp);
    ui->mainlabel->setText(desString);

}

void OWMGUI::on_curentWeatherData_clicked()
{
    optionAD = CurrentWeather;
}

void OWMGUI::on_forecastData_clicked()
{
    optionAD = ForecastData;
}

void OWMGUI::on_cmData_clicked()
{
    ui->mainlabel->setText("You have selected the option to \n \
                           get data from Cabon Monoxide Data API \n \
                           Please ENTER Longitude and Latitude");
    optionAD = CMData;
    optionSD = BYLONNLAT;
    ui->geographicCoordinates->setChecked(true);
    ui->sublabel1->setText("Enter Longitude");
    ui->sublabel2->setText("Enter Latitude");
}

void OWMGUI::on_cityName_clicked()
{
    if (optionAD != CMData){
        optionSD = BYCITY;
        ui->sublabel1->setText("Enter the City Name");
        desString = "You have selected City Name option \n \
        Please Enter the City name";
        ui->mainlabel->setText(desString);
    }
    else {
        showErrorMsg("You have selected the option to \n \
                     get data from Cabon Monoxide Data API \n \
                     Please ENTER Longitude and Latitude");
    }
}

void OWMGUI::on_cityID_clicked()
{
    if (optionAD != CMData){
        optionSD = BYCITYID;
        ui->sublabel1->setText("Enter the City ID");
        desString = "You have selected City ID option \n \
        Please Enter the City ID";
        ui->mainlabel->setText(desString);
    }
    else {
        showErrorMsg("You have selected the option to \n \
                     get data from Cabon Monoxide Data API \n \
                     Please ENTER Longitude and Latitude");
    }

}

void OWMGUI::on_geographicCoordinates_clicked()
{
    optionSD = BYLONNLAT;
    ui->sublabel1->setText("Enter Longitude");
    ui->sublabel2->setText("Enter Latitude");
    desString = "You have selected Geograpic coordinate option \n \
                Please Enter the values of Longitude and Latitude";
    ui->mainlabel->setText(desString);
}

void OWMGUI::on_zipCode_clicked()
{
    if (optionAD != CMData){
        optionSD = BYZIPCODE;
        ui->sublabel1->setText("Enter Zip Code");
        ui->sublabel2->setText("Enter Country code");
        desString = "You have selected Zip code option \n \
        Please Enter the Zip code";
        ui->mainlabel->setText(desString);
    }
    else {
        showErrorMsg("You have selected the option to \n \
                     get data from Cabon Monoxide Data API \n \
                     Please ENTER Longitude and Latitude");
    }
}

void OWMGUI::on_pushButton_clicked()
{
    if (optionAD != AO_IDEAL && optionSD != SO_IDEAL){
        QString valInTextBox1 = ui->textEdit1->toPlainText();
        QString valInTextBox2 = ui->textEdit2->toPlainText();

        int case_a = 0;
        int case_s = 0;

        switch (optionAD) {
        case CurrentWeather:
            case_a = 1;
            break;
        case ForecastData:
            case_a = 2;
            break;
        case CMData:
            case_a = 3;
            break;
        default:
            break;
        }

        if (!valInTextBox1.isEmpty()){
            switch (optionSD) {
            case BYCITY:
                case_s = 1;
                cityName = valInTextBox1;
                break;
            case BYCITYID:
                case_s = 2;
                cityID = valInTextBox1;
                break;
            case BYLONNLAT:
                if(!valInTextBox2.isEmpty()){
                    case_s = 3;
                    lon = valInTextBox1;
                    lat = valInTextBox2;
                }
                break;
            case BYZIPCODE:
                if(!valInTextBox2.isEmpty()){
                    case_s = 4;
                    zipCode = valInTextBox1;
                    countryCode = valInTextBox2;
                }
                break;
            default:
                break;
            }
            emit startOwmAPI(case_a,case_s,cityName,cityID,lon,lat,zipCode,countryCode);
        }
        else showErrorMsg("Desired value have not been entered");

    }
    else {
        showErrorMsg("Please select one option from each group");
    }


}

void OWMGUI::getDetails(QString details){
    ui->mainlabel->setText(details);
    ui->sublabel1->setText("");
    ui->sublabel2->setText("");
    ui->textEdit1->clear();
    ui->textEdit2->clear();

    switch (optionAD) {
    case CurrentWeather:
        ui->curentWeatherData->setAutoExclusive(false);
        ui->curentWeatherData->setChecked(false);
        ui->curentWeatherData->setAutoExclusive(true);
        break;
    case ForecastData:
        ui->forecastData->setAutoExclusive(false);
        ui->forecastData->setChecked(false);
        ui->forecastData->setAutoExclusive(true);
    case CMData:
        ui->cmData->setAutoExclusive(false);
        ui->cmData->setChecked(false);
        ui->cmData->setAutoExclusive(true);
    default:
        break;
    }

    switch (optionSD) {
    case BYCITY:
        ui->cityName->setAutoExclusive(false);
        ui->cityName->setChecked(false);
        ui->cityName->setAutoExclusive(true);
        break;
    case BYCITYID:
        ui->cityID->setAutoExclusive(false);
        ui->cityID->setChecked(false);
        ui->cityID->setAutoExclusive(true);
        break;
    case BYLONNLAT:
        ui->geographicCoordinates->setAutoExclusive(false);
        ui->geographicCoordinates->setChecked(false);
        ui->geographicCoordinates->setAutoExclusive(true);
        break;
    case BYZIPCODE:
        ui->zipCode->setAutoExclusive(false);
        ui->zipCode->setChecked(false);
        ui->zipCode->setAutoExclusive(true);
        break;
    default:
        break;
    }

    cityName.clear();
    cityID.clear();
    lon.clear();
    lat.clear();
    zipCode.clear();
    countryCode.clear();
    optionAD = AO_IDEAL;
    optionSD = SO_IDEAL;
}

void OWMGUI::showErrorMsg(QString errorMsg){
    QMessageBox msgBox;
    msgBox.setText(errorMsg);
    msgBox.exec();
}
