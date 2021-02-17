Image processing librairy

This custom package handle not compressed Targa 16M colors and PPM 256 gray levels. These are Unix formats where one can easilly acess and modify directly pixels values. The structures are kept to a minimum (height, width, raw datas) and you can perform any operation you want on the pixels values. Here are the classes and functions included in the package.

BaseImg: BaseImg.h and BaseImg.cpp (basic image definition and operations)

    * Put pixels (either in color RVB or black and white)
    * Miror (horizontal, vertical)
    * Transpose
    * Shift luminance
    * Modify contrast
    * Grab
    * Put image in a bigger blank image and smooth borders
    * Invert
    * Smooth
    * Scale
    * Resize (linear approximation)
    * Add noise

PicturePPM: PicturePPM: PicturePPM.h and PicturePPM.cpp (read/write pnm images)
PictureTARGA: PictureTARGA.h and PcitureTARGA.cpp (read/write targa images)
GlobalImg: GlobalImg.h and GlobalImg.cpp (base class definition for GlobalImgTreat)

GlobalImgTreat: GlobalImgTreat.h and GlobalImgTreat.cpp (process images subclass)

    * Add, subtract, multiply, divide images
    * Add, subtract, multiply, divide image by value
    * Convolve images

defImg.h (basic definitions)

Inheritance is as follow
BaseImg
  |
  |___PicturePPM
  |
  |___PictureTARGA
  |
  \___GloabalImg ___ GlobalImgTreat

Contact me if you experience any problem (these functions are also part of the SpikeNET distribution).