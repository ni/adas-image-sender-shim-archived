#pragma once

#include <vector>
#include "ImageStreamStructures.h"
#include <imageSenderDll.h>

struct EmbeddedDataLineInternal {
    uint8_t mipiDataType;
    std::vector<uint8_t> data;

    bool operator==(const EmbeddedDataLineInternal& other) const {
        return mipiDataType == other.mipiDataType && data == other.data;
    }

    EmbeddedDataLine ToApi() const {
        return { mipiDataType, data.data(), static_cast<int32_t>(data.size()) };
    }

    static EmbeddedDataLineInternal FromApi(const EmbeddedDataLine& line) {
        EmbeddedDataLineInternal result;
        result.mipiDataType = line.dataType;
        result.data.resize(line.length);
        memcpy(result.data.data(), line.data, line.length);
        return result;
    }
};

inline std::vector<EmbeddedDataLine> EmbeddedDataLineInternalToApi(const std::vector<EmbeddedDataLineInternal>& embeddedLines) {
    std::vector<EmbeddedDataLine> result;
    result.reserve(embeddedLines.size());
    for (const auto& embeddedLine : embeddedLines) {
        result.push_back(embeddedLine.ToApi());
    }
    return result;
}

inline std::vector<EmbeddedDataLineInternal> EmbeddedDataLineInternalFromApi(const std::vector<EmbeddedDataLine>& embeddedLines) {
    std::vector<EmbeddedDataLineInternal> result;
    result.reserve(embeddedLines.size());
    for (const auto& embeddedLine : embeddedLines) {
        result.push_back(EmbeddedDataLineInternal::FromApi(embeddedLine));
    }
    return result;
}

inline size_t GenerateEmbeddedDataStream(const std::vector<EmbeddedDataLineInternal>& embeddedData, uint8_t* output, size_t maxOutputSize) {
    uint8_t* outputPosition = output;
    for(const auto& line : embeddedData) {
        if(outputPosition + sizeof(EmbeddedDataHeader) + line.data.size() > (output + maxOutputSize)) {
            throw std::runtime_error("Maxinum embedded output size reached");
        }
        EmbeddedDataHeader* lineHeader = reinterpret_cast<EmbeddedDataHeader*>(outputPosition);
        lineHeader->dataType = line.mipiDataType;
        lineHeader->bytesPerCurrentLine = static_cast<uint16_t>(line.data.size());
        outputPosition += sizeof(EmbeddedDataHeader);
        memcpy(outputPosition, line.data.data(), line.data.size());
        outputPosition += line.data.size();
    }
    return outputPosition - output;
}

inline std::vector<EmbeddedDataLineInternal> ParseEmbeddedDataStream(const uint8_t* buffer, size_t totalSize) {
    std::vector<EmbeddedDataLineInternal> embeddedData;
    size_t offset = 0;
    while(offset < totalSize) {
        auto lineHeader = reinterpret_cast<const EmbeddedDataHeader*>(&buffer[offset]);
        offset += sizeof(EmbeddedDataHeader);
        EmbeddedDataLineInternal line;
        line.mipiDataType = lineHeader->dataType.value();
        line.data = std::vector<uint8_t>(&buffer[offset], &buffer[offset + lineHeader->bytesPerCurrentLine.value()]);
        offset += lineHeader->bytesPerCurrentLine.value();
        embeddedData.push_back(line);
    }
    return embeddedData;
}

inline size_t CalculateEmbeddedDataStreamSize(const std::vector<EmbeddedDataLineInternal>& embeddedData) {
    size_t size = 0;
    for(const auto& line : embeddedData) {
        size += sizeof(EmbeddedDataHeader) + line.data.size();
    }
    return size;
}

struct OverriddenMetaData {
    uint64_t timestamp;
    std::vector<EmbeddedDataLineInternal> preEmbeddedData;
    std::vector<EmbeddedDataLineInternal> postEmbeddedData;
    std::vector<double> imageScalingCoefficients;
};