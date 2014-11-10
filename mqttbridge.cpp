#include <QDebug>
#include <QTimer>
#include <mqttbridge.h>
#include <qmqtt.h>
#include <mqtt_strings_c.h>



MqttBridge::MqttBridge(QObject *parent) :
    QObject(parent)
{
qDebug()<<"Initilization of the mqtt client";
client = new QMQTT::Client("localhost",1883);

connect(client,SIGNAL(connected()),this,SLOT(connected()));
connect(client,SIGNAL(subscribed(QString)),this,SLOT(subscribed(QString)));
connect(client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));
connect(client,SIGNAL(published(QMQTT::Message&)),this,SLOT(published(QMQTT::Message&)));
connect(client,SIGNAL(received(QMQTT::Message)),this,SLOT(received(QMQTT::Message)));

client->setAutoReconnect(true);
client->setClientId("mqttToDbusBridge");
client->setKeepAlive(100);
client->connect();


}

void MqttBridge::reconnect(){
    if (!client->isConnected()){
      client->connect();
      QTimer::singleShot(2000,this,SLOT(reconnect()));
    }

}

//Private slots functions
void MqttBridge::error(QAbstractSocket::SocketError error){
    qDebug()<<"Socket error " << error;
}

void MqttBridge::connected(){
qDebug()<<"Connected to broaker";
//Now subscribe to the command that the dBus interface supports
client->subscribe(mediaPlayCommand,0);
client->subscribe(mediaVolumeCommand,0);
client->subscribe(mediaNextCommand,0);
client->subscribe(mediaPrevCommand,0);
client->subscribe(mediaPlayPauseCommand,0);
}

void MqttBridge::received(const QMQTT::Message &message){
    //Now we have to pass on the different command to the DBUS command
    //That will be done by emitting signals, as we have no
    //knowlege of the dBus class, We will try to have a clean interface

    qDebug()<<"Message Topic and payload "<<message.topic() << message.payload();
    if      (message.topic()==mediaPlayCommand) emit setPlay();
    else if (message.topic()==mediaNextCommand) emit setNext();
    else if (message.topic()==mediaPrevCommand) emit setPrevious();
    else if (message.topic()==mediaPlayPauseCommand) emit setPause();
    else if (message.topic()==mediaVolumeCommand) {
        double volume = message.payload().toDouble();
        emit setVolume(volume);
        }





}

void MqttBridge::disconnected(){
    qDebug()<<"we got a disconnect, do something smart !";
    QTimer::singleShot(500,this,SLOT(reconnect()));

}

void MqttBridge::published(QMQTT::Message &message){
    qDebug()<<"Published message "<<message.payload();
}

void MqttBridge::subscribed(const QString &topic){
    qDebug()<<"Subscribed to "<<topic;
}

//Public slot functions
void MqttBridge::setAlbum(QString album){
    QMQTT::Message *message = new QMQTT::Message(10,mediaAlbumData,album.toStdString().c_str());

    client->publish(*message);

}

void MqttBridge::setArtist(QString){

}

void MqttBridge::setTitle(QString){

}

void MqttBridge::setLength(QString){

}

void MqttBridge::setPosition(QString){

}

void MqttBridge::setAlbumArt(QString art){
    QMQTT::Message *message = new QMQTT::Message(10,mediaAlbumArtData,art.toStdString().c_str());

    client->publish(*message);
}

void MqttBridge::setTrackId(QString){

}

void MqttBridge::setTrackList(QString){

}
