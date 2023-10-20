/**
    The MIT License (MIT)

    Author: MengDu

    Copyright (C) 2023  Seeed Technology Co.,Ltd.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#ifndef __WT2605C__
#define __WT2605C__

#include "Arduino.h"

#define WT2605C_NUM_CMD_BYTES 11
#define WT2605C_NUM_NAME_BYTES 9

//These are the commands that are sent over serial to the WT2605C
#define WT2605C_SPIFLASH_PLAY_INDEX 0xA0
#define WT2605C_SD_PLAY_INDEX_IN_ROOT 0xA2
#define WT2605C_SD_PLAY_FILE_IN_ROOT 0xA3
#define WT2605C_SD_PLAY_INDEX_IN_FOLDER 0xA4
#define WT2605C_UDISK_PLAY_INDEX_IN_ROOT 0xA6
#define WT2605C_UDISK_PLAY_FILE_IN_ROOT 0xA7
#define WT2605C_UDISK_PLAY_INDEX_IN_FOLDER 0xA8
#define WT2605C_PAUSE_OR_PLAY 0xAA
#define WT2605C_STOP 0xAB
#define WT2605C_NEXT 0xAC
#define WT2605C_PREVIOUS 0xAD
#define WT2605C_SET_VOLUME 0xAE //Can take more than 150ms to complete
#define WT2605C_SET_PLAYMODE 0xAF
#define WT2605C_SET_PLAYMODE_SINGLE_NO_LOOP 0x00
#define WT2605C_SET_PLAYMODE_SINGLE_LOOP 0x01
#define WT2605C_SET_PLAYMODE_ALL_LOOP 0x02
#define WT2605C_SET_PLAYMODE_RANDOM 0x03
#define WT2605C_SET_CUTIN_MODE 0xB1
#define WT2605C_COPY_SDTOSPIFLASH 0xB3
#define WT2605C_COPY_UDISKTOSPIFLASH 0xB4
#define WT2605C_STORY_USERDATA 0xB8
#define WT2605C_ISNEED_RETURNCODE 0xBA
#define WT2605C_SWITCH_WORKDATA 0xD2

#define WT2605C_GET_VOLUME 0xC1
#define WT2605C_GET_STATE 0xC2
#define WT2605C_GET_SPIFLASH_SONGCOUNT 0xC3
#define WT2605C_GET_SD_SONGCOUNT 0xC5
#define WT2605C_GET_SD_SONGS_IN_FOLDER_COUNT 0xC6
#define WT2605C_GET_UDISK_SONGCOUNT 0xC7
#define WT2605C_GET_UDISK_SONGS_IN_FOLDER_COUNT 0xC8


#define WT2605C_GET_FILE_PLAYING 0xC9
#define WT2605C_DISKSTATUS 0xCA
#define WT2605C_GET_SONG_NAME_PLAYING 0xCB
#define WT2605C_GET_USERDATA 0xCF

#define WT2605C_START_CODE "AT+"
#define WT2605C_END_CODE "\r"
#define WT2605C_NUM_CMD_BYTES 6

#define WT2605C_MAX_VOLUME 0x1F
#define WT2605C_MIN_VOLUME 0x00
#define WT2605C_TIMEOUT 1000

#define WT2605C_SEND_ATTEMPTS 5

typedef enum {
    SINGLE_SHOT    = 0x00,
    SINGLE_CYCLE   = 0x01,
    CYCLE    = 0x02,
    RANDOM   = 0x03,
} PLAY_MODE;

typedef enum {
    SPIFLASH    = 0x00,
    SD   = 0x01,
    UDISK    = 0x02,
} STROAGE;


template <class T>
class WT2605C {
private:
    T* _serial;
    uint8_t _busyPin;
    void send(uint8_t commandLength);
    uint8_t sendCommand(uint8_t commandLength);
    uint8_t sendCommand(uint8_t commandLength, String data, uint8_t len);
    String commandBytes[WT2605C_NUM_CMD_BYTES];

public:
    WT2605C();
    void init(T& serialPort);
    void init(T& serialPort, uint8_t pin);

    uint8_t playSDRootSong(uint16_t parameters);
    // uint8_t playSDSong(const char* fileName);
    // uint8_t playSDDirectorySong(const char* dir, uint16_t index);
    // uint8_t playUDiskRootSong(uint32_t index);
    // uint8_t playUDiskSong(const char* fileName);
    // uint8_t playUDiskDirectorySong(const char* dir, uint32_t index);
    // uint8_t pause_or_play();
    // uint8_t stop();
    // uint8_t next();
    // uint8_t previous();
    // uint8_t volume(uint8_t vol);
    // uint8_t volumeDown();
    // uint8_t volumeUp();
    // uint8_t playMode(PLAY_MODE mode);
    // uint8_t cutInPlay(STROAGE device, uint32_t index);
    // uint8_t copySDtoSPIFlash();
    // uint8_t copyUDisktoSPIFlash();
    // uint8_t writeUserData(uint16_t address,  uint32_t data);
    // uint8_t switchWorkDisk(STROAGE disk);

    // int8_t getVolume();
    // int8_t getStatus();
    // uint32_t getSPIFlashMp3FileNumber();
    // uint32_t getSDMp3FileNumber();
    // uint32_t getSDDirectoryMp3FileNumber(const char* dir);
    // uint32_t getUDiskMp3FileNumber();
    // uint32_t getUDiskDirectoryMp3FileNumber(const char* dir);
    // uint32_t getTracks();
    // void getSongName(char* Songname);
    // uint8_t getDiskStatus();
    // void getSPIFLashMp3Data(char* data, uint16_t address, uint16_t len);
};

#endif

