#ifndef MediaModalityController_H
#define MediaModalityController_H

#include <QObject>
#include <QtDBus/QDBusInterface>
#include <QtCore/QFile>
#include <stdio.h>
#include <mpreis_interface.h>
#include <QList>





class MediaModalityController : public QObject
{
    Q_OBJECT

public:
    explicit MediaModalityController(QObject *parent = 0);
    QDBusInterface *iface;
    QFile qstdin;
    typedef QMap<QString,QString> Metadata;

signals:
    void setAlbum(QString);
    void setArtist(QString);
    void setTitle(QString);
    void setLength(QString);
    void setPosition(QString);
    void setAlbumArt(QString);
    void setTrackId(QString);
    void setTrackList(QString);
    void setVolume(QString);
    void setShuffle(QString);
    void setLoopStatus(QString);
    void setPlaybackStatus(QString);

private slots:
    void quit();
    void on_TrackAdded(QVariantMap,QDBusObjectPath);
    void on_propertyChange(QString,QVariantMap,QStringList);
    void on_seeked(qlonglong position);
    void init();

public slots:
    void play();
    void pause();
    void previous();
    void next();
    void setVolume(double mVolume);
    void playId(QDBusObjectPath);
    void playPause();
    void mix(bool);

private:
    org::mpris::MediaPlayer2::Player *musicPlayer2PlayerProxy;
    org::mpris::MediaPlayer2::TrackList *musicPlayer2TracklistProxy;
    org::freedesktop::DBus::Properties *musicPlayer2property;
    void timerEvent(QTimerEvent *event);
    Metadata getMetadata(QVariant);
    QList<QDBusObjectPath> getTracklist();
    void sendMetadataUpdate(Metadata);
    void getCurrentPlaylist();
    QString convertTrackLenght(QString);    //converting to milisec
    QString convertTrackPosition(QString);  //converting to milisec
    QString convertLoopStatus(QString);
    QString converPlackbackStatus(QString);
    QStringList _trackList;

    bool isSet;
};

#endif // DBUSBRIDGE_H
