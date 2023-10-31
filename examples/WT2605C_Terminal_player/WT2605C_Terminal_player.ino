#include "WT2605C_Player.h"

#ifdef __AVR__
  #include <SoftwareSerial.h>
  SoftwareSerial SSerial(2, 3); // RX, TX
  #define COMSerial SSerial
  #define ShowSerial Serial

  WT2605C<SoftwareSerial> Mp3Player;
#endif

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define COMSerial Serial1
  #define ShowSerial SerialUSB

  WT2605C<Uart> Mp3Player;
#endif

#ifdef ARDUINO_ARCH_STM32F4
  #define COMSerial Serial
  #define ShowSerial SerialUSB

  WT2605C<HardwareSerial> Mp3Player;
#endif

void setup() {
  while (!ShowSerial);
  ShowSerial.begin(9600);
  COMSerial.begin(115200);
  ShowSerial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++");
  Mp3Player.init(COMSerial);

  ShowSerial.println("0...");
}


void loop() {
  if(ShowSerial.available()) {
    String input = Serial.readString();
    input.trim();
    
    if(input.startsWith("v")) {
      int vol = input.substring(1).toInt();
      int err = Mp3Player.volume(vol);
      ShowSerial.println(err);
      if(!err) ShowSerial.println("Volume set to: " + String(vol));
      else ShowSerial.println("ERROR");
    }
    
    else if(input.startsWith("m")) {
      if(input.substring(1) == "1"){
        ShowSerial.println("1");
        int err = Mp3Player.playMode(0x00);
        ShowSerial.println(err);
        if(!err) ShowSerial.println("The playback mode is set to Loop mode.");
        else ShowSerial.println("ERROR");
      }
      else if(input.substring(1) == "2"){
        ShowSerial.println("2");
        int err = Mp3Player.playMode(0x01);
        ShowSerial.println(err);
        if(!err) ShowSerial.println("The playback mode is set to Single song loop mode.");
        else ShowSerial.println("ERROR");
      }
      else if(input.substring(1) == "3"){
        ShowSerial.println("3");
        int err = Mp3Player.playMode(0x02);
        ShowSerial.println(err);
        if(!err) ShowSerial.println("The playback mode is set to Folder loop mode.");
        else ShowSerial.println("ERROR");
      }
      else if(input.substring(1) == "4"){
        ShowSerial.println("4");
        int err = Mp3Player.playMode(0x03);
        ShowSerial.println(err);
        if(!err) ShowSerial.println("The playback mode is set to Random mode.");
        else ShowSerial.println("ERROR");
      }
      else if(input.substring(1) == "5"){
        ShowSerial.println("5");
        int err = Mp3Player.playMode(0x04);
        ShowSerial.println(err);
        if(!err) ShowSerial.println("The playback mode is set to Single song mode.");
        else ShowSerial.println("ERROR");
      }
    }
    else if(input.startsWith("b")){
      int index = input.substring(1).toInt();
      Mp3Player.playSDRootSong(index);
      ShowSerial.println("Play music: " + String(index));
    }
    else if(input.startsWith("+")){
      int err = Mp3Player.volumeUp();
      if(!err) ShowSerial.println("Volume up");
      else ShowSerial.println("ERROR");
    }
    else if(input.startsWith("-")){
      int err = Mp3Player.volumeDown();
      if(!err) ShowSerial.println("Volume down");
      else ShowSerial.println("ERROR");
    }
    else if(input.startsWith("n")){
      Mp3Player.next();
      ShowSerial.println("Next song");
    }
    else if(input.startsWith("p")){
      Mp3Player.previous();
      ShowSerial.println("Previous song");
    }
  }
}
