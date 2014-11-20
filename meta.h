#include <QList>
#include <QVariantMap>
#ifndef META_H
#define META_H
typedef QList<QVariantMap> TrackMetadata;
typedef QMap<QString,QString> Metadata;
Q_DECLARE_METATYPE(TrackMetadata)
Q_DECLARE_METATYPE(Metadata)
#endif // META_H

