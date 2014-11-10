#ifndef MQTTBRIDGE_H
#define MQTTBRIDGE_H

#include <QObject>
#include <qmqtt.h>
class MqttBridge : public QObject
{
    Q_OBJECT


    public:
    explicit MqttBridge(QObject *parent = 0);
    QMQTT::Client *client;


signals:
    void setPlay();
    void setPause();
    void setPrevious();
    void setNext();
    void setVolume(double mVolume);

private slots:
    void error(QAbstractSocket::SocketError);
    void connected();
    void received(const QMQTT::Message &message);
    void disconnected();
    void published(QMQTT::Message &message);
    void subscribed(const QString &topic);
    void reconnect();


public slots:
    void setAlbum(QString);
    void setArtist(QString);
    void setTitle(QString);
    void setLength(QString);
    void setPosition(QString);
    void setAlbumArt(QString);
    void setTrackId(QString);
    void setTrackList(QString);


};

#endif // MQTTBRIDGE_H
