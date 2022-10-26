#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
   extern "C" {
#endif


#if defined(_WIN32)
    #define C_CONV __cdecl
    #if defined(_BUILDING_IMAGE_SENDER_DLL)
        #define _IMPORT_EXPORT_IMAGE_SENDER __declspec(dllexport)
    #else
        #define _IMPORT_EXPORT_IMAGE_SENDER __declspec(dllimport)
    #endif
#else
    #define C_CONV
    #if defined(_BUILDING_IMAGE_SENDER_DLL)
        #define _IMPORT_EXPORT_IMAGE_SENDER __attribute__ ((section (".export")))
    #else
        #define _IMPORT_EXPORT_IMAGE_SENDER
    #endif
#endif
#define  _IMAGE_SENDER_FUNC _IMPORT_EXPORT_IMAGE_SENDER C_CONV

#pragma pack(push, 8)

typedef void* ImageSenderSessionHandle;

// Input formats:
//      BGRA (4x8bpp)
//      RGB (3x8bpp)
//      BGR (3x8bpp)
//      MonodriveBayer (32-bit, MSB is used)
//      NV12 (YUV 4:2:0 planar)
//      YUYV422_10 (10-bit YUV422, unpacked)
//      R210 (10-bit RGB packed into 32-bits)
//      V210 (10-bit YUV packed into 32-bits)
//      24BitFloatAsRGB (24-bit float RAW packed into RGB pixel)
//      32BitFloatBayer (32-bit float RAW)
//
//      Also supports passing any of the output formats
//      as input, to allow for pass-through of data without
//      any conversion.

struct InputImageProperties {
    const char* pixelFormat;
    int32_t width;
    int32_t height;
};

// Output formats:
//      RAW8_BG / RAW8_GR / RAW8_RG / RAW8_GB
//      RAW12_GR / RAW12_BG / RAW12_RG / RAW12_GB
//      YUV422
//      RGB888

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

// Structure provided during back channel event callbacks
struct RdmaBackChannelRequest {
    uint64_t timestampOutput_WorkingTime;
    uint64_t timestampRequested_SystemTime;
    const EmbeddedDataLine* preEmbeddedDataLines;
    int preEmbeddedDataLineCount;
    const EmbeddedDataLine* postEmbeddedDataLines;
    int postEmbeddedDataLineCount;
    const uint8_t* customBackChannelData;
    int customBackChannelDataSize;
    const double* imageDataScaling;
    int32_t imageDataScalingCount;
};

// Callback function for back channel events
typedef void (C_CONV *BackChannelEventCallback)(const RdmaBackChannelRequest* request, void* context);

// Properties for the RDMA connection
struct RdmaSessionProperties {
    const char* localListenAddress;             // Local IP address to listen on
    uint16_t localListenPort;                   // Local port to listen on
    int32_t transferChunkSize;                  // Size of each transfer chunk. Must match receiver configuration
    uint16_t localBackChannelPort;              // Optional backchannel port to listen on
    BackChannelEventCallback backChannelEventCallbackFunction;  // If provided, this function will be called when a back channel event is received
                                                                // The caller is then expected to call SendImageBlock with the data within the callback.
                                                                // If this function is not provided, SendImageBlock will block waiting for a backchannel event (if enabled)
                                                                // and override any meta data coming from the backchannel in the image stream
    void* backChannelEventCallbackContext;      // User-provided context to pass to the back channel event callback function
};

#define imageSender_Error_OutOfMemory       -10001
#define imageSender_Error_GenericError      -10002
#define imageSender_Error_InvalidFormat     -10003
#define imageSender_Error_InvalidSession    -10004

int32_t _IMAGE_SENDER_FUNC CreateImageSenderSession(const InputImageProperties* inputImageProps, const OutputImageProperties* outputImageProps, const RdmaSessionProperties* rdmaSessionProperties, ImageSenderSessionHandle* sessionHandle);
int32_t _IMAGE_SENDER_FUNC AcceptConnection(ImageSenderSessionHandle sessionHandle, int32_t timeoutMs);
int32_t _IMAGE_SENDER_FUNC DestroyImageSenderSession(ImageSenderSessionHandle sessionHandle);
int32_t _IMAGE_SENDER_FUNC SendImageBlock(ImageSenderSessionHandle sessionHandle, const ImageBlock* imageBlock, int32_t timeoutMs);

#pragma pack(pop)


#ifdef __cplusplus
   }
#endif