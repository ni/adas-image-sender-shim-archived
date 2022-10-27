# HOW TO BUILD
1. Make sure you have CMAKE 3.0.0 or above installed
2. Install boost/program_options library
3. Download latest release from https://github.com/ni/adas-replay-hil-image-shim/tags
4. Extract the content in the release to the root of this folder
5. If you are on Linux platform, create a symbolic link to the linux shared library

   ``` ImageSender/linux/ImageSenderLib/libimageSender.so -> ImageSender/linux/ImageSenderLib/libimageSender.so.{VERSION}```
6. The complete structure should be as follows:

```
    +--- ImageSender
    |  |--- includes
    |  |  +--- imageSenderDll.h
    |  |--- linux
    |  |  +--- ImageSenderLib
    |  |     |--- libimageSender.so
    |  |     +--- libimageSender.so.{VERSION}
    |  +--- windows
    |     +--- ImageSenderLib
    |        +--- imageSender.lib
    |--- source
    +--- CMakeLists.txt
```
7. Use CMAKE to trigger the build