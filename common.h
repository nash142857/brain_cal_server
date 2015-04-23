#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <string>
#include <vector>
#include "QTSource.h"
#include <cstring>
#include <string>
#include <fstream>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <uuid/uuid.h>
using namespace std;
template <typename T> void delete_arr_if_not_null(T * a){
    if (a != NULL){
        delete [] a;
    }
}

namespace commutil {
    inline QString make_ret_msg(bool flg, QString msg){
        QJsonObject json;
        if(flg){
            json.insert("status","success");
        }
        else{
            json.insert("status","fail");
        }
        json.insert("content", msg);
        QJsonDocument document;
        document.setObject(json);
        return QString(document.toJson(QJsonDocument::Indented));
    }
    inline QString generate_uuid(){
        uuid_t uuid;
        //The UUID is 16 bytes (128 bits) long
        uuid_generate(reinterpret_cast<unsigned char *>(&uuid));
        char tmp_str[5];
        memset(tmp_str, 0, sizeof(tmp_str));
        QString res;
        for(int i = 0; i < 16;++i){
            sprintf(tmp_str,"%x",uuid[i]);
            res.append(tmp_str);
        }
        res.insert(8,"-");
        res.insert(13,"-");
        res.insert(18,"-");
        res.insert(23,"-");
        return res;
    }
}

#endif
