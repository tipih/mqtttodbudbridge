#include "metadata.h"
#include <QDebug>

MetaData::MetaData()
{

}

//Public functions


//Function getMetadata
//Overload function taking in a QVariant containing a
//QDBusArgument.
//Function will return  QMap<QString,QString>
//Coorosponding to a DBus signature a{ss}
Metadata MetaData::getMetadata(QVariant value){
    Metadata _metadata;
    _metadata.clear();

    const QDBusArgument &optionsArgument = qvariant_cast<QDBusArgument>(value);
    QString error;

    //To ensure that we have correct data type as we expect key value
    if(optionsArgument.currentType()==QDBusArgument::MapType)
    {
        _metadata = MetaData::decodeMapType(value, error);
    }else{
        _metadata.insert("error","not map type");
    }

    return _metadata;
}


//Function getMetadata
//This function will process a reply from the DBus and return the metadata
//The metadata looks like this QMap<QString,QString> mapping datra comming
//from DBus

Metadata MetaData::getMetadata(QDBusPendingReply<QDBusVariant> TrackList){
    Metadata _metadata;
    _metadata.clear();
    QString error;


    QDBusVariant argumentList=TrackList.value();
    const QDBusArgument &optionsArgument = qvariant_cast<QDBusArgument>(argumentList.variant());

    qDebug()<<optionsArgument.currentType();
    if (optionsArgument.currentType() == QDBusArgument::MapType)
    {
        _metadata = MetaData::decodeMapType(argumentList,error);
    }
    return _metadata;
}


//function getTrackSet
//This function will convert a DBus reply containing a QList<QVariantMap>
//This is the dataset coorespond to DBus signature aa{sv}
//it will return a string containint all the Data formattet as a jsonstring
QString MetaData::getTrackSet(QDBusPendingReply<TrackMetadata>metadatalist)
{

    TrackMetadata metaList = metadatalist.value();
    QString tracklist = "";
    tracklist = getMetaData(metaList);
    return tracklist;
}


//function getTracklist
//This function will conver a QDBusArgument containing a arraytype
//it will extract data and return it in a QList<QDBusObjectPath>
//coorespond to a DBus to ao
QList<QDBusObjectPath> MetaData::getTracklist(QDBusPendingReply<QDBusVariant>TrackList){
    QDBusVariant argumentList=TrackList.value();
    const QDBusArgument &optionsArgument = qvariant_cast<QDBusArgument>(argumentList.variant());
    QList<QDBusObjectPath> _trackList;
    QString error;

    if(optionsArgument.currentType()==QDBusArgument::ArrayType)
    {
        _trackList= MetaData::decodeArrayType(optionsArgument, error);
    }

    return _trackList;
}



//*********************************************************
//Private functions
//This section is private helper function for decoding


//Function decodeArrayType
//General function for decoding array type and return ao signature
QList<QDBusObjectPath> MetaData::decodeArrayType(QDBusArgument _optionsArgument,QString &error){
    error="false";
    QList<QDBusObjectPath> data;
    data.clear();
    _optionsArgument.beginArray();
    QDBusObjectPath element;
    while ( !_optionsArgument.atEnd() ) {
        _optionsArgument >> element;
        data.append(element);
    }
    _optionsArgument.endArray();
    return data;
}

//Function getMetaData
//This helper function will return a json formatted string
//from a QList<QVariantMap>
QString MetaData::getMetaData(TrackMetadata metaList){

    TrackListToJsonString::clearTrackList();
    TrackListToJsonString::trackSet trackset;

    QVariantMap metadata;

    trackset.album="Unknown";
    trackset.title="Unknown";
    trackset.id="";

    for (int i = 0; i < metaList.size(); ++i) {
        metadata = metaList.at(i);
        for(QVariantMap::const_iterator iter = metadata.begin(); iter != metadata.end(); ++iter) {

            if (iter.key()== "xesam:album"){
                trackset.album=iter.value().toString();
            }
            else if (iter.key()== "xesam:title"){
                trackset.title=iter.value().toString();
            }
            else if (iter.key()== "mpris:trackid"){
                const QDBusObjectPath _path = qvariant_cast<QDBusObjectPath>(iter.value());
                trackset.id=_path.path();
            }
        }
        TrackListToJsonString::addToTrackList(trackset);
    }

    return TrackListToJsonString::getTrackList();
}

//Function decodeMapType
//This helper function will decode a mapType and return
//a QMap<QString,QString> coorespond to a a{ss} DBus signature
Metadata MetaData::decodeMapType(QVariant value, QString &){

    const QDBusArgument &optionsArgument = qvariant_cast<QDBusArgument>(value);
    Metadata _metadata;


    optionsArgument.beginMap();
    while ( !optionsArgument.atEnd() ) {
        QString key;
        QVariant value;
        optionsArgument.beginMapEntry();
        optionsArgument >> key >> value;
        _metadata.insert(key,value.toString());
        optionsArgument.endMapEntry();
    }
    optionsArgument.endMap();
    return _metadata;
}


//Function decodeMapType
//This helper function is a overload mapType decoder
//It takes in QDbusvariant and return a QMap<QString,QString>
//Coorespond to a a{ss}
Metadata MetaData::decodeMapType(QDBusVariant value, QString &){

    Metadata _metadata;
    _metadata.clear();
    const QDBusArgument &optionsArgument = qvariant_cast<QDBusArgument>(value.variant());
    optionsArgument.beginMap();
    while ( !optionsArgument.atEnd() ) {
        QString key;
        QVariant value;
        optionsArgument.beginMapEntry();
        optionsArgument >> key >> value;
        _metadata.insert(key,value.toString());
        optionsArgument.endMapEntry();
    }
    optionsArgument.endMap();
    return _metadata;
}
