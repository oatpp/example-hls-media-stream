
#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "hls/Playlist.hpp"
#include "Utils.hpp"

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

#include "oatpp/json/ObjectMapper.hpp"

#include "oatpp/macro/component.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:

  /**
   * Create Async Executor
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
    return std::make_shared<oatpp::async::Executor>(
      4 /* Data-Processing threads */,
      1 /* I/O threads */,
      1 /* Timer threads */
    );
  }());

  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    /* non_blocking connections should be used with AsyncHttpConnectionHandler for AsyncIO */
    return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 8000, oatpp::network::Address::IP_4});
  }());
  
  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor); // get Async executor component
    return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
  }());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    return std::make_shared<oatpp::json::ObjectMapper>();
  }());
  
  OATPP_CREATE_COMPONENT(std::shared_ptr<StaticFilesManager>, staticFilesManager)([] {
    return std::make_shared<StaticFilesManager>(EXAMPLE_MEDIA_FOLDER /* path to '<this-repo>/Media-Stream/video' folder. Put full, absolute path here */) ;
  }());
  
  OATPP_CREATE_COMPONENT(std::shared_ptr<Playlist>, livePlayList)([] {
    auto playlist = Playlist::parseFromFile(EXAMPLE_MEDIA_FOLDER "/playlist_live.m3u8" /* path to '<this-repo>/Media-Stream/video/playlist_live.m3u8' file. Put full, absolute path here */);
    return std::make_shared<Playlist>(playlist);
  }());

};

#endif /* AppComponent_hpp */
