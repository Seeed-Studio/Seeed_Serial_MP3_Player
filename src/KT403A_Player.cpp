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
#include "KT403A_Player.h"

template <class T>
KT403A<T>::KT403A()
{
    _busyPin = -1;
    _status = 0;
}

template <class T>
void KT403A<T>::init(T &serialPort,uint8_t device)
{
    _serial = &serialPort;
    this->setDevice(device);
    this->volume(0x0E);
}


template <class T>
void KT403A<T>::init(T &serialPort, uint8_t pin, uint8_t device)
{
    _serial = &serialPort;
    _busyPin = pin;
    this->setDevice(device);
    this->volume(0x0E);
}


/**************************************************************** 
 * Function Name: sendCommand
 * Description: send command  to KT403A
 * Parameters: commandLength:uint8_t, *data:uint8_t*, len:uint8_t
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::sendCommand(uint8_t commandLength, uint8_t *data, uint8_t len)
{
  #if KT403A_FEEDBACK_CODE
  long time;
  uint8_t recv[11]; //receve 
  uint8_t index = 0;
  uint8_t  again_count = 0;
  #endif
  uint8_t delayTime = 20; 
    
again: 
    //Clear anything in the buffer
    while(_serial->available() > 0) {
        _serial->read();
    }
    _serial->write(KT403A_CMD_START_CODE);
    _serial->write(KT403A_VERSION_CODE);
    _serial->write(commandLength + 2); //Add one byte for 'length', one for CRC
    for (byte x = 0; x < commandLength; x++) //Length + command code + parameter
    {
        _serial->write(commandBytes[x]); //Send this byte
    }
    _serial->write(KT403A_CMD_END_CODE);
	
	delay(delayTime);


    if(commandBytes[1] == KT403A_SET_DEVICE){ // if the cmd is set device.
      delay(200);
    }
    
    // feedback on
    #if KT403A_FEEDBACK_CODE
    time = millis();
    index = 0;
	
    while(_serial->available() && ((millis() - time) < KT403A_TIMEOUT)){
      recv[index++] = _serial->read();
      if(index == 10){
        index = 0;
		delay(40);
      }
    }
    if(recv[3] == 0x41){
       //Serial.println("pass");
       return 0;
    }else{
      if(again_count == 3) 
         return -1;
      again_count++;
      delayTime+=10;
     // Serial.println("again");
      goto again;
     }
    #endif
    
    return 0;
}

/**************************************************************** 
 * Function Name: setDevice
 * Description: Select the player device, U DISK or SD card.
 * Parameters: 0x01:U DISK;  0x02:SD card
 * Return:  0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t  KT403A<T>::setDevice(uint8_t device)
{
    commandBytes[0] = KT403A_SET_DEVICE;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = 0x00;
    commandBytes[3] = device;
    return sendCommand(4, NULL, 0);
}

/**************************************************************** 
 * Function Name: volume
 * Description: Set the volume, the range is 0x00 to 0x1E.
 * Parameters: volume: the range is 0x00 to 0x1E.
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::volume(uint8_t volume)
{
   if(volume > KT403A_MAX_VOLUME)
      volume == KT403A_MAX_VOLUME;
      
   commandBytes[0] = KT403A_SET_VLOUME_VALUE;
   commandBytes[1] = KT403A_FEEDBACK_CODE;
   commandBytes[2] = 0x00;
   commandBytes[3] = volume;
   return sendCommand(4, NULL, 0);
}

/**************************************************************** 
 * Function Name: volumeDown
 * Description: Decrease the volume.
 * Parameters: none
 * Return:  0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::volumeDown()
{ 
   commandBytes[0] = KT403A_SET_VOLUME_DOWN;
   commandBytes[1] = KT403A_FEEDBACK_CODE;
   commandBytes[2] = 0x00;
   commandBytes[3] = 0x00;
   return sendCommand(4, NULL, 0);
}

/**************************************************************** 
 * Function Name: volumeUp
 * Description: Increase the volume.
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::volumeUp()
{ 
   commandBytes[0] = KT403A_SET_VOLUME_UP;
   commandBytes[1] = KT403A_FEEDBACK_CODE;
   commandBytes[2] = 0x00;
   commandBytes[3] = 0x00;
   return sendCommand(4, NULL, 0);
}


/**************************************************************** 
 * Function Name: playSongSpecify
 * Description: Specify the music index in the folder to play, the index is decided by the input sequence of the music.
 * Parameters: folder: folder name, must be number;  index: the music index.
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::playSongSpecify(uint8_t floder, uint8_t index)
{
    commandBytes[0] = KT403A_PLAY_SONG_SPECIFY;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)floder;
    commandBytes[3] = (uint8_t)index;
    if(sendCommand(4, NULL, 0) == 0){
      _status = 1;
      return 0;
    }else{
      return -1;
    }
}

/**************************************************************** 
 * Function Name: playSongIndex
 * Description: Specify the music index to play, the index is decided by the input sequence of the music.
 * Parameters: index: the music index.
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::playSongIndex(uint16_t index)
{
    uint8_t hbyte, lbyte;
    hbyte = index / 256;
    lbyte = index % 256;
    commandBytes[0] = KT403A_PLAY_SPECIFIC_TRACK;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)hbyte;
    commandBytes[3] = (uint8_t)lbyte;
    if(sendCommand(4, NULL, 0) == 0){
      _status = 1;
      return 0;
    }else{
      return -1;
    }
}


/*************************************************************
 * Function Name: playSongMP3
 * Description: Plays the music specified in the MP3 folder.
 *              First create a folder named MP3. Then rename the music file to 0001.mp3,0002.mp3, and so on. Save these music files in the MP3 folder.
 *              The name must be Decimal. 
 * Parameters: index, the name of MP3 flie.
 * Return:  0:success -1:fail
**************************************************************/
template <class T>
uint8_t KT403A<T>::playSongMP3(uint16_t index)
{
    uint8_t hbyte, lbyte;
    hbyte = index / 256;
    lbyte = index % 256;
    commandBytes[0] = KT403A_PLAY_SONG_MP3;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)hbyte;
    commandBytes[3] = (uint8_t)lbyte;
    if(sendCommand(4, NULL, 0) == 0){
      _status = 1;
      return 0;
    }else{
      return -1;
    }
}

