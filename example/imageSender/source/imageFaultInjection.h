#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
   extern "C" {
#endif

#pragma pack(push, 8)

enum FaultCallbackType {
   FaultCallbackType_PreConvertedImage       = 1, // Image before conversion - passes ImageDataToFault
   FaultCallbackType_PostConvertedImage      = 2, // Image after conversion - passes ImageDataToFault
   FaultCallbackType_PreEmbeddedDataLines    = 3, // Embedded data lines before the image - passes EmbeddedDataToFault
   FaultCallbackType_PostEmbeddedDataLines   = 4, // Embedded data lines after the image - passes EmbeddedDataToFault
   FaultCallbackType_Timestamp               = 5, // Timestamp manipulation - passes TimestampDataToFault
   FaultCallbackType_FaultFlag               = 6, // Trigger FPGA-based faults - passes FaultFlagToFault
   FaultCallbackType_BackChannelUserData     = 7, // Passes any custom user data given from the backchannel - passes BackChannelUserData
};


struct ImageDataToFault {
   uint64_t timestamp;
   uint64_t frameNumber;
   const char* pixelFormat; // String representing the pixel format (e.g. RAW12_GR, or BGRA)
   int32_t imageWidthInPixels;
   int32_t imageLines;
   void* imagePixelData; // Block containing raw pixels of whatever format is specified in pixelFormat
};

struct EmbeddedDataToFault {
   uint64_t timestamp;
   uint64_t frameNumber;
   int32_t embeddedDataLinesCount;
   // EmbeddedDataLine data
   //    Block containing each line of embedded data:
   //       -little_uint16_buf_t bytesPerCurrentLine;
   //       -little_uint8_buf_t  dataType;
   //       -bytes of embedded data for this line
    void* embeddedDataLines;
};

enum ImageFaultFlag {
   FaultType_NoFault          = 0x00,
   FaultType_DropFrame        = 0x01,    // Support frame drop only for now
   FaultType_CRC              = 0x02
};

struct TimestampDataToFault {
   uint64_t frameNumber;
   uint64_t* timestampToFault; // Pointer to the timestamp to read/modify
};

struct FaultData {
   ImageFaultFlag faultFlag;
   uint8_t* faultInjectionData;
   size_t faultInjectionDataSize;
};

struct FaultDataToFault {
   uint64_t timestamp;
   uint64_t frameNumber;
   FaultData faultData;
};

struct BackChannelUserData {
   uint64_t timestamp;
   uint64_t frameNumber;
   const void* userData;
   size_t userDataSize;
};

#pragma pack(pop)

// Exported function named "FaultInitialization" is called once to initialize the plugin.
// Any application-defined configuration data is passed in the faultConfiguration parameters. The plugin
// can store whatever context specific to this configuration in userContext which will be passed back to future calls to the
// The plugin should return 0 if it is able to initialize successfully
typedef int32_t (*FaultInitializeFunc)(const char* faultConfigurationData, size_t faultConfigurationSize, void** userContext);

// Exported function named "FaultImage" is called on every image to allow it to cause faults prior to transmission.
// The data in imageData can be modified to cause faults. The userContext is the same context that was returned from the FaultInitialization
// The plugin should return 0 if it is able to fault the image successfully
typedef int32_t (*FaultImageFunc)(FaultCallbackType callbackType, void* userContext, void* dataToFault);

// Exported function named "FaultUninitialization" that is called when the plugin is unloaded
typedef void (*FaultUninitializeFunc)(void* userContext);

// If file is included by a client that defines BUILDING_FAULT_INJECTION, we'll set the proper function definitions to be exported
#ifdef BUILDING_FAULT_INJECTION
   #if defined(_WIN32)
      #define EXPORT_FUNC __declspec(dllexport)
   #else
      #define EXPORT_FUNC __attribute__ ((section (".export")))
   #endif
   int32_t EXPORT_FUNC FaultInitialization(const char* faultConfigurationData, size_t faultConfigurationSize, void** userContext);
   int32_t EXPORT_FUNC FaultImage(FaultCallbackType callbackType, void* userContext, void* dataToFault);
   void EXPORT_FUNC FaultUninitialization(void* userContext);
#endif

#ifdef __cplusplus
   }
#endif
