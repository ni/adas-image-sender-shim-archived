#pragma once
#include <boost/endian/buffers.hpp>

#pragma pack(push, 1)

struct ImageStreamHeader {
    boost::endian::little_uint32_buf_t totalBlockSize;
    boost::endian::little_uint32_buf_t frameCounter;
    boost::endian::little_uint64_buf_t frameTimestampNs;
    boost::endian::little_uint16_buf_t numLinesPreImageEmbeddedData;
    boost::endian::little_uint16_buf_t numLinesImageData;
    boost::endian::little_uint16_buf_t numLinesPostImageEmbeddedData;
    boost::endian::little_uint16_buf_t numBytesPerEvenLine;
    boost::endian::little_uint16_buf_t numBytesPerOddLine;
    boost::endian::little_uint8_buf_t  mipiImageDataType;
    boost::endian::little_uint8_buf_t  reserved[5];
};
static_assert(sizeof(ImageStreamHeader) == 32, "ImageStreamHeader is not 32 bytes");

struct EmbeddedDataHeader {
    boost::endian::little_uint16_buf_t bytesPerCurrentLine;
    boost::endian::little_uint8_buf_t  dataType;
};


#pragma pack(pop)
