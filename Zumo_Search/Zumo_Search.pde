// Need G4P library
import g4p_controls.*;
import processing.serial.*;

Serial myPort;  // Create object from Serial class
int buffer = 10;


public void setup(){
  
  String portName = "/dev/tty.usbserial-AL1L30FO"; 
  myPort = new Serial(this,portName , 9600);
  size(680, 720, JAVA2D);
  createGUI();
  customGUI();
  // Place your setup code here
  
}

public void draw(){
    background(230);
  while (myPort.available() > 0){
  String message = myPort.readStringUntil(buffer);
  if(message != null){
    logArea.appendText(message);
    }
  }
  
}

// Use this method to add additional statements
// to customise the GUI controls
public void customGUI(){

}
