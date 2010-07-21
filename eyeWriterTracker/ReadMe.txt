README 7/20/2010 by ito + kyle
EyeWriter Remote Eye Tracker

To use this realtime eye tracking system, you need a camera and IR LEDs and some electronics hardware.
For information on the hardware, please see (for now):
http://collab.eyewriter.org/?p=123
http://collab.eyewriter.org/?p=102

to use PSEYE USB camera with this software, we recommend to use the newest macam driver.
the macam driver can be downloaded from 
http://sourceforge.net/projects/webcam-osx/files/cvs-build/2009-09-25/macam-cvs-build-2009-09-25.zip/download

Arduino code for driving the hardware is at /StrobeEye/StrobeEye.pde

This software is made with Open Frameworks and based on the original EyeWriter software.
For more information about Open Frameworks and the Eye Writer project please see:
http://www.openframeworks.cc/
http://www.eyewriter.org/

To compile the code, the following openFrameworks addons are needed:
- ofxDirList
- ofxVectorMath
- ofxXmlSettings

Note:

To change the input mode to camera input (movie file input is chosen by default),
change the line 5 of (PROJECT FOLDER)/bin/data/Settings/inputSettings.xml as below.
from:   <mode>1</mode>
to:      <mode>0</mode>


