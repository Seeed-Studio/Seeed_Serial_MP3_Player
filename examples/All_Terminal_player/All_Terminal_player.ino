/**
 * The MIT License (MIT)
 * 
 * Author: Hongtai Liu (lht856@foxmail.com)
 * 
 * Copyright (C) 2019  Seeed Technology Co.,Ltd. 
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
#include "SeeedGroveMP3.h"
#include "KT403A_Player.h"
#include "WT2003S_Player.h"

#include <SoftwareSerial.h>
SoftwareSerial SSerial(2, 3); // RX, TX
#define COMSerial SSerial
#define ShowSerial Serial 

// uncomment the chip what you choose
//MP3Player<WT2003S<SoftwareSerial>> Mp3Player;
MP3Player<KT403A<SoftwareSerial>> Mp3Player;


static uint8_t recv_cmd[8] = {};

void setup() {
  
  // put your setup code here, to run once:
 
    ShowSerial.begin(9600);
    COMSerial.begin(9600);
    while (!ShowSerial);
    while (!COMSerial);
    delay(100);
    Mp3Player.controller->init(COMSerial);
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
            case 'p':
                Mp3Player.controller->pause_or_play();            
                Serial.println("Pause or Resume the MP3 player");
                break;
            case 'n':            
                Mp3Player.controller->next();
                Serial.println("Play the next song");
                break;
            case 'l':
                Mp3Player.controller->previous();
                Serial.println("Play the previous song");
                break;
           case '+':
				ShowSerial.print("Volume Raise: ");
				Mp3Player.controller->volumeUp();
				break;
			case '-':
				ShowSerial.println("Volume down: ");
				Mp3Player.controller->volumeDown();         
				break;
            default:
				printMenu();
                break;
        }

    }    
    delay(100);
    
}

void printMenu( void )
{
  ShowSerial.println("MP3 Command List:");
  ShowSerial.println("-----------------");
  ShowSerial.println("'+' or '-'  : raise/lower volume");
  ShowSerial.println("'p'         : play or pause");
  ShowSerial.println("'n'         : next song");
  ShowSerial.println("'l'         : next song");
  ShowSerial.println("Any other key to show this menu");
  ShowSerial.println();
}