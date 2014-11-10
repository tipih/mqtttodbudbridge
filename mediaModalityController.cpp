#include "mediaModalityController.h"
#include <QtCore/QCoreApplication>
#include <QtDBus/QtDBus>
#include <mpreis_interface.h>
#include <imagetobase64.h>
#include <QList>
#include <tracklisttojsonstring.h>


MediaModalityController::MediaModalityController(QObject *parent) :
    QObject(parent)
{

    musicPlayer2PlayerProxy = new org::mpris::MediaPlayer2::Player("org.mpris.MediaPlayer2.vlc", "/org/mpris/MediaPlayer2",
                                                                    QDBusConnection::sessionBus(), this);

    musicPlayer2property = new org::freedesktop::DBus::Properties("org.mpris.MediaPlayer2.vlc", "/org/mpris/MediaPlayer2",
                                                                   QDBusConnection::sessionBus(), this);

    musicPlayer2TracklistProxy = new org::mpris::MediaPlayer2::TrackList("org.mpris.MediaPlayer2.vlc", "/org/mpris/MediaPlayer2",
                                                                          QDBusConnection::sessionBus(), this);

    startTimer(1000);
    qDebug()<<"Signal Track added connected" <<connect(musicPlayer2TracklistProxy,SIGNAL(TrackAdded(QVariantMap,QDBusObjectPath)),this,SLOT(on_TrackAdded(QVariantMap,QDBusObjectPath)));
    qDebug()<<"Signal PropertiesChanged connected"<<connect(musicPlayer2property,SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)),this,SLOT(on_propertyChange(QString,QVariantMap,QStringList)));
    qDebug()<<"Signal Seeked connected"<<connect(musicPlayer2PlayerProxy,SIGNAL(Seeked(qlonglong)),this,SLOT(on_seeked(qlonglong)));
    //Flag used only for debugging, to prevent from printing out more
    //than one time
    isSet=true;

}
void MediaModalityController::on_seeked(qlonglong position){
    qDebug()<<"Seeked "<<position;
}


void MediaModalityController::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (musicPlayer2PlayerProxy->isValid())
       {
        if(isSet==true)
        qDebug()<<"DBus Connection to mpris ok";

        isSet=false;
        QDBusPendingReply<QDBusVariant> position = musicPlayer2property->Get(musicPlayer2PlayerProxy->interface(),"Position");
        position.waitForFinished();

            if (position.isError())
                // call failed. Show an error condition.
                position.error();
            else
                // use the returned value
            {
            QDBusVariant data = position.value();
            //qDebug()<<data.variant();
            }

    }
    else{
        if(isSet==false)
        qDebug()<<"DBus connection to mpris NOT ok";

        isSet=true;

    }
}

void MediaModalityController::on_TrackAdded(QVariantMap _map, QDBusObjectPath _path)
{
  Q_UNUSED(_path);
    qDebug()<<"Track Added "<< _map;
}

void MediaModalityController::on_propertyChange(QString str1, QVariantMap variant1, QStringList list1){
Q_UNUSED(list1);



    if(str1==musicPlayer2PlayerProxy->interface())
    {

        //qDebug()<<"property change Variant "<<variant1;
        for(QVariantMap::const_iterator iter = variant1.begin(); iter != variant1.end(); ++iter) {
        qDebug() << iter.key() << iter.value();
            if (iter.key()== "Metadata"){
                sendMetadataUpdate(getMetadata(iter.value()));

                //TODO call helper function to send the updated meta data to mqtt using the plinta protecol
            }
            else if(iter.key()==""){

            }
       }
    }
    else if(str1==musicPlayer2TracklistProxy->interface()){

        //qDebug()<<"property change Variant "<<variant1;
        QStringList trackList=MediaModalityController::getTracklist();
        emit setTrackList(TrackListToJsonString::encodeToJason(trackList));

    }
}


