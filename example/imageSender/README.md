ImageSender allows the users to parse the image from any source like recorded file or Simulator and optionally the embedded Data 

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/fb7fa1d5-e435-4aba-b8ed-d5904bea8941)

# Software Requirement
- CMAKE 3.0.0 or above
- Boost
- Visual Studio Code
- RDMA Mellanox Driver https://network.nvidia.com/products/adapter-software/ethernet/windows/winof-2/

# Hardware Requirement
- PC with Mellanox RDMA Card

# C++ Quick Start:

- imageSenderDLL.h:
  this header file contain the configurations for the image resolution , RDMA backet size , RDMA session properties

this structure define the image size and pixel format like BGRA , RGB NV12 ,  Also supports passing any of the output formats as input, to allow for pass-through of data without any conversion.
```
struct InputImageProperties {
    const char* pixelFormat;
    int32_t width;
    int32_t height;
};
```


This block define the output image properties like pixel format  the variables for embedded , pre embedded data and image block  
```
struct OutputImageProperties {
    const char* pixelFormat;
    int32_t preEmbeddedDataLineCount;
    int32_t preEmbeddedDataLinePayloadSize;
    int32_t postEmbeddedDataLineCount;
    int32_t postEmbeddedDataLinePayloadSize;
};
```

This block define the output image properties like pixel format  , embedded , pre embedded data and image block  
```
struct OutputImageProperties {
    const char* pixelFormat;
    int32_t preEmbeddedDataLineCount;
    int32_t preEmbeddedDataLinePayloadSize;
    int32_t postEmbeddedDataLineCount;
    int32_t postEmbeddedDataLinePayloadSize;
};

struct EmbeddedDataLine {
    uint8_t dataType;
    const uint8_t* data;
    int32_t length;
};

struct ImageBlock {
    const void* imageDataBlock;
    uint64_t timestampNs;
    const EmbeddedDataLine* preEmbeddedDataLines;
    int32_t preEmbeddedDataLineCount;
    const EmbeddedDataLine* postEmbeddedDataLines;
    int32_t postEmbeddedDataLineCount;
    const double* imageDataScaling;
    int32_t imageDataScalingCount;
};
```

This structure contains the configuration for RDMA connection , ```localListenAddress``` is the local IP address for RDMA card , ```localListenPort``` local port for RDMA connection and you can use any available port number , ```transferChunkSize``` is Size of each transfer chunk. Must match receiver configuration , and optionally ```localBackChannelPort``` backchannel port to listen on
```
struct RdmaSessionProperties {
    const char* localListenAddress;             
    uint16_t localListenPort;                   
    int32_t transferChunkSize;                  
    uint16_t localBackChannelPort;              
    BackChannelEventCallback backChannelEventCallbackFunction;                                   
    void* backChannelEventCallbackContext;      
};
```

- Main.cpp:
  Example code that using ```imagesender``` and Programmatically generated image based on linear function and create\send\close RDMA sessions for sending the frames

Code below for initialize the parameters for image properties , RDMA Local IP , RDMA local port , RDMA chunk size , RDMA backchannel port and Preembedded data
```
int main(int argc, char** argv) {
    InputImageProperties inputProps { "BGRA", 640, 480 };
    std::string localAddress;
    uint16_t localPort = 0;
    uint16_t backChannelPort = 0;
    int rdmaChunkSize = 1024*1024;
    int numFrames = INT_MAX;
    std::string outputFormat = "YUV422";
    int32_t preEmbeddedDataLineCount = 0;
    int32_t preEmbeddedDataLinePayloadSize = 0;
    int32_t postEmbeddedDataLineCount = 0;
    int32_t postEmbeddedDataLinePayloadSize = 0;
```

This code for creating command line interface for the example 
```
boost::program_options::variables_map argMap;
        boost::program_options::options_description argDescription("Allowed options");
        boost::program_options::positional_options_description posDesc;
        argDescription.add_options()
            ("help",     "produce help message")
            ("width", boost::program_options::value<int>(&inputProps.width), "Specify the width of the image")
            ("height", boost::program_options::value<int>(&inputProps.height), "Specify the height of the image")
            ("localAddress", boost::program_options::value<std::string>(&localAddress)->required(), "Specify the local RDMA IP address to listen on")
            ("localPort", boost::program_options::value<uint16_t>(&localPort)->required(), "Specify the local RDMA port number to listen on")
            ("backChannelPort", boost::program_options::value<uint16_t>(&backChannelPort), "Specify the local RDMA port number to listen on for backchannel commands")
            ("rdmaChunkSize", boost::program_options::value<int>(&rdmaChunkSize), "RDMA sender chunk size (default 1MB)")
            ("preEmbeddedDataLineCount", boost::program_options::value<int>(&preEmbeddedDataLineCount), "Pre-embedded data line count")
            ("preEmbeddedDataLinePayloadSize", boost::program_options::value<int>(&preEmbeddedDataLinePayloadSize), "Pre-embedded data line payload size")
            ("postEmbeddedDataLineCount", boost::program_options::value<int>(&postEmbeddedDataLineCount), "Post-embedded data line count")
            ("postEmbeddedDataLinePayloadSize", boost::program_options::value<int>(&postEmbeddedDataLinePayloadSize), "Post-embedded data line payload size")
            ("frames", boost::program_options::value<int>(&numFrames), "Specify number of frames to send")
            ("outputFormat", boost::program_options::value<std::string>(&outputFormat), "Specify the output format (e.g. RAW12, YUV422)")
            ("wait", "Waits on exe launch, to allow debugger attach")
        // Since we have all of our parsing options setup we now need to parse our commandline options.
        ;

        boost::program_options::store(
            boost::program_options::command_line_parser(argc, argv)
                .options(argDescription)
                .positional(posDesc)
                .allow_unregistered()
                .run(), argMap);

        if (argMap.count("help")) {
            std::cout << std::endl << "** Options **" << std::endl;
            std::cout << argDescription << "\n";
            return 0;
        }
        if (argMap.count("wait")) {
            std::cout << "Waiting. Press enter to continue... " << std::endl;
            std::cin.get();
        }
        boost::program_options::notify(argMap);
```

