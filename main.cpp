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
    qDebug()<<"Connecting signal pause "<< QObject::connect(mqttService,SIGNAL(setPause()),MediaControllerService,SLOT(pause()));
    qDebug()<<"Connecting signal previous "<< QObject::connect(mqttService,SIGNAL(setPrevious()),MediaControllerService,SLOT(previous()));
    qDebug()<<"Connecting signal next "<< QObject::connect(mqttService,SIGNAL(setNext()),MediaControllerService,SLOT(next()));
    qDebug()<<"Connecting signal Album "<< QObject::connect(MediaControllerService,SIGNAL(setAlbum(QString)),mqttService,SLOT(setAlbum(QString)));
    qDebug()<<"Connecting signal coverArt"<< QObject::connect(MediaControllerService,SIGNAL(setAlbumArt(QString)),mqttService,SLOT(setAlbumArt(QString)));

    return a.exec();

}

