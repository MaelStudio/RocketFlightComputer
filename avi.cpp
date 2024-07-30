#include "globals.h"

// avi header data
const uint8_t dcBuf[4] = {0x30, 0x30, 0x64, 0x63};   // 00dc
const uint8_t wbBuf[4] = {0x30, 0x31, 0x77, 0x62};   // 01wb
static const uint8_t idx1Buf[4] = {0x69, 0x64, 0x78, 0x31}; // idx1
static const uint8_t zeroBuf[4] = {0x00, 0x00, 0x00, 0x00}; // 0000
static uint8_t* idxBuf = NULL;

uint8_t aviHeader[AVI_HEADER_LEN] = { // AVI header template
  0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x41, 0x56, 0x49, 0x20, 0x4C, 0x49, 0x53, 0x54,
  0x16, 0x01, 0x00, 0x00, 0x68, 0x64, 0x72, 0x6C, 0x61, 0x76, 0x69, 0x68, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54, 0x6C, 0x00, 0x00, 0x00,
  0x73, 0x74, 0x72, 0x6C, 0x73, 0x74, 0x72, 0x68, 0x30, 0x00, 0x00, 0x00, 0x76, 0x69, 0x64, 0x73,
  0x4D, 0x4A, 0x50, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x74, 0x72, 0x66,
  0x28, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x18, 0x00, 0x4D, 0x4A, 0x50, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x4C, 0x49, 0x53, 0x54, 0x56, 0x00, 0x00, 0x00, 
  0x73, 0x74, 0x72, 0x6C, 0x73, 0x74, 0x72, 0x68, 0x30, 0x00, 0x00, 0x00, 0x61, 0x75, 0x64, 0x73,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x11, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x11, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x73, 0x74, 0x72, 0x66,
  0x12, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x11, 0x2B, 0x00, 0x00, 0x11, 0x2B, 0x00, 0x00,
  0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 
  0x4C, 0x49, 0x53, 0x54, 0x00, 0x00, 0x00, 0x00, 0x6D, 0x6F, 0x76, 0x69,
};

struct frameSizeStruct {
  uint8_t frameWidth[2];
  uint8_t frameHeight[2];
};
// indexed by frame type - needs to be consistent with sensor.h framesize_t enum
static const frameSizeStruct frameSizeData[] = {
  {{0x60, 0x00}, {0x60, 0x00}}, // 96X96
  {{0xA0, 0x00}, {0x78, 0x00}}, // qqvga 
  {{0xB0, 0x00}, {0x90, 0x00}}, // qcif 
  {{0xF0, 0x00}, {0xB0, 0x00}}, // hqvga 
  {{0xF0, 0x00}, {0xF0, 0x00}}, // 240X240
  {{0x40, 0x01}, {0xF0, 0x00}}, // qvga 
  {{0x90, 0x01}, {0x28, 0x01}}, // cif 
  {{0xE0, 0x01}, {0x40, 0x01}}, // hvga 
  {{0x80, 0x02}, {0xE0, 0x01}}, // vga 
  {{0x20, 0x03}, {0x58, 0x02}}, // svga 
  {{0x00, 0x04}, {0x00, 0x03}}, // xga 
  {{0x00, 0x05}, {0xD0, 0x02}}, // hd
  {{0x00, 0x05}, {0x00, 0x04}}, // sxga
  {{0x40, 0x06}, {0xB0, 0x04}}  // uxga 
};

#define IDX_ENTRY 16 // bytes per index entry

// separate index for motion capture and timelapse
static size_t idxPtr;
static size_t idxOffset;
static size_t moviSize;
static size_t audSize;
static size_t indexLen;
bool haveSoundFile = false;
const uint32_t SAMPLE_RATE = 16000; // sample rate used


void prepAviIndex() {
  // prep buffer to store index data, gets appended to end of file
  if (idxBuf == NULL) idxBuf = (uint8_t*)ps_malloc((MAX_FRAMES+1)*IDX_ENTRY); // include some space for audio index
  memcpy(idxBuf, idx1Buf, 4); // index header
  idxPtr = CHUNK_HDR;  // leave 4 bytes for index size
  moviSize = indexLen = 0;
}