This code for initialize RDMA session 
```
 RdmaSessionProperties rdmaProps {
            localAddress.c_str(),
            localPort,
            rdmaChunkSize,
            backChannelPort,
            nullptr,
            nullptr};
```

This code for constructing the embedded data line
```
std::vector<EmbeddedDataLineInternal> preEmbeddedDataLines(outputProps.preEmbeddedDataLineCount);
        for(auto& line : preEmbeddedDataLines) {
            line.mipiDataType = 0x30;
            line.data.resize(outputProps.preEmbeddedDataLinePayloadSize);
            for(size_t i = 0; i < line.data.size(); ++i) {
                line.data[i] = i % 256;
            }
        }
        std::vector<EmbeddedDataLineInternal> postEmbeddedDataLines(outputProps.postEmbeddedDataLineCount);
        for(auto& line : postEmbeddedDataLines) {
            line.mipiDataType = 0x31;
            line.data.resize(outputProps.postEmbeddedDataLinePayloadSize);
            for(size_t i = 0; i < line.data.size(); ++i) {
                line.data[i] = i % 256;
            }
        }
```

This code for constructing the image block and send to RDMA 
```
auto lastUpdateTime = std::chrono::steady_clock::now();
        uint64_t totalFramesProcessed = 0;
        uint64_t framesSinceLastUpdate = 0;
        for (int i = 0; i < numFrames; ++i) {
            auto image = ConstructTestImage(inputProps.width * inputProps.height, i);
            session.SendImage(image, i, &preEmbeddedDataLines, &postEmbeddedDataLines, 30000);

            ++totalFramesProcessed;
            ++framesSinceLastUpdate;
            if (std::chrono::steady_clock::now() - lastUpdateTime > std::chrono::milliseconds(500)) {
                auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastUpdateTime).count();
                std::cout << "Rate: " <<  framesSinceLastUpdate*1000.0f / durationMs << "fps; " << "                       \r";

                framesSinceLastUpdate = 0;
                lastUpdateTime = std::chrono::steady_clock::now();
            }
        }
```

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

7. Use CMAKE to trigger the build , from the root folder run cmake CLI
  ``` 
   $ cmake .
   $ cmake --build ./
   ```
   



# HOW TO BUILD
# Windows
1. Make sure you have CMAKE 3.0.0 or above installed
2. Install boost/program_options library
3. copy ```boost``` to ```local``` folder

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/78121545-b04d-41b5-b0e4-4ab42da1b59c)

4. Add ```boost``` to environment variables 

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/70d10179-1dc7-45f2-85b9-f5641bbc4fcc)

5. Download latest release from https://github.com/ni/adas-replay-hil-image-shim/tags
6. Extract the content in the release to the root of this folder

8. The complete structure should be as follows:

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
9. Use CMAKE to trigger the build
    ```--build ../adas-replay-hil-simulator-sdk-23.5.0d159/example/imageSender --target ALL_BUILD```

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/0cdd8b24-f1fc-4f5b-948c-98419b2f7739)

10. From the generated ```Build``` folder select the solution file 

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/67e86902-e7a0-41d6-8b07-b0c1c6175030)

11. Build the project 

12. Download ```ImageSender.zip``` from https://github.com/ni/adas-replay-hil-simulator-sdk/releases/tag/v23.5.0d102

13. Copy ```imageSender.dll``` to the Debug\Release folder , 

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/50d3dd3c-a7c8-4380-b94e-239b9abd94de)

14. From command line run ```ImageSenderDemo.exe --localAddress xx.xx.xx.xx --localPort xxxxx```

Note -- for localAddress you have to configure the manual IP address for RDMA Mellanox Card , for localPort select the avaliable port 

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/a92190b7-0e62-4849-88cf-6476bdde33dd)

15. RDMA Sending will remain in waiting state until the RDMA receiver start

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/1dad77c8-7fdf-47d4-a6a2-42ab023bff0c)


