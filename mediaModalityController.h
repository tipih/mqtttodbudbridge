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

private slots:
    void quit();
    void on_TrackAdded(QVariantMap,QDBusObjectPath);
    void on_propertyChange(QString,QVariantMap,QStringList);
    void on_seeked(qlonglong position);

public slots:
    void play();
    void pause();
    void previous();
    void next();
    void Volume(double mVolume);

private:
    org::mpris::MediaPlayer2::Player *musicPlayer2PlayerProxy;
    org::mpris::MediaPlayer2::TrackList *musicPlayer2TracklistProxy;
    org::freedesktop::DBus::Properties *musicPlayer2property;
    void timerEvent(QTimerEvent *event);
    Metadata getMetadata(QVariant);
    QStringList getTracklist();
    void sendMetadataUpdate(Metadata);
    QStringList _trackList;

    bool isSet;
};

#endif // DBUSBRIDGE_H
