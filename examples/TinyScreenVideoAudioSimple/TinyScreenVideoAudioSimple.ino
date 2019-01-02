/**********************************************************************
 * This is the example sketch used to display .TSV files on a
 * TinyScreen+. The audio handling included will only run on the
 * TinyArcade.
 * 
 * Written by Ben Rose for TinyCircuits. http://tinycircuits.com
 * Last updated 08 June 2017
 **********************************************************************/

#include <TinyScreen.h>
#include <SPI.h>
#include "SdFat.h"

TinyScreen display = TinyScreen(TinyScreenPlus);

SdFat sd;
SdFile dataFile;
uint8_t buffer[96 * 64 * 2];
uint16_t audioBuffer[1024];

volatile uint32_t __SampleIndex = 0;
uint32_t __NumberOfSamples = 1024;


void setup(void) {
  display.begin();
  display.setBitDepth(1);
  display.setFlip(1);
  display.initDMA();
  SPI.begin();
  if (!sd.begin(10, SPI_FULL_SPEED)) {
    SerialUSB.println(F("Card failed"));
    while (1);
  }
  SPI.setClockDivider(0);

  analogWrite(A0, 0);
  tcConfigure(30720);
}

void loop() {
  sd.vwd()->rewind();
  dataFile.close();
  while (dataFile.openNext(sd.vwd(), O_READ)) {
    if (!dataFile.isDir()) {
      char path[100];
      memset(path, 0, 100);
      int nameLength = 0;
      dataFile.getName(path, 50);
      nameLength = strlen(path);
      dataFile.close();
      if (!strcmp(path + (nameLength - 3), "tsv") || !strcmp(path + (nameLength - 3), "TSV")) {
        playVideo(path, 30);
      }
    }
    dataFile.close();
  }
}


void playVideo(char * filename, int framerate) {
  if (!dataFile.open(filename, O_READ)) {
    //SerialUSB.println(F("Error opening file!"));
    delay(100);
    return;
  }
  display.goTo(0, 0);
  display.startData();
  unsigned long timer = millis();
  unsigned char t = 0;
  unsigned char startFlag = 1;
  dataFile.read(buffer, 96 * 64 * 2);
  dataFile.read((uint8_t*)audioBuffer, 1024 * 2);
  while (dataFile.available()) {
    if (startFlag) {
      tcStartCounter();
      startFlag = 0;
    }
    dataFile.read(buffer, 96 * 64 * 2);
    while (__SampleIndex < 1000);
    dataFile.read((uint8_t*)audioBuffer, 1024 * 2);
    display.writeBufferDMA(buffer, 96 * 64 * 2);
  }
  while(!display.getReadyStatusDMA());
  display.endTransfer();
  dataFile.close();
}

