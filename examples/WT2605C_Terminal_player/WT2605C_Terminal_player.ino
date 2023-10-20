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
    ShowSerial.begin(115200);
    COMSerial.begin(115200);
    ShowSerial.println("+++++++++++++++++++++++++++++++++++++++++++++++++++++");
    Mp3Player.init(COMSerial);

    ShowSerial.println("0...");
}

void loop() {
//    if (ShowSerial.available()) {
//        char cmd = ShowSerial.read();
//        switch (cmd) {
//            case '8':
              while(!COMSerial.available()){
                  ShowSerial.print("AT+PLAY=sd0,1\r\n");
                  COMSerial.print("AT+PLAY=sd0,1\r");
                  delay(3000);
              }
              ShowSerial.println(COMSerial.read(),HEX);
              while(1);
//            case '9':
//                ShowSerial.print("stop:");
//                COMSerial.println("AT+PP");
//                while(!COMSerial.available()){}
//                ShowSerial.println(ShowSerial.read());
//                Mp3Player.playSDRootSong(1);
//                ShowSerial.println();
//                break;
//            default:
//                break;
//        }
//    }
  
}

