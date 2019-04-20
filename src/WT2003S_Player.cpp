/**
 * The MIT License (MIT)
 * 
 * Author: Baozhu Zuo (baozhu.zuo@gmail.com)
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
#include "WT2003S_Player.h"
WT2003S::WT2003S()
{
    _busyPin = -1;
}

void WT2003S::init(COMserial &serialPort)
{
    _serial = &serialPort;
}
void WT2003S::init(COMserial &serialPort, uint8_t pin)
{
    _serial = &serialPort;
    _busyPin = pin;
}

uint8_t WT2003S::sendCommand(uint8_t commandLength)
{
    //Clear anything in the buffer
    while(_serial->available() > 0) {
        _serial->read();
    }
    _serial->write(WT2003S_START_CODE);
    _serial->write(commandLength + 2); //Add one byte for 'length', one for CRC

    //Begin sending command bytes while calc'ing CRC
    byte crc = commandLength + 2;
    for (byte x = 0; x < commandLength; x++) //Length + command code + parameter
    {
        _serial->write(commandBytes[x]); //Send this byte
        crc += commandBytes[x];          //Add this byte to the CRC
    }

    _serial->write(crc); //Send CRC
    _serial->write(WT2003S_END_CODE);
    return 0;
}

bool WT2003S::getResponse(uint8_t code, uint8_t *data, uint8_t len)
{
    uint8_t d = 0;
    while(1){
        if(_serial->available()){
            d = _serial->read();
            if(code == d)
                break;
        }
        delayMicroseconds(500);
    }
    if(len > 9) len = 9;
    for (int i = 1; i < len; i++){
        while (! _serial->available()){}
        data[i - 1] = _serial->read();
    }
    return true;
}

uint8_t WT2003S::playSPIFlashSong(uint16_t index)
{
    commandBytes[0] = WT2003S_SPIFLASH_PLAY_INDEX;
    commandBytes[1] = (index >> 8) & 0xff;
    commandBytes[2] = 0xff & index;
    return sendCommand(3);
}
uint8_t WT2003S::playSDRootSong(uint32_t index)
{
    commandBytes[0] = WT2003S_SD_PLAY_INDEX_IN_ROOT;
    commandBytes[1] = (index >> 8) & 0xff;
    commandBytes[2] = 0xff & index;
    return sendCommand(3);    
}
uint8_t WT2003S::playSDSong(const char *fileName)
{
    commandBytes[0] = WT2003S_SD_PLAY_FILE_IN_ROOT;
    commandBytes[1] = fileName[0];
    commandBytes[2] = fileName[1];
    commandBytes[3] = fileName[2];
    commandBytes[4] = fileName[3];
    return sendCommand(5);
}
uint8_t WT2003S::playSDDirectoruSong(const char* dir, uint16_t index)
{
    commandBytes[0] = WT2003S_SD_PLAY_INDEX_IN_FOLDER;
    commandBytes[1] = dir[0];
    commandBytes[2] = dir[1];
    commandBytes[3] = dir[2];
    commandBytes[4] = dir[3];
    commandBytes[5] = dir[4];
    commandBytes[6] = (index >> 8) & 0xff;
    commandBytes[7] = 0xff & index;
    return sendCommand(8);
}
uint8_t WT2003S::playUDiskRootSong(uint32_t index)
{
    commandBytes[0] = WT2003S_UDISK_PLAY_INDEX_IN_ROOT;
    commandBytes[1] = (index >> 8) & 0xff;
    commandBytes[2] = 0xff & index;
    return sendCommand(3);
}
uint8_t WT2003S::playUDiskSong(const char *fileName)
{
    commandBytes[0] = WT2003S_UDISK_PLAY_FILE_IN_ROOT;
    commandBytes[1] = fileName[0];
    commandBytes[2] = fileName[1];
    commandBytes[3] = fileName[2];
    commandBytes[4] = fileName[3];
    return sendCommand(5);
}
uint8_t WT2003S::playUDiskDirectoruSong(const char* dir, uint32_t index)
{
    commandBytes[0] = WT2003S_UDISK_PLAY_INDEX_IN_FOLDER;
    commandBytes[1] = dir[0];
    commandBytes[2] = dir[1];
    commandBytes[3] = dir[2];
    commandBytes[4] = dir[3];
    commandBytes[5] = dir[4];
    commandBytes[6] = (index >> 8) & 0xff;
    commandBytes[7] = 0xff & index;
    return sendCommand(8);
}

uint8_t WT2003S::pause_or_play()
{
    commandBytes[0] = WT2003S_PAUSE_OR_PLAY;
    return sendCommand(1);
}
uint8_t WT2003S::stop()
{
    commandBytes[0] = WT2003S_STOP;
    return sendCommand(1);
}
uint8_t WT2003S::next()
{
    commandBytes[0] = WT2003S_NEXT;
    return sendCommand(1);
}
uint8_t WT2003S::previous()
{
    commandBytes[0] = WT2003S_PREVIOUS;
    return sendCommand(1);
}
uint8_t WT2003S::volume(uint8_t vol)
{
    if (vol > WT2003S_MAX_VOLUME)
        vol = WT2003S_MAX_VOLUME;
    commandBytes[0] = WT2003S_SET_VOLUME;
    commandBytes[1] = vol;
    return sendCommand(2);
}
uint8_t WT2003S::playMode(PLAY_MODE mode)
{
    commandBytes[0] = WT2003S_SET_PLAYMODE;
    commandBytes[1] = mode;
    return sendCommand(2);
}
//Note if current playing in spi flash ,this API cannt be used
uint8_t WT2003S::cutInPlay(STROAGE device, uint32_t index)
{   
    commandBytes[0] = WT2003S_SET_CUTIN_MODE;
    commandBytes[1] = device;
    commandBytes[2] = index;
    return sendCommand(3);
}
//Copy SD to SPI-FLASH
// return: 
// 0x00, mp3 file copy ok, config data copy ok
// 0x01, mp3 file copy ok, config  data error
// 0x02, mp3 file copy error. config data ok , maybe mp3 file too big to copy
// 0x03, mp3 file cope ok, config data error, maybe mp3 file too big to copy
uint8_t WT2003S::copySDtoSPIFlash()
{
    commandBytes[0] = WT2003S_COPY_SDTOSPIFLASH;
    return sendCommand(1);
}
//Copy UDISK to SPI-FLASH
// return: 
// 0x00, mp3 file copy ok, config data copy ok
// 0x01, mp3 file copy ok, config  data error
// 0x02, mp3 file copy error. config data ok , maybe mp3 file too big to copy
// 0x03, mp3 file cope ok, config data error, maybe mp3 file too big to copy
uint8_t WT2003S::copyUDisktoSPIFlash()
{
    commandBytes[0] = WT2003S_COPY_UDISKTOSPIFLASH;
    return sendCommand(1);
}
uint8_t WT2003S::writeUserData(uint16_t address, uint32_t data)
{
    commandBytes[0] = WT2003S_STORY_USERDATA;
    commandBytes[1] = (address >> 8) & 0xff;
    commandBytes[2] = 0xff & address;
    commandBytes[3] = (data >> 24) & 0xff;
    commandBytes[4] = (data >> 16) & 0xff;
    commandBytes[5] = (data >> 8) & 0xff;
    commandBytes[6] = 0xff & data;
    return sendCommand(7);
}
uint8_t WT2003S::switchWorkDisk(STROAGE disk)
{
    commandBytes[0] = WT2003S_SWITCH_WORKDATA;
    commandBytes[1] = disk;
    return sendCommand(2);
}

int8_t WT2003S::getVolume()
{
    uint8_t vol;
    commandBytes[0] = WT2003S_GET_VOLUME;
    sendCommand(1);
    getResponse(WT2003S_GET_VOLUME, &vol, 2);
    return vol;
}
int8_t WT2003S::getStatus()
{
    uint8_t status;
    commandBytes[0] = WT2003S_GET_STATE;
    sendCommand(1);
    getResponse(WT2003S_GET_STATE, &status, 2);
    return status;
}

uint32_t WT2003S::getSPIFlashMp3FileNumber()
{
    uint8_t num;
    commandBytes[0] = WT2003S_GET_SPIFLASH_SONGCOUNT;
    sendCommand(1);
    getResponse(WT2003S_GET_SPIFLASH_SONGCOUNT, &num, 2);
    return num;
}
uint32_t WT2003S::getSDMp3FileNumber()
{
    union {
        uint8_t d[2];
        uint16_t n;
    }num;
    uint8_t tmp;
    commandBytes[0] = WT2003S_GET_SD_SONGCOUNT;
    sendCommand(1);
    getResponse(WT2003S_GET_SD_SONGCOUNT, num.d, 3);
    tmp = num.d[0];
    num.d[0] = num.d[1];
    num.d[1] = tmp;
    return num.n;
}
uint32_t WT2003S::getSDDirectoryMp3FileNumber(const char *dir)
{    
    uint8_t num;
    commandBytes[0] = WT2003S_GET_SD_SONGS_IN_FOLDER_COUNT;
    commandBytes[1] = dir[0];
    commandBytes[2] = dir[1];
    commandBytes[3] = dir[2];
    commandBytes[4] = dir[3];
    commandBytes[5] = dir[4];
    sendCommand(6);
    getResponse(WT2003S_GET_SD_SONGS_IN_FOLDER_COUNT, &num, 2);
    return num;
}
uint32_t WT2003S::getUDiskMp3FileNumber()
{
    uint8_t num;
    commandBytes[0] = WT2003S_GET_UDISK_SONGCOUNT;
    sendCommand(1);
    getResponse(WT2003S_GET_UDISK_SONGCOUNT, &num, 2);
    return num;
}
uint32_t WT2003S::getUDiskDirectoryMp3FileNumber(const char *dir)
{
    uint8_t num;
    commandBytes[0] = WT2003S_GET_UDISK_SONGS_IN_FOLDER_COUNT;
    commandBytes[1] = dir[0];
    commandBytes[2] = dir[1];
    commandBytes[3] = dir[2];
    commandBytes[4] = dir[3];
    commandBytes[5] = dir[4];
    sendCommand(6);
    getResponse(WT2003S_GET_UDISK_SONGS_IN_FOLDER_COUNT, &num, 2);
    return num;
}
uint32_t WT2003S::getTracks()
{
    union {
        uint8_t d[2];
        uint16_t n;
    }data;
    uint8_t tmp;
    commandBytes[0] = WT2003S_GET_FILE_PLAYING;
    sendCommand(1);
    getResponse(WT2003S_GET_FILE_PLAYING, data.d, 3);
    tmp = data.d[0];
    data.d[0] = data.d[1];
    data.d[1] = tmp;
    return data.n;
}
void WT2003S::getSongName(char* Songname)
{
    commandBytes[0] = WT2003S_GET_SONG_NAME_PLAYING;
    sendCommand(1);
    getResponse(WT2003S_GET_SONG_NAME_PLAYING, Songname, 9);
}
uint8_t WT2003S::getDiskStatus()
{
    uint8_t disk;
    commandBytes[0] = WT2003S_DISKSTATUS;
    sendCommand(1);
    getResponse(WT2003S_DISKSTATUS, &disk, 2);
    return disk;
}
void WT2003S::getSPIFLashMp3Data(char *data,uint16_t address,uint16_t len)
{
    if(len > 512) len = 512;
    commandBytes[0] = WT2003S_GET_USERDATA;
    commandBytes[1] = 0x00;
    commandBytes[2] = (address >> 8) & 0xff;
    commandBytes[3] = 0xff & address;

    commandBytes[4] = (len >> 8) & 0xff;
    commandBytes[5] = 0xff & len;
    sendCommand(6);
    getResponse(WT2003S_GET_USERDATA, data, len + 4);
}