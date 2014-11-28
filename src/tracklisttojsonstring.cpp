#include "tracklisttojsonstring.h"
#include <QStringListIterator>
#include <QDebug>

QList<TrackListToJsonString::trackSet> TrackListToJsonString::trackList;

TrackListToJsonString::TrackListToJsonString(QObject *parent) :
    QObject(parent)
{


}

void TrackListToJsonString::clearTrackList(){

   TrackListToJsonString::trackList.clear();
}

void TrackListToJsonString::addToTrackList(trackSet trackset){
    TrackListToJsonString::trackList.append(trackset);
}

QString TrackListToJsonString::getTrackList(){
 QString _tracklist;
 bool firsttime=true;
 QListIterator<trackSet> i(TrackListToJsonString::trackList);
 TrackListToJsonString::trackSet _trackset;
 _tracklist.append("{\"trackList\": [");
 while (i.hasNext()){
     //TODO Get metadata from the track id in the stringlist
     if (firsttime==false) _tracklist.append(",");
     _trackset=i.next();
     _tracklist.append("{");
     _tracklist.append("\"trackId\":");
     _tracklist.append("\""+_trackset.id+"\""+",");

     _tracklist.append("\"titleName\":");
     _tracklist.append("\""+_trackset.title+"\"");
     _tracklist.append(",");

     _tracklist.append("\"albumName\":");
     _tracklist.append("\""+_trackset.album+"\"");
     _tracklist.append("}");
     firsttime=false;
 }
 _tracklist.append("]}");


 return _tracklist;
}

