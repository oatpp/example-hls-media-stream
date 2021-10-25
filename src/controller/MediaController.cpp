
#include "MediaController.hpp"

std::shared_ptr<MediaController::OutgoingResponse> MediaController::getStaticFileResponse(const oatpp::String& filename,
                                                                                          const oatpp::String& rangeHeader) const
{

  auto file = staticFileManager->getFile(filename);

  OATPP_ASSERT_HTTP(file.get() != nullptr, Status::CODE_404, "File not found");

  std::shared_ptr<OutgoingResponse> response;

  if(!rangeHeader) {
    response = getFullFileResponse(file);
  } else {
    response = getRangeResponse(rangeHeader, file);
  }

  response->putHeader("Accept-Ranges", "bytes");
  response->putHeader(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
  auto mimeType = staticFileManager->guessMimeType(filename);
  if(mimeType) {
    response->putHeader(Header::CONTENT_TYPE, mimeType);
  } else {
    OATPP_LOGD("Server", "Unknown Mime-Type. Header not set");
  }

  return response;

}

std::shared_ptr<MediaController::OutgoingResponse> MediaController::getFullFileResponse(const oatpp::String& file) const {
  return createResponse(Status::CODE_200, file);
}

std::shared_ptr<MediaController::OutgoingResponse> MediaController::getRangeResponse(const oatpp::String& rangeStr,
                                                                                     const oatpp::String& file) const
{

  auto range = oatpp::web::protocol::http::Range::parse(rangeStr.getPtr());

  if(range.end == 0) {
    range.end = file->size() - 1;
  }

  OATPP_ASSERT_HTTP(range.isValid() &&
                    range.start < file->size() &&
                    range.end > range.start &&
                    range.end < file->size(), Status::CODE_416, "Range is invalid");

  auto chunk = oatpp::String(&file->data()[range.start], (v_int32)(range.end - range.start + 1));

  auto response = createResponse(Status::CODE_206, chunk);

  oatpp::web::protocol::http::ContentRange contentRange(oatpp::web::protocol::http::ContentRange::UNIT_BYTES,
                                                        range.start, range.end, file->size(), true);

  OATPP_LOGD("Server", "range=%s", contentRange.toString()->c_str());

  response->putHeader(Header::CONTENT_RANGE, contentRange.toString());
  return response;

}