void buildAviHdr(uint8_t FPS, uint8_t frameType, uint16_t frameCnt) {
  // update AVI header template with file specific details
  size_t aviSize = moviSize + AVI_HEADER_LEN + ((CHUNK_HDR+IDX_ENTRY) * (frameCnt+(haveSoundFile?1:0))); // AVI content size 
  // update aviHeader with relevant stats
  memcpy(aviHeader+4, &aviSize, 4);
  uint32_t usecs = (uint32_t)round(1000000.0f / FPS); // usecs_per_frame 
  memcpy(aviHeader+0x20, &usecs, 4); 
  memcpy(aviHeader+0x30, &frameCnt, 2);
  memcpy(aviHeader+0x8C, &frameCnt, 2);
  memcpy(aviHeader+0x84, &FPS, 1);
  uint32_t dataSize = moviSize + ((frameCnt+(haveSoundFile?1:0)) * CHUNK_HDR) + 4; 
  memcpy(aviHeader+0x12E, &dataSize, 4); // data size 
  uint8_t withAudio = 2; // increase number of streams for audio
  memcpy(aviHeader+0x100, &audSize, 4); // audio data size
  // apply video framesize to avi header
  memcpy(aviHeader+0x40, frameSizeData[frameType].frameWidth, 2);
  memcpy(aviHeader+0xA8, frameSizeData[frameType].frameWidth, 2);
  memcpy(aviHeader+0x44, frameSizeData[frameType].frameHeight, 2);
  memcpy(aviHeader+0xAC, frameSizeData[frameType].frameHeight, 2);
  // apply audio details to avi header
  memcpy(aviHeader+0xF8, &SAMPLE_RATE, 4);
  uint32_t bytesPerSec = SAMPLE_RATE * 2;
  memcpy(aviHeader+0x104, &bytesPerSec, 4); // suggested buffer size
  memcpy(aviHeader+0x11C, &SAMPLE_RATE, 4);
  memcpy(aviHeader+0x120, &bytesPerSec, 4); // bytes per sec

  // reset state for next recording
  moviSize = idxOffset = idxPtr = 0;
}

void buildAviIdx(size_t dataSize, bool isVid) {
  // build AVI video index into buffer - 16 bytes per frame
  // called from saveFrame() for each frame
  moviSize += dataSize;
  if (isVid) memcpy(idxBuf+idxPtr, dcBuf, 4);
  else memcpy(idxBuf+idxPtr, wbBuf, 4);
  memcpy(idxBuf+idxPtr+4, zeroBuf, 4);
  memcpy(idxBuf+idxPtr+8, &idxOffset, 4); 
  memcpy(idxBuf+idxPtr+12, &dataSize, 4); 
  idxOffset += dataSize + CHUNK_HDR;
  idxPtr += IDX_ENTRY; 
}

size_t writeAviIndex(byte* clientBuf, size_t buffSize) {
  // write completed index to avi file
  // called repeatedly from closeAvi() until return 0
  if (idxPtr < indexLen) {
    if (indexLen-idxPtr > buffSize) {
      memcpy(clientBuf, idxBuf+idxPtr, buffSize);
      idxPtr += buffSize;
      return buffSize;
    } else {
      // final part of index
      size_t final = indexLen-idxPtr;
      memcpy(clientBuf, idxBuf+idxPtr, final);
      idxPtr = indexLen;
      return final;    
    }
  }
  return idxPtr = 0;
}
  
void finalizeAviIndex(uint16_t frameCnt) {
  // update index with size
  uint32_t sizeOfIndex = (frameCnt+(haveSoundFile?1:0))*IDX_ENTRY;
  memcpy(idxBuf+4, &sizeOfIndex, 4); // size of index 
  indexLen = sizeOfIndex + CHUNK_HDR;
  idxPtr = 0; // pointer to index buffer
}