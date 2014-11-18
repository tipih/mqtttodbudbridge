#include <QCoreApplication>
#include <mediaModalityController.h>
#include <mqttbridge.h>
#include <imagetobase64.h>




int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    MediaModalityController* MediaControllerService = new MediaModalityController();
    MqttBridge* mqttService = new MqttBridge();

    qDebug()<<"Connecting signal play "<< QObject::connect(mqttService,SIGNAL(setPlay()),MediaControllerService,SLOT(play()));
    qDebug()<<"Connecting signal playId "<< QObject::connect(mqttService,SIGNAL(setPlayId(QDBusObjectPath)),MediaControllerService,SLOT(playId(QDBusObjectPath)));
    qDebug()<<"Connecting signal pause "<< QObject::connect(mqttService,SIGNAL(setPause()),MediaControllerService,SLOT(pause()));
    qDebug()<<"Connecting signal playpause "<< QObject::connect(mqttService,SIGNAL(setPlayPause()),MediaControllerService,SLOT(playPause()));
    qDebug()<<"Connecting signal previous "<< QObject::connect(mqttService,SIGNAL(setPrevious()),MediaControllerService,SLOT(previous()));
    qDebug()<<"Connecting signal next "<< QObject::connect(mqttService,SIGNAL(setNext()),MediaControllerService,SLOT(next()));
    qDebug()<<"Connecting signal volume "<< QObject::connect(mqttService,SIGNAL(setVolume(double)),MediaControllerService,SLOT(setVolume(double)));
    qDebug()<<"Connecting signal mix "<< QObject::connect(mqttService,SIGNAL(setShuffle(bool)),MediaControllerService,SLOT(mix(bool)));









//    void setTrackId(QString);
//    void setTrackList(QString);
    qDebug()<<"Connecting signal Album "<< QObject::connect(MediaControllerService,SIGNAL(setAlbum(QString)),mqttService,SLOT(setAlbum(QString)));
    qDebug()<<"Connecting signal coverArt"<< QObject::connect(MediaControllerService,SIGNAL(setAlbumArt(QString)),mqttService,SLOT(setAlbumArt(QString)));
    qDebug()<<"Connecting signal Title"<< QObject::connect(MediaControllerService,SIGNAL(setTitle(QString)),mqttService,SLOT(setTitle(QString)));
    qDebug()<<"Connecting signal Artist"<< QObject::connect(MediaControllerService,SIGNAL(setArtist(QString)),mqttService,SLOT(setArtist(QString)));
    qDebug()<<"Connecting signal Length"<< QObject::connect(MediaControllerService,SIGNAL(setLength(QString)),mqttService,SLOT(setLength(QString)));
    qDebug()<<"Connecting signal Position"<< QObject::connect(MediaControllerService,SIGNAL(setPosition(QString)),mqttService,SLOT(setPosition(QString)));
    qDebug()<<"Connecting signal Volume"<< QObject::connect(MediaControllerService,SIGNAL(setVolume(QString)),mqttService,SLOT(setVolume(QString)));
    qDebug()<<"Connecting signal Shuffle"<< QObject::connect(MediaControllerService,SIGNAL(setShuffle(QString)),mqttService,SLOT(setShuffle(QString)));
    qDebug()<<"Connecting signal LoopStatus"<< QObject::connect(MediaControllerService,SIGNAL(setLoopStatus(QString)),mqttService,SLOT(setLoopStatus(QString)));
    qDebug()<<"Connecting signal PlaybackStatus"<< QObject::connect(MediaControllerService,SIGNAL(setPlaybackStatus(QString)),mqttService,SLOT(setPlaybackStatus(QString)));
    qDebug()<<"Connecting signal setTrackList"<< QObject::connect(MediaControllerService,SIGNAL(setTrackList(QString)),mqttService,SLOT(setTrackList(QString)));






    return a.exec();

}

