# Programming Things Individual Assignment 
# Zumo Search & Rescue

This is a project for our first semester assignment for programming things. We are tasked to program a Zumo-Shield robot with Arduino board which needs to navigate itself along the corridor and when reaches to a room search for survivors and report back.

The GUI is designed by G4P builder and we are using processing software to send commands using an XBee chip to the Zumo's XBee receiver.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

## Prequisites
1-Install Arduino software from here:

[Arduino IDE-For Windows XP and up](https://www.arduino.cc/download_handler.php?f=/arduino-1.8.8-windows.zip)

[Arduino IDE-Mac OS X  10.8 Mountain Lion or newer](https://www.arduino.cc/download_handler.php?f=/arduino-1.8.8-macosx.zip)

[Linux  64 bits](https://www.arduino.cc/download_handler.php?f=/arduino-1.8.8-linux64.tar.xz)

2-Install processing from here:

[Processing-For Windows XP and up](http://download.processing.org/processing-3.4-windows64.zip)

[Arduino IDE-Mac OS X  10.8 Mountain Lion or newer](http://download.processing.org/processing-3.4-macosx.zip)

[Linux  64 bits](http://download.processing.org/processing-3.4-linux64.tgz)

3-Download G4P builder Libaries for our GUI:

[Download G4P library files](http://www.lagers.org.uk/g4p/download.html)


### Installing

 - Download the project Zip-File.

ORRR

- Clone the repo with:

```
https://github.com/Sahandsi/Final-Year-Zumo.git
```

Install the Arduino and processing.
Open processing and in tools tab search for G4P and install it.

### For G4P Builder
Paste the G4P library files you download to:

-   For mac users the sketchbook folder is located inside ~/Documents/Processing.
-   For windows users the sketchbook folder is located inside folder 'My Documents'/Processing

The folder structure for tool GUI Builder should be as follows :

```
Processing
-tools
    -G4PTool
      -data
      -examples
      -reference
      -src  
	      -tool 
	          -G4P.jar
	          -G4PTool.jar
```
After tool has been successfully installed, restart processing.

### For Zumo

Open Arduino-IDE and in tools navigate to manage libraries.

 - search for zumo shield and install it. 
 - search for newping and install it.
 
 After all the applications were installed conenct the XBee shield and use Arduino IDE to push the code to it.
 Make sure that robot is in broadcast mode otherwise it will not work.
 This goes for the Arduino if the robot is on broadcast mode the code will not be pushed with Arduino IDE


## Built With

* Processing
* Arduino

## Authors

* **Sahand Simaei**