QStringList MediaModalityController::getTracklist(){
    QStringList test;

    QDBusPendingReply<QDBusVariant> TrackList = musicPlayer2property->Get(musicPlayer2TracklistProxy->interface(),"Tracks");
    TrackList.waitForFinished();

        if (TrackList.isError())
            // call failed. Show an error condition.
            TrackList.error();
        else
        {    // use the returned value

       QDBusVariant argumentList=TrackList.value();

       qDebug()<<"Stuff "<<argumentList.variant();

       const QDBusArgument &optionsArgument = qvariant_cast<QDBusArgument>(argumentList.variant());
       qDebug()<<"Type "<<optionsArgument.currentType();

       if(optionsArgument.currentType()==QDBusArgument::ArrayType)
            {
            _trackList.clear();
            optionsArgument.beginArray();
            QString element;
            while ( !optionsArgument.atEnd() ) {
             optionsArgument >> element;
             _trackList.append(element);
             }
            optionsArgument.endArray();
            }
       }

return _trackList;
}



void MediaModalityController::quit()
{
    qDebug()<<"Could not connect to dbus service, and we should quit the app, but for now we do not";
    //QCoreApplication::quit();
}

//************************************************
//helper function to get data from property
MediaModalityController::Metadata MediaModalityController::getMetadata(QVariant value){
    MediaModalityController::Metadata _metadata;
    const QDBusArgument &optionsArgument = qvariant_cast<QDBusArgument>(value);

  //qDebug()<<"Type "<<optionsArgument.currentType();
  //qDebug()<<"map "<<QDBusArgument::MapType;

    //To ensure that we have correct data type as we expect key value
    if(optionsArgument.currentType()==QDBusArgument::MapType)
    {
    optionsArgument.beginMap();
    while ( !optionsArgument.atEnd() ) {
    QString key;
    QVariant value;
            optionsArgument.beginMapEntry();
                optionsArgument >> key >> value;
                //qDebug()<<"Key "<<key<<" Value "<<value;
                _metadata.insert(key,value.toString());
            optionsArgument.endMapEntry();
        }
    optionsArgument.endMap();
    }
    return _metadata;
}

//************************************************

void MediaModalityController::sendMetadataUpdate(Metadata data){
    //Get image from metadata and convert it to base 64 string
    QString albumArtString;
    QString static oldString;
    if (oldString!=data.value("mpris:artUrl"))
    {
     if(!ImageToBase64::convertImage(data.value("mpris:artUrl"),albumArtString)){
                 qDebug()<<"Error in sending album art "<<ImageToBase64::getLastError();
     }
     else{
         emit setAlbumArt(albumArtString);
         oldString=data.value("mpris:artUrl");
     }
    }
    if(data.value("xesam:album")!=""){
       emit setAlbum(data.value("xesam:album"));
    }
    if(data.value("xesam:artist")!=""){
      emit setArtist(data.value("xesam:artist"));
    }
    if(data.value("xesam:title")!=""){
      emit setTitle(data.value("xesam:title"));
    }
    if(data.value("mpris:length")!=""){
      emit setLength(data.value("mpris:length"));
    }
    if(data.value("mpris:trackid")!=""){
      emit setLength(data.value("mpris:trackid"));
    }
}

//************************************************
//Slot metode for control
void MediaModalityController::play(){
    qDebug()<<"Signal dBus service to PLAY";
    musicPlayer2PlayerProxy->Play();
}

void MediaModalityController::pause(){
    qDebug()<<"Signal dBus service to PAUSE";
    musicPlayer2PlayerProxy->Pause();
}

void MediaModalityController::previous(){
    qDebug()<<"Signal dBus service to PREVIOUS";
    musicPlayer2PlayerProxy->Previous();
}

void MediaModalityController::next(){
    qDebug()<<"Signal dBus service to NEXT";
    musicPlayer2PlayerProxy->Next();
}

void MediaModalityController::Volume(double mVolume){
    qDebug()<<"Signal dBus service to SET VOLUME TO "<<mVolume;
    musicPlayer2PlayerProxy->setVolume(mVolume);
}
//************************************************
