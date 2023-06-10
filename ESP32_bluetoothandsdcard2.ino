#include <BluetoothSerial.h>
#include <SD.h>

#define ECG_INPUT     36
#define ECG_LO_MIN    40
#define ECG_LO_PLUS   41

BluetoothSerial SerialBT;
const int chipSelect = 15;
const int customSCK = 14;   // Custom SPI SCK pin
const int customMOSI = 12;  // Custom SPI MOSI pin
const int customMISO = 13;  // Custom SPI MISO pin
int fileCount = 0;
//SPIClass spi;

void saveDataToFile(int data) {
  File dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
    Serial.println("Data saved to file: " + String(data));
  } else {
    Serial.println("Failed to open file for writing.");
  }
}

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, int message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  file.close();
}

void createNewFile() {
  fileCount++;
  String fileName = "data" + String(fileCount) + ".txt";
  File newFile = SD.open(fileName.c_str(), FILE_WRITE);
  if (newFile) {
    newFile.println("Data:");
    newFile.close();
    Serial.println("New file created: " + fileName);
  } else {
    Serial.println("Failed to create file: " + fileName);
  }
}

void setup() {
  pinMode(ECG_INPUT, INPUT);
  SerialBT.begin("ESP32test-s", true);
  Serial.begin(115200);
//  spi.begin(customSCK, customMISO, customMOSI, chipSelect);
  SPI.begin();
  SD.begin(chipSelect,SPI);
  
  writeFile(SD,"data.txt","1000");
  
  createNewFile();
}

void loop() {
  int ecgRead = analogRead(ECG_INPUT);

  Serial.println(ecgRead);
  SerialBT.println(ecgRead);
  
//  writeFile(SD,"/data.txt","1000");
  appendFile(SD,"data.txt",ecgRead);

  saveDataToFile(ecgRead);

  delay(1);
}
