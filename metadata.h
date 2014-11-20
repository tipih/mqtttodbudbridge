#ifndef METADATA_H
#define METADATA_H
#include <QDBusPendingReply>
#include <QDBusArgument>
#include "tracklisttojsonstring.h"
#include <QList>
#include <meta.h>



#define _trackid "mpris:trackid"
#define _url     "xesam:url"
#define _title   "xesam:title"
#define _artist  "xesam:artist"
#define _album   "xesam:album"
#define _vlctime "vlc:time"
#define _length  "mpris:length"
#define _art     "mpris:artUrl"
#define _vlclength "vlc:length"
#define _vlcpublisher "vlc:publisher"


class MetaData
{
public:
    MetaData();
    static QString getTrackSet(QDBusPendingReply<TrackMetadata>);
    static QList<QDBusObjectPath> getTracklist(QDBusPendingReply<QDBusVariant>);
    static Metadata getMetadata(QVariant value);
    static Metadata getMetadata(QDBusPendingReply<QDBusVariant>);

private:
    static QString getMetaData(TrackMetadata);
    static QList<QDBusObjectPath> decodeArrayType(QDBusArgument, QString&);
    static Metadata decodeMapType(QVariant value, QString&);
    static Metadata decodeMapType(QDBusVariant value, QString&);
};

#endif // METADATA_H

