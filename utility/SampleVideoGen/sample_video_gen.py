#!/usr/bin/env python3

'''
Sample Video Generator

1. requirements:
    --> Python3.5 or above
    --> FFMPEG executable

2. example:
    --> Generate sample video with default settings:
        python sample_video_gen.py --ffmpegPath ffmpeg/bin --output ./out.h264:
    --> Generate sample video with settings: resolution=1920x1080,framerate=60,duration=300s,pixel_format=yuv422(progressive)
        python sample_video_gen.py --ffmpegPath ffmpeg/bin --width 1920 --height 1080 --rate 60 --duration 300 --format yuv422p --output ./out.h264

'''

import argparse
import subprocess
import os.path

class VideoGenerator(object):
    """Wrapper for FFMPEG to generate sample video for testing purpose.
    Args:
        width: Width of the video
        height: Height of the video
        duration: Duration of the video in seconds
        rate: Frame rate of the video
        format: Pixel format of the video, for a list of all supported formats, run ffmpeg -pix_fmts
        ffmpeg_path: Path to the ffmpeg executable
    """
    
    def __init__(self, width, height, duration, rate, format, ffmpeg_path):
        self.width = width
        self.height = height
        self.duration = duration
        self.rate = rate
        self.format = format
        self.ffmpeg =  os.path.join(ffmpeg_path, 'ffmpeg')

    def Generate(self, output_path):
        """Call FFMPEG to generate sample video and output to the specified path.

        Args:
            output_path (str): The path of the output sample video file to generate.
        Raises:
            CalledProcessError: Failed to run FFMPEG in subprocess.
        """

        filterSource = "testsrc2=duration=%d:size=%dx%d:rate=%d,format=%s" % (self.duration, self.width, self.height, self.rate, self.format)
        subprocess.run([self.ffmpeg, '-f', 'lavfi', '-i', filterSource, output_path])


    def __repr__(self) -> str:
        return '%s:size=%dx%d,frame_rate=%d' % (__class__.__name__, self.width, self.height, self.rate)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Create sample video for testing purpose')
    parser.add_argument('--width', type=int, default=640, help='Specify the width of the image')
    parser.add_argument('--height', type=int, default=480, help='Specify the height of the image')
    parser.add_argument('--duration', type=int, default=60, help='Specify the total video duration in seconds')
    parser.add_argument('--rate', type=int, default=30, help='Specify the frame rate of the video')
    parser.add_argument('--output', type=str, help='Specify the path of the outpu video file', required=True)
    parser.add_argument('--format', type=str, default='yuv420p', help='Specify the pixel format of the video')
    parser.add_argument('--ffmpegPath', type=str, help='Specify the path to the ffmpeg executable', required=True)
    args = parser.parse_args()
    output_file = args.output

    try:
        gen = VideoGenerator(args.width, args.height, args.duration, args.rate, args.format, args.ffmpegPath)
        gen.Generate(output_file)
    except subprocess.CalledProcessError as e:
        print(e.output)
   
