/*
 * MP3_Play_Test.ino
 * A quick start example for Grove-Serial MP3 Player V2.0
 * Note: The MP3 chip of Grove-Serial MP3 Player V2.0 is different from Grove-Serial MP3 Player V1.0
 * Description: This demo let you can send instruction 1-8 to control the Grove-Serial MP3 Player, via the serial port.
 *
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Wuruibin
 * Created Time: Dec 2015
 *
 * Modified By   : Hongtai Liu(lht856@foxmail.com)
 * Modified Time : June 2019
 * Modified Description : Allows users to define hard or soft serial port initialization modules.
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "KT403A_Player.h"

#ifdef __AVR__
#include <SoftwareSerial.h>
SoftwareSerial SSerial(2, 3); // RX, TX
#define COMSerial SSerial
#define ShowSerial Serial 

KT403A<SoftwareSerial> Mp3Player;
#endif

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define COMSerial Serial1
#define ShowSerial SerialUSB 

KT403A<Uart> Mp3Player;
#endif

#ifdef ARDUINO_ARCH_STM32F4
#define COMSerial Serial
#define ShowSerial SerialUSB 

KT403A<HardwareSerial> Mp3Player;
#endif

static uint8_t recv_cmd[8] = {};

void setup() {
  // put your setup code here, to run once:
 
    ShowSerial.begin(9600);
    COMSerial.begin(9600);
    while (!ShowSerial);
    while (!COMSerial);
    Mp3Player.init(COMSerial);
    printMenu();
}

void loop() {
  uint8_t len = 0;
    uint8_t i;

    if(Serial.available())
    {
        char chr = '\0';
        while(chr != '\n')  // Blockly read data from serial monitor
        {
            chr = Serial.read();
            // Serial.print(chr);
            recv_cmd[len++] = chr;        
        }
    }

    if(len > 0)
    {
        // Print reveiced data    
        // Serial.print("Received cmd: ");   
        // for(i = 0; i < len; i++) {
        //     Serial.print(recv_cmd[i]);
        //     Serial.print(" ");
        // }
        // Serial.println();
            
        switch (recv_cmd[0])
        {
            case 'P':
                if(recv_cmd[1] == 'm') 
                {
                    /** 
                      * Play music in "MP3" folder by index 
                      * example:
                      * "Pm1" -> ./MP3/0001.mp3
                    */
                    Mp3Player.playSongMP3(recv_cmd[2] - '0');
                    Serial.print("Play ");
                    Serial.write(recv_cmd[2]);
                    Serial.println(".mp3 in MP3 folder");
                } 
                else if(recv_cmd[1] == 'f')
                {
                    /** 
                      * Play specify folder and music
                      * example:
                      * "Pf11" -> ./01/001***.mp3
                    */
                    Mp3Player.playSongSpecify(recv_cmd[2] - '0', recv_cmd[3] - '0');
                    Serial.print("Play ");
                    Serial.write(recv_cmd[3]);
                    Serial.print("xxx.mp3");
                    Serial.print(" in folder ");
                    Serial.write(recv_cmd[2]);
                    Serial.println();
                    
                } 
                else
                {
                    /** 
                      * Play music by default index
                      * example:
                      * "P1" -> ./***.mp3
                    */                
                    Mp3Player.playSongIndex(recv_cmd[1] - '0');
                    Serial.print("Play xxx.MP3 by index ");
                    Serial.write(recv_cmd[1]);
                    Serial.println();
                }            
                // Serial.println("Specify the music index to play");
                break;
            case 'p':
                Mp3Player.pause();            
                Serial.println("Pause the MP3 player");
                break;
            case 'R':            
                Mp3Player.play();
                Serial.println("Resume the MP3 player");
                break;
            case 'N':            
                Mp3Player.next();
                Serial.println("Play the next song");
                break;
            case 'L':
                Mp3Player.previous();
                Serial.println("Play the previous song");
                break;
            case 'l':
                /** 
                      * Play music by default index
                      * example:
                      * "l1" -> l1/
                */                
                Mp3Player.loopFolder(recv_cmd[1]-'0');
                Serial.print("Play loop for all the songs in the floder");
                Serial.write(recv_cmd[1]);
                Serial.println();
                break;
            case 'I':
                Mp3Player.volumeUp();
                Serial.println("Increase volume");
                break;
            case 'D':
                Mp3Player.volumeDown();
                Serial.println("Decrease volume");
                break;
            default:
                printMenu();
                break;
        }

    }    
    delay(200);
}
void printMenu()
{
  Serial.println("Grove - Serial MP3 Demo");
  Serial.println(
        "Input command:\r\n\r\n"
        "P[ ] play music by default index\r\n"
        "Pm[ ] play music in MP3 folder by index\r\n"
        "Pf[ ][ ] play music by specify folder and index\r\n"        
        "p Pause\r\n"
        "R Resume\r\n"
        "N Next\r\n"
        "L Previous\r\n"
        "l[] LoopFolder\r\n"
        "I Increase volume\r\n"
        "D Decrease volumern\r\n"
        "Other keys for print menu\r\n");
    delay(100);
}