# HOW TO BUILD
1. Make sure you have CMAKE 3.0.0 or above installed
2. Install boost/program_options library
3. copy ```boost``` to ```local``` folder

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/78121545-b04d-41b5-b0e4-4ab42da1b59c)

4. Add ```boost``` to environment variables 

![image](https://github.com/ni/adas-replay-hil-simulator-sdk/assets/95634231/70d10179-1dc7-45f2-85b9-f5641bbc4fcc)

5. Download latest release from https://github.com/ni/adas-replay-hil-image-shim/tags
6. Extract the content in the release to the root of this folder
7. If you are on Linux platform, create a symbolic link to the linux shared library

   ``` ImageSender/linux/ImageSenderLib/libimageSender.so -> ImageSender/linux/ImageSenderLib/libimageSender.so.{VERSION}```
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


