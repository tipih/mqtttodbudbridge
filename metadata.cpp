#include "metadata.h"
#include <QDebug>

MetaData::MetaData()
{

}

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



QString MetaData::getTrackSet(QDBusPendingReply<TrackMetadata>metadatalist)
{

    TrackMetadata metaList = metadatalist.value();
    QString tracklist = "";
    tracklist = getMetaData(metaList);
    return tracklist;
}

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
