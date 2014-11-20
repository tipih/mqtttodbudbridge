#include "mediaModalityController.h"
#include <QtCore/QCoreApplication>
#include <QtDBus/QtDBus>
#include <mpreis_interface.h>
#include <imagetobase64.h>
#include <QList>
#include <tracklisttojsonstring.h>
#include <meta.h>
#include "metadata.h"



MediaModalityController::MediaModalityController(QObject *parent) :
    QObject(parent)
{

    qDBusRegisterMetaType<TrackMetadata>();
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
    QTimer::singleShot(100, this, SLOT(init())); //Do to error in VLC mpris on playlist we do this to get it, if there are any
}
void MediaModalityController::on_seeked(qlonglong position){
    emit (setPosition(QString::number(position/1000)));
}

void MediaModalityController::init(){
    getCurrentPlaylist(); //Get the playlist, and after that get the metadata for all elements

    QDBusPendingReply<QDBusVariant> metaData = musicPlayer2property->Get(musicPlayer2PlayerProxy->interface(),"Metadata");
    metaData.waitForFinished();
    if (metaData.isError()){
        QDBusError _error= metaData.error();
        emit(setError((_error.errorString(_error.type())))); //Send error signal to client
    }
    else
    {
        sendMetadataUpdate(MetaData::getMetadata(metaData));
    }


    QDBusPendingReply<QDBusVariant> volume = musicPlayer2property->Get(musicPlayer2PlayerProxy->interface(),"Volume");
    volume.waitForFinished();

    if (volume.isError()){
        QDBusError _error= volume.error();
        emit(setError((_error.errorString(_error.type())))); //Send error signal to client
    }
    else
    {
        QDBusVariant data = volume.value();
        QString _volume = QString::number(data.variant().toDouble()*100);
        emit (setVolume(_volume));
    }

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
            QString _position = QString::number(data.variant().toLongLong()/1000);
            emit (setPosition(_position));
        }

    }
    else
    {

        emit(setError("Not connected to media player")); //Send error signal to client
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
    if(str1==musicPlayer2PlayerProxy->interface()) //Check what interface the property change accured on
    {

        //qDebug()<<"property change Variant "<<variant1;
        for(QVariantMap::const_iterator iter = variant1.begin(); iter != variant1.end(); ++iter) {

            if (iter.key()== "Metadata"){
                sendMetadataUpdate(MetaData::getMetadata(iter.value()));
                getCurrentPlaylist();
            }
            else if (iter.key()=="Position"){
                emit (setPosition(iter.value().toString()));
            }
            else if (iter.key()=="Volume"){
                emit (setVolume(QString::number(iter.value().toFloat()*100)));
            }
            else if (iter.key()=="Shuffle"){
                emit (setShuffle(QString::number(iter.value().toInt())));
            }
            else if (iter.key()=="LoopStatus"){
                emit (setLoopStatus(convertLoopStatus(iter.value().toString())));
            }
            else if (iter.key()=="PlaybackStatus"){
                emit (setPlaybackStatus(converPlackbackStatus(iter.value().toString())));
            }
            else if(iter.key()==""){
            }
        }
    }
    else if(str1==musicPlayer2TracklistProxy->interface()){

        //qDebug()<<"property change Variant "<<variant1;
        QList<QDBusObjectPath> trackListObjectPath = MediaModalityController::getTracklist();

        QDBusPendingReply<TrackMetadata> metadatalist = musicPlayer2TracklistProxy->GetTracksMetadata(trackListObjectPath);
        metadatalist.waitForFinished();
        if (metadatalist.isError()){
            // call failed. Show an error condition.
            qDebug()<<"Something is wrong";

        }
        else
        {
            QString _tracklist = MetaData::getTrackSet(metadatalist);
            emit(setTrackList(_tracklist));
        }
    }
}

void MediaModalityController::getCurrentPlaylist(){
    QList<QDBusObjectPath> trackListObjectPath = MediaModalityController::getTracklist();
    QDBusPendingReply<TrackMetadata> metadatalist = musicPlayer2TracklistProxy->GetTracksMetadata(trackListObjectPath);
    metadatalist.waitForFinished();
    if (metadatalist.isError()){
        // call failed. Show an error condition.
        qDebug()<<"Something is wrong";

    }
    else {
        emit(setTrackList(MetaData::getTrackSet(metadatalist)));
    }
}


QList<QDBusObjectPath> MediaModalityController::getTracklist(){

    QList<QDBusObjectPath> _trackList;
    QDBusPendingReply<QDBusVariant> TrackList = musicPlayer2property->Get(musicPlayer2TracklistProxy->interface(),"Tracks");
    TrackList.waitForFinished();

    if (TrackList.isError()){
        // call failed. Show an error condition.
        TrackList.error();

    }
    else{
        _trackList = MetaData::getTracklist(TrackList);
    }

    return _trackList;
}



void MediaModalityController::quit()
{
    qDebug()<<"Could not connect to dbus service, and we should quit the app, but for now we do not";
    //QCoreApplication::quit();
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
        emit setLength(convertTrackLenght(data.value("mpris:length")));
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

void MediaModalityController::playId(QDBusObjectPath path){
    musicPlayer2TracklistProxy->GoTo(path);
}

void MediaModalityController::playPause(){
    musicPlayer2PlayerProxy->PlayPause();
}

void MediaModalityController::mix(bool mixstatus){

    musicPlayer2PlayerProxy->setShuffle(mixstatus);

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

void MediaModalityController::setVolume(double mVolume){
    qDebug()<<"Signal dBus service to SET VOLUME TO "<<mVolume;
    musicPlayer2PlayerProxy->setVolume(mVolume);
}
//************************************************

//************************************************
//Helper functions
QString MediaModalityController::convertTrackLenght(QString tracklenght){
    bool ok;
    qlonglong _lenght = tracklenght.toLongLong(&ok,10);
    if (!ok){
        return "0";
    }

    _lenght = _lenght / 1000; //Convert to milisec

    return QString::number(_lenght);
}

QString MediaModalityController::convertTrackPosition(QString trackposition){
    bool ok;
    qlonglong _position = trackposition.toLongLong(&ok,10);
    if (!ok){
        return "0"; //If error in convertion return a 0, doing development we should make a exception
    }

    _position = _position / 1000; //Convert to milisec

    return QString::number(_position);
}

QString MediaModalityController::convertLoopStatus(QString loopstatus){
    QString _status;
    _status="0"; //default 0=none
    if (loopstatus=="None")
        _status = "0";
    else if (loopstatus=="Playlist")
        _status="1";
    else if (loopstatus=="Track")
        _status="2";

    return _status;

}

QString MediaModalityController::converPlackbackStatus(QString playbackstatus){
    QString _playbackstatus="stopped";
    if(playbackstatus=="Playing")
        _playbackstatus="playing";
    else if (playbackstatus=="Stopped")
        _playbackstatus="stopped";
    else if (playbackstatus=="Paused")
        _playbackstatus="paused";

    return _playbackstatus;
}
