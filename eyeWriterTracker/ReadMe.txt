README 6/3/2010 by ito
EyeWriter Remote Eye Tracker

To use real time eye tracking, you need a camera and IR LEDs and some electronics parts.
For the information for the hardware, see this (for now).
http://collab.eyewriter.org/?p=123
http://collab.eyewriter.org/?p=102

This software is made by openFrameworks and based on the original EyeWriter software.
about openFrameworks, eyeWriter project please check this.
http://www.openframeworks.cc/
http://www.eyewriter.org/

Following openFrameworks AddOns are needed.
- ofxDirList
- ofxVectorMath
- ofxXmlSettings
- ofxOpenCv


Note:

To build it, you have to replace ofxCvContourFinder.cpp in ofxOpenCv folder with the file that comes with this project(in the same folder as this readme.txt file).

I changed only the order of 2 lines in findContour method as below.

- in original file, 

    inputCopy = input;
    inputCopy.setROI( input.getROI() );

- new file 

    // ito changed the order of these 2 lines. (1:setROI , 2:copy)
    inputCopy.setROI( input.getROI() );
    inputCopy = input;

