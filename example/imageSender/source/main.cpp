#include <boost/program_options.hpp>
#include <iostream>
#include "ImageSenderDllSession.h"
#include <chrono>

static std::vector<uint8_t> ConstructTestImage(int pixels, int offset);

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

    try {
          // Setup boost to process our command line options
        boost::program_options::variables_map argMap;
        boost::program_options::options_description argDescription("Allowed options");
        boost::program_options::positional_options_description posDesc;
        argDescription.add_options()
            ("help",     "produce help message")
            ("width", boost::program_options::value<int>(&inputProps.width), "Specify the width of the image")
            ("height", boost::program_options::value<int>(&inputProps.height), "Specify the height of the image")
            ("localAddress", boost::program_options::value<std::string>(&localAddress), "Specify the local RDMA IP address to listen on")
            ("localPort", boost::program_options::value<uint16_t>(&localPort), "Specify the local RDMA port number to listen on")
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
        
        ImageSenderDllSession session;
        OutputImageProperties outputProps = { 
            outputFormat.c_str(), 
            preEmbeddedDataLineCount, 
            preEmbeddedDataLinePayloadSize, 
            postEmbeddedDataLineCount, 
            postEmbeddedDataLinePayloadSize};

        RdmaSessionProperties rdmaProps {
            localAddress.c_str(),
            localPort,
            rdmaChunkSize,
            backChannelPort,
            nullptr,
            nullptr};

        session.Create(inputProps, outputProps, rdmaProps);
        std::cout << "Waiting for RDMA connection ..." << std::endl;
        session.AcceptConnection(30000);
        
        // Construct embedded data
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

        session.Close();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

}

std::vector<uint8_t> ConstructTestImage(int pixels, int offset) {
    std::vector<uint8_t> image(pixels * 4);
    for(size_t i = 0; i < image.size(); ++i) {
        image[i] = (i + offset) % 256;
    }
    return std::move(image);
}