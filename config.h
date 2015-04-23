#ifndef CONFIG_H
#define CONFIG_H
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <string>
#include <map>
using std::string;
using std::map;

class Config{
public:
    map <QString,QString> mp;
    static Config * single(){
        static Config * singleton = NULL;
        if (singleton == NULL){
            singleton = new Config();
            const char * config_file = "/Users/gsl/BrainstrokecalServer/conf/config.json";
            QFile fin(config_file);
            fin.open(QIODevice::ReadOnly);
            QJsonParseError json_error;
            QJsonDocument parse_doucment = QJsonDocument::fromJson(fin.readAll(), &json_error);
            if(json_error.error == QJsonParseError::NoError){
                if(parse_doucment.isObject()){
                    QJsonObject obj = parse_doucment.object();
                    for(auto itr = obj.begin(); itr != obj.end(); ++itr){
                        singleton -> mp[itr.key()] = itr.value().toString();
                    }
                }
            }
            return singleton;
        }
        return singleton;
    }

private:
    Config(){

    }
};

#endif // CONFIG_H

        //this place needs to be modified
        /*
        static Config * singleton = NULL;
        if (singleton == NULL){
            singleton = new Config();
            const char * config_file = "/Users/gsl/BrainstrokecalServer/conf/config.json";
            QFile fin(config_file);
            fin.open(QIODevice::ReadOnly);
            QJsonParseError json_error;
            QJsonDocument parse_doucment = QJsonDocument::fromJson(fin.readAll(), &json_error);
            if(json_error.error == QJsonParseError::NoError){
                if(parse_doucment.isObject()){
                    QJsonObject obj = parse_doucment.object();
                    singleton -> pdata_dir = obj.take("pdata_dir").toString().toStdString();
                    singleton -> log_file = obj.take("log_file").toString().toStdString();
                }
            }
            cout <<  singleton -> pdata_dir << " " << singleton -> log_file << endl;
            return singleton;
        }
        return singleton;*/


