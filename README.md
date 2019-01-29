# Example-HLS-Media-Stream [![Build Status](https://dev.azure.com/lganzzzo/lganzzzo/_apis/build/status/oatpp.example-hls-media-stream?branchName=master)](https://dev.azure.com/lganzzzo/lganzzzo/_build/latest?definitionId=12&branchName=master)

Example project how-to build HLS-streaming server using oat++ Async-API.

Live stream tested with Safari-Browser and VLC-player

More about oat++:
- Website: [https://oatpp.io](https://oatpp.io)
- Docs: [https://oatpp.io/docs/start](https://oatpp.io/docs/start)
- Oat++ Repo: [https://github.com/oatpp/oatpp](https://github.com/oatpp/oatpp)

## Overview

Server generates infinite .m3u8 playlist from the pre-generated 
```playlist_live.m3u8``` file in the ```video``` folder.  
File ```video/playlist_live.m3u8``` together with video chunks is generated using ```ffmpeg```.  

Server is built using oat++ Async-Api and has following endpoints:

- ```("GET", "/", Root)``` - Starting html-page aka index.html
- ```("GET", "video/*", Video)``` - Embedded video html-page 
- ```("GET", "media/live", Live)``` - Infinite HLS playlist for live-stream describing video chunks
- ```("GET", "media/*", Static)``` - Endpoint serving media files with range requests support

### Project layout

```

|- CMakeLists.txt              // project loader script. load and build dependencies 
|- main/                       // main project directory
|    |
|    |- CMakeLists.txt         // projects CMakeLists.txt
|    |- src/                   // source folder
|    |- test/                  // test folder
|
|- video/                      // media files and playlists here
     |- generate_pls.sh        // example how to use ```ffmpeg``` to generate initial playlist and video chunks
     |- playlist_live.m3u8     // playlist used to generate infinite playlist for http-live-streaming

```
```
- src/
    |
    |- controller/              // Folder containing UserController where all endpoints are declared
    |- hls/                     // Playlist generator is here
    |- Utils.hpp                // Utils
    |- AppComponent.hpp         // Service config
    |- Logger.hpp               // Application Logger
    |- App.cpp                  // main() is here
    
```

---

### Build and Run

#### Using CMake

```
$ mkdir build && cd build
$ cmake ..
$ make run        ## Download, build, and install all dependencies. Run project

```

#### In Docker

```
$ docker build -t example-hls .
$ docker run -p 8000:8000 -t example-hls
```

---

### Screenshots

<img src="https://github.com/oatpp/example-hls-media-stream/blob/master/screenshots/screen-2.png" width="33%"><img src="https://github.com/oatpp/example-hls-media-stream/blob/master/screenshots/screen-3.png" width="33%"><img src="https://github.com/oatpp/example-hls-media-stream/blob/master/screenshots/screen-4.png" width="33%">

---
## Notes

### Urls
- localhost:8000 - Index page
- localhost:8000/media/live - Live stream made from {repo}/video/video1.mp4 and {repo}/video/video2.mp4 played in the loop

### Files
- {repo}/video/playlist_live.m3u8 - playlist used for live HLS streaming
- {repo}/video/generate_pls.sh - example script used to generate sub-playlists and video fragmets using ffmpeg tool. #EXTINF sections of sub-playlists then have to be manualy moved to playlist_live.m3u8.

### Note
If app can't find playlist of video files then specify full-file-paths for {repo}/video/playlist_live.m3u8 file and {repo}/video folder in AppComponent.hpp