/*************************************************************
 * Function Name: playSongADVERT
 * Description: Plays the music specified in the Advert folder.
 *              First create a folder named MP3. Then rename the music file to 0001.mp3,0002.mp3, and so on. Save these music files in the Advert folder.
 *              The name must be Decimal. 
 * Parameters: index, the name of MP3 flie.
 * Return:  0:success -1:fail
**************************************************************/
template <class T>
uint8_t KT403A<T>::playSongADVERT(uint16_t index)
{
    uint8_t hbyte, lbyte;
    hbyte = index / 256;
    lbyte = index % 256;
    commandBytes[0] = KT403A_PLAY_SONG_ADVERT;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)hbyte;
    commandBytes[3] = (uint8_t)lbyte;
    if(sendCommand(4, NULL, 0) == 0){
      _status = 1;
      return 0;
    }else{
      return -1;
    }
}

/**************************************************************** 
 * Function Name: stop
 * Description: stop the current song.
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::stop()
{
    commandBytes[0] = KT403A_STOP;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)0x00;
    if(sendCommand(4, NULL, 0) == 0){
      _status = 0;
      return 0;
    }else{
      return -1;
    }
}

/**************************************************************** 
 * Function Name: pause
 * Description: pause the current song.
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::pause()
{
    commandBytes[0] = KT403A_PAUSE;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)0x00;
    if(sendCommand(4, NULL, 0) == 0){
      _status = 0;
      return 0;
    }else{
      return -1;
    }
}

/**************************************************************** 
 * Function Name: play
 * Description: play the current song.
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::play()
{
    commandBytes[0] = KT403A_PLAY;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)0x00;
    if(sendCommand(4, NULL, 0) == 0){
      _status = 1;
      return 0;
    }else{
      return -1;
    }
}

/**************************************************************** 
 * Function Name: pause_or_play
 * Description: play or pause the current song.
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::pause_or_play()
{
   if(_status){
      return this->pause();
   }else{
      return this->play();
   }
}

/**************************************************************** 
 * Function Name: next
 * Description: play the next song.
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::next()
{
    commandBytes[0] = KT403A_PLAY_NEXT_SONG;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)0x00;
    return sendCommand(4, NULL, 0);
}

/**************************************************************** 
 * Function Name: previous
 * Description: play the previous song.
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::previous()
{
    commandBytes[0] = KT403A_PLAY_PRRVIOUS_SONG;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)0x00;
    return sendCommand(4, NULL, 0);
}

/**************************************************************** 
 * Function Name: loop
 * Description: Loop all music
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::loop(uint8_t state)
{
    commandBytes[0] = KT403A_SET_ALL_LOOP;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)state;
    return sendCommand(4, NULL, 0);
}

/**************************************************************** 
 * Function Name: loopFolder
 * Description: Loop all music in the specified folder
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::loopFolder(uint8_t floder)
{
    commandBytes[0] = KT403A_SET_LOOP_FOLDER;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)floder;
    return sendCommand(4, NULL, 0);
}

/**************************************************************** 
 * Function Name: setEqualizer
 * Description: set the palyer equalizer
 * Parameters: NORMAL = 0x00, POP = 0x01, ROCK = 0x02,
			   JAZZ = 0x03, CLASSIC = 0x04, BASS = 0x05
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::setEqualizer(EQUALIZER Equalizer)
{
    commandBytes[0] = KT403A_SET_EQUALIZER;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)Equalizer;
    return sendCommand(4, NULL, 0);
}

/**************************************************************** 
 * Function Name: repeat
 * Description: repeat the current music
 * Parameters: 0:repeat 1:no repeat
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::repeat(uint8_t state)
{
    commandBytes[0] = KT403A_REPEAT_SINGLE;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)state;
    return sendCommand(4, NULL, 0);
}

/**************************************************************** 
 * Function Name: suffleFolder
 * Description: suffle all music in the specified folder
 * Parameters: none
 * Return: 0:success -1:fail
****************************************************************/ 
template <class T>
uint8_t KT403A<T>::suffleFolder(uint8_t floder)
{
    commandBytes[0] = KT403A_SET_SHUFFLE_FOLDER;
    commandBytes[1] = KT403A_FEEDBACK_CODE;
    commandBytes[2] = (uint8_t)0x00;
    commandBytes[3] = (uint8_t)floder;
    return sendCommand(4, NULL, 0);
}

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
template class KT403A<Uart>;
#endif
template class KT403A<HardwareSerial>;

#ifdef __AVR__
#include <SoftwareSerial.h>
template class KT403A<SoftwareSerial>;
#endif
