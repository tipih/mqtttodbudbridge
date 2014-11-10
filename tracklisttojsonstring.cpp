#include "tracklisttojsonstring.h"
#include <QStringListIterator>
#include <QDebug>


TrackListToJsonString::TrackListToJsonString(QObject *parent) :
    QObject(parent)
{


}

//This is currently the plintaformat
QString TrackListToJsonString::encodeToJason(QStringList _tracks){

    QString tracklist;
    bool firsttime=true;
    tracklist.append("{\"trackList\": [");
    QStringListIterator tracks(_tracks);
        while (tracks.hasNext()){
        //TODO Get metadata from the track id in the stringlist
        if (firsttime==false) tracklist.append(",");
        tracklist.append("{");
        tracklist.append("\"trackId\":");
        tracklist.append("\""+tracks.next()+"\""+",");

        tracklist.append("\"trackName\":");
        tracklist.append("\"someTrackName\"");
        tracklist.append(",");

        tracklist.append("\"albumName\":");
        tracklist.append("\"someAlbumName\"");
        tracklist.append("}");
        firsttime=false;
        }
        tracklist.append("]}");
        qDebug()<<tracklist;
        return tracklist;

}
