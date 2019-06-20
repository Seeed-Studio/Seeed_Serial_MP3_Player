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
#ifndef __KT403A__
#define __KT403A__
 
#include "Arduino.h"

#define KT403A_NUM_CMD_BYTES 8
#define KT403A_TIMEOUT 1000
#define  KT403A_MAX_VOLUME 0x1E
//start or stop a cmd 

#define KT403A_CMD_START_CODE 0x7E
#define KT403A_CMD_END_CODE  0xEF
#define KT403A_VERSION_CODE  0xFF
#define KT403A_FEEDBACK_CODE  0x01 // ON: 0x01,OFF: 0x00
//These are the commands that are sent over serial to the KT403A

#define KT403A_PLAY_NEXT_SONG 0x01
#define KT403A_PLAY_PRRVIOUS_SONG 0x02
#define KT403A_PLAY_SPECIFIC_TRACK 0x03
#define KT403A_SET_VOLUME_UP 0x04
#define KT403A_SET_VOLUME_DOWN 0x05
#define KT403A_SET_VLOUME_VALUE 0x06
#define KT403A_SET_EQUALIZER 0x07
#define KT403A_REPEAT_CURRENT_TRACK 0x08
#define KT403A_SET_DEVICE 0x09
#define KT403A_ENTER_STANBY_MODE 0xA
#define KT403A_RESET 0x0C
#define KT403A_PLAY 0x0D
#define KT403A_PAUSE 0x0E
#define KT403A_PLAY_SONG_SPECIFY 0x0F
#define KT403A_SET_ALL_LOOP 0x11
#define KT403A_PLAY_SONG_MP3  0x12
#define KT403A_PLAY_SONG_ADVERT  0x13
#define KT403A_SET_SONG_NAME 0x14
#define KT403A_ADD_LOOP_TRACK 0x15
#define KT403A_STOP 0x16
#define KT403A_SET_LOOP_FOLDER 0x17
#define KT403A_REPEAT_SINGLE 0x19
#define KT403A_SET_SHUFFLE_FOLDER 0x28

//There are the parameter for some cmds

//EQUALIZER
typedef enum
{
    NORMAL = 0x00,
    POP = 0x01,
    ROCK = 0x02,
    JAZZ = 0x03,
    CLASSIC = 0x04,
    BASS = 0x05
} EQUALIZER;

//DEVICE 

#define U_DISK 0x01
#define SDCARD  0x02


//LOOP
#define STOP_LOOP 0x00
#define START_LOOP 0x01

template <class T>
class KT403A
{
  private:
	T *_serial;
	uint8_t _busyPin;
	uint8_t _status;
	uint8_t sendCommand(uint8_t commandLength, uint8_t *data, uint8_t len);
	uint8_t commandBytes[KT403A_NUM_CMD_BYTES];

  public:
  KT403A();
  void init(T &serialPort, uint8_t device = SDCARD);
  void init(T &serialPort, uint8_t pin, uint8_t device = SDCARD);
  uint8_t setDevice(uint8_t device);
  uint8_t playSongSpecify(uint8_t floder, uint8_t index);
  uint8_t playSongIndex(uint16_t index);
  uint8_t playSongMP3(uint16_t index);
  uint8_t playSongADVERT(uint16_t index);
  uint8_t volume(uint8_t volume);
  uint8_t volumeDown();
  uint8_t volumeUp();
  uint8_t setEqualizer(EQUALIZER Equalizer);
  uint8_t stop();
  uint8_t play();
  uint8_t pause();
  uint8_t next();
  uint8_t previous();
  uint8_t pause_or_play();
  uint8_t loop(uint8_t state);
  uint8_t loopFolder(uint8_t floder);
  uint8_t repeat(uint8_t state);
  uint8_t suffleFolder(uint8_t floder);
};

#endif
