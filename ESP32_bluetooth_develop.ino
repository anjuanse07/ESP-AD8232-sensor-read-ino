#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <BluetoothSerial.h>

#define SCK  14
#define MISO  12
#define MOSI  13
#define CS  15

#define ECG_INPUT     36
#define ECG_LO_MIN    40
#define ECG_LO_PLUS   41

SPIClass spi = SPIClass(HSPI);
BluetoothSerial SerialBT;
int ecgRead;
char newFileNameBuffer[20];
String newFileName;
int lastIndex = -1;

int scanFileIndex(const String& fileName) {
  int fileIndex = -1;
  
  if (sscanf(fileName.c_str(), "data%d.csv", &fileIndex) == 1) {
    return fileIndex;
  }else {
    return -1;  // Invalid file name format
  }
}

void writeFile(fs::FS &fs, const char * path, int message){
  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.println(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void writeFileStr(fs::FS &fs, const char * path, String message){
  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.println(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

String generateNewFileName(){
    File root = SD.open("/");
    
    while (true) {
      File entry = root.openNextFile();
      if (!entry){
        break;
      }

      String fileName = entry.name();
      
      if (fileName.startsWith("data")) {

        int currentIndex = scanFileIndex(fileName);
  
        if (currentIndex > lastIndex) {
          lastIndex = currentIndex;
        }
      }
      entry.close();
    }
    
    root.close();

    int newIndex = lastIndex + 1;
    String newFileName = "data" + String(newIndex) + ".csv";
    return newFileName;
}

void setup(){

  Serial.begin(115200);
  pinMode(ECG_INPUT, INPUT);
  SerialBT.begin("ESP32test");
  
  
  spi.begin(SCK, MISO, MOSI, CS);
  
  if(!SD.begin(CS,spi,8000000)){
    Serial.println("Card Mount Failed");
    return;
  }
  newFileName = generateNewFileName();
  newFileName.toCharArray(newFileNameBuffer, sizeof(newFileNameBuffer));
  
  
  File newFile = SD.open(("/" + String(newFileNameBuffer)).c_str(), FILE_WRITE);
  if (newFile) {
    newFile.println("1000");
    newFile.close();
  } else {
    Serial.println("Failed to create new file!");
  }
  writeFileStr(SD, "/pasien_1.csv", "rekamanbaru");
}

void loop(){
  ecgRead = analogRead(ECG_INPUT);

  Serial.println(ecgRead);
  SerialBT.println(ecgRead);

  writeFile(SD, ("/" + String(newFileNameBuffer)).c_str(), ecgRead);
  writeFile(SD, "/pasien_1.csv", ecgRead);

  delay(1);
}
