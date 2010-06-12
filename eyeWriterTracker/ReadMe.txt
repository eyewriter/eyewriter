README 6/12/2010 by ito + kyle
EyeWriter Remote Eye Tracker

To use this realtime eye tracking system, you need a camera and IR LEDs and some electronics hardware.
For information on the hardware, please see (for now):
http://collab.eyewriter.org/?p=123
http://collab.eyewriter.org/?p=102

Arduino code for driving the hardware is at /StrobeEye/StrobeEye.pde

This software is made with Open Frameworks and based on the original EyeWriter software.
For more information about Open Frameworks and the Eye Writer project please see:
http://www.openframeworks.cc/
http://www.eyewriter.org/

To compile the code, the following openFrameworks addons are needed:
- ofxDirList
- ofxVectorMath
- ofxXmlSettings
- ofxOpenCv

Note:

1.
To build it, you have to replace ofxCvContourFinder.cpp in ofxOpenCv folder with the file
that comes with this project(in the same folder as this readme.txt file).

I changed only the order of 2 lines in findContour method as below.

- in original file, 

    inputCopy = input;
    inputCopy.setROI( input.getROI() );

- new file 

    // ito changed the order of these 2 lines. (1:setROI , 2:copy)
    inputCopy.setROI( input.getROI() );
    inputCopy = input;

2.
To change the input mode to camera input (movie file input is chosen by default),
change the line 5 of (PROJECT FOLDER)/bin/data/Settings/inputSettings.xml as below.
from:   <mode>1</mode>
to:      <mode>0</mode>


