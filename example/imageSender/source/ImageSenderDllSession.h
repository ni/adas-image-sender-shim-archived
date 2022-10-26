#pragma once

#include <imageSender/imageSenderDll.h>
#include "ImageMetaData.h"

#define IMAGE_SENDER_THROW_IF_FATAL(code, ...)       \
    {                                                \
        auto capturedCode = (code);                  \
        if(capturedCode != 0) {                      \
            std::cerr << capturedCode << std::endl;  \
        }                                            \
    } \

class ImageSenderDllSession {
    public:
        ImageSenderDllSession() {

        }
        ImageSenderDllSession(ImageSenderDllSession&& other) {
            sessionHandle = other.sessionHandle;
            other.sessionHandle = nullptr;
        };
        ~ImageSenderDllSession() {
            if (sessionHandle) {
                try {
                    Close();
                }
                catch (...) {
                    // ignore
                }
            }
        }
        void Close() {
            if(sessionHandle) {
                IMAGE_SENDER_THROW_IF_FATAL(DestroyImageSenderSession(sessionHandle));
                sessionHandle = nullptr;
            }
        }
        void Create(const InputImageProperties& inputImageProps, const OutputImageProperties& outputImageProps, const RdmaSessionProperties& rdmaSessionProperties) {
            Close();
            IMAGE_SENDER_THROW_IF_FATAL(::CreateImageSenderSession(&inputImageProps, &outputImageProps, &rdmaSessionProperties, &sessionHandle));
        }

        ImageSenderDllSession& operator=(ImageSenderDllSession&& other) {
            if (this != &other) {
                sessionHandle = other.sessionHandle;
                other.sessionHandle = nullptr;
            }
            return *this;
        }

        void AcceptConnection(int32_t timeoutMs) {
            IMAGE_SENDER_THROW_IF_FATAL(::AcceptConnection(sessionHandle, timeoutMs));
        }

        void SendImage(const std::vector<uint8_t> bytes, uint64_t timestamp, std::vector<EmbeddedDataLineInternal>* preEmbeddedLines, std::vector<EmbeddedDataLineInternal>* postEmbeddedLines, int32_t timeoutMs) {
            ImageBlock imageBlock = { 0 };
            imageBlock.imageDataBlock = bytes.data();
            imageBlock.timestampNs = timestamp;

            std::vector<EmbeddedDataLine> preEmbeddedLinesApi;
            std::vector<EmbeddedDataLine> postEmbeddedLinesApi;
            if(preEmbeddedLines && preEmbeddedLines->size()) {
                for(const auto& line : *preEmbeddedLines) {
                    preEmbeddedLinesApi.push_back(line.ToApi());
                }
                imageBlock.preEmbeddedDataLineCount = static_cast<int32_t>(preEmbeddedLines->size());
                imageBlock.preEmbeddedDataLines = preEmbeddedLinesApi.data();
            }
            if(postEmbeddedLines && postEmbeddedLines->size()) {
                for(const auto& line : *postEmbeddedLines) {
                    postEmbeddedLinesApi.push_back(line.ToApi());
                }
                imageBlock.postEmbeddedDataLineCount = static_cast<int32_t>(postEmbeddedLines->size());
                imageBlock.postEmbeddedDataLines = postEmbeddedLinesApi.data();
            }
            SendImageBlock(&imageBlock, timeoutMs);
        }


    protected:

        void SendImageBlock(const ImageBlock* imageBlock, int32_t timeoutMs) {
            IMAGE_SENDER_THROW_IF_FATAL(::SendImageBlock(sessionHandle, imageBlock, timeoutMs));
        }

        ImageSenderSessionHandle sessionHandle = nullptr;
};
