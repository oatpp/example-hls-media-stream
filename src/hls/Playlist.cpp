
#include "Playlist.hpp"
#include "../Utils.hpp"
#include <cmath>

Playlist Playlist::parse(oatpp::parser::Caret& caret) {

  std::list<RecordMarker> result;
  
  while (caret.canContinue()) {
    caret.findChar('#');
    if(caret.isAtText("#EXTINF:", true)) {
      v_float64 secs = caret.parseFloat64();
      caret.findROrN();
      caret.skipAllRsAndNs();
      auto uriLabel = caret.putLabel();
      caret.findChar('\n');
      result.push_back({secs, oatpp::String(uriLabel.toString())});
    }
    caret.findROrN();
    caret.skipAllRsAndNs();
  }
  
  return Playlist(result);
  
}

Playlist Playlist::parseFromFile(const char* filename) {
  auto text = oatpp::String::loadFromFile(filename);
  if(!text){
    throw std::runtime_error("Can't find playlist file. Make sure you specified full file path's for video and playlist in AppComponent.hpp");
  }
  oatpp::parser::Caret caret(text);
  return parse(caret);
}

std::shared_ptr<oatpp::data::stream::BufferOutputStream> Playlist::generateForTime(v_int64 millis, v_int32 numRecords) {
  if(m_records.size() == 0) {
    throw std::runtime_error("Empty playlist");
  }
  auto stream = std::make_shared<oatpp::data::stream::BufferOutputStream>();
  
  v_float64 secs = (v_float64) millis / 1000.0;
  
  v_int64 rounds = std::floor(secs / m_totalDuration);
  v_float64 loopTime = secs - rounds * m_totalDuration;
  v_float64 currTime = 0;
  
  v_int64 mediaSequence = 1 + rounds * m_records.size();

  auto currRecordNode = m_records.begin();
  auto lastRecordNode = std::prev(m_records.end());
  while(currRecordNode != lastRecordNode) {
    if(currRecordNode->duration + currTime >= loopTime) {
      break;
    }
    currTime += currRecordNode->duration;
    currRecordNode ++;
    mediaSequence ++;
  }
  
  v_float64 targetDuration = 0;
  std::list<RecordMarker> list;
  for(v_int32 i = 0; i < numRecords; i++) {
    auto& marker = *currRecordNode;
    list.push_back(marker);
    
    if(marker.duration > targetDuration) {
      targetDuration = marker.duration;
    }
    
    currRecordNode ++;
    if(currRecordNode == m_records.end()) {
      currRecordNode = m_records.begin();
    }
  }
  
  OATPP_LOGD("playlist", "generating sequence %d", mediaSequence);
  
  *stream
  << "#EXTM3U\n"
  << "#EXT-X-TARGETDURATION:" << (v_int32)(targetDuration + 1) << "\n"
  << "#EXT-X-VERSION:3\n"
  << "#EXT-X-MEDIA-SEQUENCE:" << mediaSequence << "\n";

  for(auto& marker : list) {
    *stream
      << "#EXTINF:" << marker.duration << ",\n"
      << marker.uri << "\n";
  }
  
  return stream;
}
