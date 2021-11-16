
#ifndef Playlist_hpp
#define Playlist_hpp

#include "oatpp/core/data/stream/BufferStream.hpp"
#include "oatpp/core/parser/Caret.hpp"

#include <chrono>
#include <list>

class Playlist {
private:
  
  struct RecordMarker {
    v_float64 duration;
    oatpp::String uri;
  };

private:
  std::list<RecordMarker> m_records;
  v_float64 m_totalDuration;
public:
  
  Playlist(const std::list<RecordMarker>& records)
    : m_records(records)
  {
    m_totalDuration = 0;
    for(auto marker : m_records) {
      m_totalDuration += marker.duration;
    }
  }
  
  static Playlist parse(oatpp::parser::Caret& caret);
  static Playlist parseFromFile(const char* filename);
  
  std::shared_ptr<oatpp::data::stream::BufferOutputStream> generateForTime(v_int64 millis, v_int32 numRecords);
  
  
};

#endif /* Playlist_hpp */
