
#include "Utils.hpp"

#include <fstream>
#include <cstring>
#include <cstdarg>

oatpp::String StaticFilesManager::getExtension(const oatpp::String& filename) {
  v_int32 dotPos = 0;
  for(v_int32 i = filename->size() - 1; i > 0; i--) {
    if(filename->data()[i] == '.') {
      dotPos = i;
      break;
    }
  }
  if(dotPos != 0 && dotPos < filename->size() - 1) {
    return oatpp::String((const char*)&filename->data()[dotPos + 1], filename->size() - dotPos - 1);
  }
  return nullptr;
}

oatpp::String StaticFilesManager::getFile(const oatpp::String& path) {
  if(!path) {
    return nullptr;
  }
  std::lock_guard<oatpp::concurrency::SpinLock> lock(m_lock);
  auto& file = m_cache [path];
  if(file) {
    return file;
  }
  oatpp::String filename = m_basePath + "/" + path;
  file = oatpp::String::loadFromFile(filename->c_str());
  return file;
}

oatpp::String StaticFilesManager::guessMimeType(const oatpp::String& filename) {
  auto extension = getExtension(filename);
  if(extension) {
    
    if(extension == "m3u8"){
      return "application/x-mpegURL";
    } else if(extension == "mp4"){
      return "video/mp4";
    } else if(extension == "ts"){
      return "video/MP2T";
    } else if(extension == "mp3"){
      return "audio/mp3";
    }
    
  }
  return nullptr;
}

oatpp::String formatText(const char* text, ...) {
  char buffer[4097];
  va_list args;
  va_start (args, text);
  vsnprintf(buffer, 4096, text, args);
  va_end(args);
  return oatpp::String(buffer);
}

v_int64 getMillisTickCount(){
  std::chrono::milliseconds millis = std::chrono::duration_cast<std::chrono::milliseconds>
  (std::chrono::system_clock::now().time_since_epoch());
  return millis.count();
}
