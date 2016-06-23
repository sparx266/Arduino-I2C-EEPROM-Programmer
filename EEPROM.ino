#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

File root;


const int chipSelect = 10;  // Change this if you use a different pin for CS
char* file_list[20];        // Maximun number of .BIN files is 20
int listCounter=0;
int x = 0;

char* chips[]={"24LC256","24LC512"}; 
int chip_pointer =0;

int files_found = 0;
int file_pointer=0;
int xPin = A1;
int yPin = A0;
int buttonPin = A2;

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;
byte a;
unsigned int end_address = 0;
unsigned int counter = 0;
byte data;



void setup() {
lcd.begin(16,2);   
lcd.backlight();

lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("     EEPROM ");
  lcd.setCursor(0,1);
  delay(500);
  lcd.print("   Programmer");
  delay(1000);
  lcd.clear();
  lcd.home();
  lcd.setCursor(0,0);
  lcd.print("by Sparx266 2016");
  lcd.setCursor(0,1);
  lcd.print(" Made in Greece ");
delay(1000);

lcd.clear();

Wire.beginTransmission(0x50);  //Adress of EEPROM
if (Wire.endTransmission()==0)
{
  lcd.print("Chip detected");

}
else {
lcd.print("No Chip detected");
}


delay(1000);

  // initialize serial communications at 115200 bps:
 // Serial.begin(115200); 
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  //activate pull-up resistor on the push-button pin
  pinMode(buttonPin, INPUT_PULLUP); 
  
   pinMode(SS, OUTPUT);
   
     if (!SD.begin(chipSelect)) {
    lcd.clear();
     lcd.print("No Card found?");

    return;
  }

     lcd.clear();
       lcd.print("Card detected");
   delay(1000);
delay(1000);

  
  root = SD.open("/");
  
  printDirectory(root, 0);
  
  lcd.setCursor(0,1);
//Display number of files
lcd.clear();
     lcd.home();
     lcd.print(files_found, DEC);
     lcd.print(" files found");
    delay(1000); 
//End of Display number of files

  
// Select EEPROM to use
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Select EEPROM");


while ((digitalRead(buttonPin)) == HIGH)
{ // Start of while loop
lcd.setCursor(0,1);
lcd.print(chips[chip_pointer]); 
if (analogRead(xPin)<5)
{
  chip_pointer--;
if (chip_pointer < 0)
   {
       (chip_pointer=0);
       
    }

lcd.setCursor(0,1);
lcd.print("                "); 
  
}
else 

if (analogRead(xPin)>1020)
{
  chip_pointer++;
   if (chip_pointer >1)
   {
       (chip_pointer=1);
     }
lcd.setCursor(0,1);
lcd.print("                "); 
} // End of if loop
delay (100);
} // End of while loop  

switch (chip_pointer) {
     case 0:
       end_address=32768; 
       break;
     case 1:
       end_address=65535;
       break;

   }

delay(200);

// End of EEPROM select

//Select file to write to EEPROM 
lcd.clear();
lcd.setCursor(0,0);
delay(500);
lcd.print("Select file");

while ((digitalRead(buttonPin)) == HIGH)
{ // Start of while loop
lcd.setCursor(0,1);
lcd.print(file_list[file_pointer]); 
if (analogRead(xPin)<5)
{
  file_pointer--;
if (file_pointer < 0)
   {
       (file_pointer=0);
       
    }

lcd.setCursor(0,1);
lcd.print("                "); 
  
}
else 

if (analogRead(xPin)>1020)
{
  file_pointer++;
   if ((file_pointer >=(files_found-1)))
   {
       (file_pointer=(files_found-1));
     }
lcd.setCursor(0,1);
lcd.print("                "); 
} // End of if loop
delay (100);
} // End of while loop

lcd.clear();
lcd.setCursor(0,0);

// open file
  File dataFile = SD.open(file_list[file_pointer]);
  // if the file is available, read it:
  if (dataFile) {

    
    for (counter = 0; counter < end_address; counter++)
{
  lcd.setCursor(0,0);
  dataFile.seek(counter);
  a=dataFile.peek();
  lcd.print("Writing Data");
  
  lcd.setCursor(0,1);
    
    lcd.print(counter,HEX); 
    lcd.print(" ");
    lcd.print(a,HEX);
  
  
  // Actually write Data to Chip
  Wire.beginTransmission(0x50);
  Wire.write((int)highByte(counter));
  Wire.write((int)lowByte(counter));
  Wire.write(a);
  Wire.endTransmission();
  delay(5);

}
// Verify Routine
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Verify");
    for (counter = 0; counter < end_address; counter++)
{

  dataFile.seek(counter);
  a=dataFile.peek();
  // Read Data from Chip
  Wire.beginTransmission(0x50);
  Wire.write((int)highByte(counter));
  Wire.write((int)lowByte(counter));
  Wire.endTransmission();
  Wire.requestFrom((0x50),1);
  if (Wire.available()) data = Wire.read();
  lcd.setCursor(0,1);
  lcd.print(counter,HEX);
  lcd.print("  ");
  lcd.print(a,HEX);
  lcd.print("  ");
  lcd.print(data,HEX);

  if (a!=data)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed at: ");
    lcd.print(counter, DEC);
    dataFile.close();
    break;
  }
}


    
    
   dataFile.close();
   lcd.setCursor(0,1);
   lcd.print("Verify Pass    ");
   
   
  }  
// if the file isn't open, pop up an error:
  else {
    lcd.setCursor(0,0);
    lcd.print("error opening ");
    lcd.setCursor(0,1);
    lcd.print(file_list[file_pointer]);

delay(1000); 
 delay(1000); 
  }
  
}

void loop() {


}


void printDirectory(File dir, int numTabs) {
  // Begin at the start of the directory
  dir.rewindDirectory();
  
  while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {

     }
     // Print the 8.3 name
    if (isFnBin(entry.name()))
    {
          
Serial.println(entry.name());
     lcd.clear();
     lcd.print(entry.name());
     lcd.home();
    delay(1000); 

  

char* temp = entry.name();
     String toString = String(temp); //converted filename to String

     toString.toCharArray(temp, toString.length()+1); //converted it back to char array for it to be stored in char* file_list
     file_list[x]=strdup(temp);

     listCounter++;
     x++;
     


  }
     // Recurse for directories, otherwise print the file size
     if (entry.isDirectory()) {
     } else {
     }
     entry.close();
   }



}


bool isFnBin(char* filename) {
 int8_t len = strlen(filename);
bool result;
if (strstr(strlwr(filename+(len-4)),".bin")||strstr(strlwr(filename+(len-4)),".BIN")
){
  result = true;
  files_found++;
}else{
  result = false;
}
return result, files_found;
}
