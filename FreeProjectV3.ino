#include <NintendoExtensionCtrl.h>
#include <Mouse.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3D //initialize with the I2C addr 0x3D
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*
  -Pour MAP: après calib utilisation des val (0 ou 23 et etc..)
  -Voir pour drift (sys cal def ?)
*/

int cooX;
int cooY;
boolean buttonZ;
boolean buttonC;

boolean buttonMode;
boolean buttonScroll;

long variateur = 0;
int mode = 0;
const int nbrMode = 4;
boolean etat = 0;

unsigned int zoneMup = 0;  //Initialisation avant récup données eeProm
unsigned int zoneMdown = 0;
unsigned int zoneMright = 0;
unsigned int zoneMleft = 0;

unsigned int MinX = 0;    //Initialisation avant récup données Zones Max Joy
unsigned int MaxX = 0;
unsigned int MinY = 0;
unsigned int MaxY = 0;

/*Setup zone morte:
int zoneMup = 131;
int zoneMdown = 128;
int zoneMright = 125;
int zoneMleft = 127;

PIN variateur:         A0
PIN bouton Mode:       D1
PIN bouton scroll:     D2
*/

Nunchuk nchuk;

void setup(){

  Serial.begin(9600);
  Wire.begin();
  delay(50);

  mode = readEEPROM(0x50, 0);
  zoneMup = readEEPROM(0x50, 1);
  writeEEPROM(0x50, 1, 131);
  zoneMdown =readEEPROM(0x50, 2);
  writeEEPROM(0x50, 2, 128);
  zoneMright = readEEPROM(0x50, 3);
  writeEEPROM(0x50, 3, 124);
  zoneMleft = readEEPROM(0x50, 4);
  writeEEPROM(0x50, 4, 127);
  MinX = readEEPROM(0x50, 5);
  MaxX = readEEPROM(0x50, 6);
  MinY = readEEPROM(0x50, 7);
  MaxY = readEEPROM(0x50, 8);

  //mode = -1;  //MODE CALIBRATION ACTIF !!!!!!!

  Mouse.begin();
  nchuk.begin();

  display.begin(i2c_Address, true);
  display.clearDisplay();
  affichageMode(mode);
  delay(100);

  pinMode(A0, INPUT);       //Variateur Vitesse
  pinMode(D1, INPUT_PULLUP); //Bouton Mode
  pinMode(D2, INPUT_PULLUP); //Bouton Scroll
  pinMode(D3, INPUT); // PinMapButton

  while(!nchuk.connect()){
    Serial.println("! Nunchuk non detecte !");
    delay(1000);
  }
}

void loop(){

  //etat = nchuk.connect();
  etat = nchuk.update();
  Serial.print("Etat: ");
  Serial.println(etat);
  buttonMode = digitalRead(D1);
  buttonScroll = digitalRead(D2);

  if(!etat){
    Serial.println("! Nunchuk deconnecte !");
    digitalWrite(9, 0); digitalWrite(10, 0); digitalWrite(11, 0);
    delay(1000);
    nchuk.connect();
  }else{
    if(!buttonMode){
      mode++;
      if(mode==0){mode++;}
      if(mode > nbrMode){
        mode = 1;
        //EEPROM.write(0, mode);
        writeEEPROM(0x50, 0, mode);
        Serial.print("Nouveau mode: "); Serial.println(mode);
        affichageMode(mode);
      }else{
        //EEPROM.write(0, mode);
        writeEEPROM(0x50, 0, mode);
        Serial.print("Nouveau mode: "); Serial.println(mode);
        affichageMode(mode);
      }
      delay(240);
    }

    if(!buttonScroll && !buttonMode){
      mode = -1;
      delay(250);
    }

    switch(mode){
      case 1:
        Serial.println("Mode 1");
        modeSimple();
      break;

      case 2:
        Serial.println("Mode 2");
        modeIntermediaire();
      break;

      case 3:
        Serial.println("Mode 3");
        modeComplet();
      break;

      case 4:
        Serial.println("Mode 4");
        modeScroll();
      break;

      case -1:
        Serial.println("Mode calibration");
        calibrate();
      break;

      default:
        Serial.print("ERREUR: Aucun mode("); Serial.print(mode); Serial.println(")");
        delay(1000);
      break;
    }
  }

  delay(10);
}

void modeScroll(){
  buttonZ = nchuk.buttonZ();
  buttonC = nchuk.buttonC();
  cooY = nchuk.joyY();
  cooX = nchuk.joyX();
/*-------------------------------------------------
    Serial.print("cooY : "); Serial.println(cooY);
      
    Serial.print("cooX : "); Serial.println(cooX);

    delay(100);
  */


  if(!buttonScroll && cooY < zoneMdown || !buttonScroll && cooY > zoneMup){
    if(!buttonScroll && cooY < zoneMdown) {
      Serial.println("Scroll Down");
      Mouse.move(0, 0, -1);
      delay(300);
    }
    if(!buttonScroll && cooY > zoneMup) {
      Serial.println("Scroll Up");
      Mouse.move(0, 0, 1);
      delay(300);
    }
  }else{
    if(buttonC) {
    Serial.println("C");
    Mouse.press(MOUSE_RIGHT);
    }else{
      Mouse.release(MOUSE_RIGHT);
    }

    if(buttonZ) {
      Serial.println("Z");
      Mouse.press(MOUSE_LEFT);
    }else{
      Mouse.release(MOUSE_LEFT);
    }

    variateur = analogRead(A0);
    variateur = map(variateur, 0, 1023, 1, 12);

    if(cooX > zoneMleft || cooX < zoneMright){
      Serial.println(cooX);
      cooX = map(cooX, MinX, MaxX, -variateur, variateur);
      Mouse.move(cooX, 0, 0);
      Serial.print("X: "); Serial.print(cooX);
    }
    if(cooY < zoneMdown || cooY > zoneMup){
      cooY = map(cooY, MinY, MaxY, variateur, -variateur);
      Mouse.move(0, cooY, 0);
      Serial.print(" - Y: "); Serial.print(cooY);
    }
  }
  
  Serial.println("");
}

void modeComplet(){
  buttonZ = nchuk.buttonZ();
  buttonC = nchuk.buttonC();

  if(buttonZ) {
    Serial.println("Z");
    Mouse.press(MOUSE_RIGHT);
  }else{
    Mouse.release(MOUSE_RIGHT);
  }

  if(buttonC) {
    Serial.println("C");
    Mouse.press(MOUSE_LEFT);
  }else{
    Mouse.release(MOUSE_LEFT);
  }

  cooY = nchuk.joyY();
  cooX = nchuk.joyX();

  variateur = analogRead(A0);
  variateur = map(variateur, 0, 1023, 1, 12);
  Serial.print("Variateur: ");Serial.println(variateur);

  if(cooX > zoneMleft || cooX < zoneMright){
    cooX = map(cooX, MinX, MaxX, -variateur, variateur);
    Mouse.move(cooX, 0, 0);
    Serial.print("X: "); Serial.print(cooX);

  }
  if(cooY < zoneMdown || cooY > zoneMup){
    cooY = map(cooY, MinY, MaxY, variateur, -variateur);
    Mouse.move(0, cooY, 0);
    Serial.print(" - Y: "); Serial.print(cooY);
  }
  Serial.println("");
}

void modeIntermediaire(){
  buttonZ = nchuk.buttonZ();
  buttonC = nchuk.buttonC();

  if(buttonC) {
    Serial.println("Clic ! C");
    Mouse.press(MOUSE_LEFT);
  }

    if(buttonZ) {
    Serial.println("Clic ! Z");
    Mouse.press(MOUSE_LEFT);
  }
  if(!buttonC && !buttonZ){
    Mouse.release(MOUSE_LEFT);
  }

  cooY = nchuk.joyY();
  cooY = map(cooY, 0, 255, MinY, MaxY);
  cooX = nchuk.joyX();
  cooX = map(cooX, 0, 255, MinX, MaxX);

  variateur = analogRead(A0);
  variateur = map(variateur, 0, 1023, 1, 12);

  if(cooX < zoneMleft || cooX > zoneMright){
    cooX = map(cooX, 30, 210, -variateur, variateur);
    Mouse.move(cooX, 0, 0);
    Serial.print("X: "); Serial.print(cooX);

  }
  if(cooY < zoneMdown || cooY > zoneMup){
    cooY = map(cooY, 30, 210, variateur, -variateur);
    Mouse.move(0, cooY, 0);
    Serial.print(" - Y: "); Serial.print(cooY);
  }
  Serial.println("");
}

void modeSimple(){
  buttonZ = nchuk.buttonZ();
  buttonC = nchuk.buttonC();
  cooY = nchuk.joyY();
  cooY = map(cooY, 0, 255, MinY, MaxY);
  cooX = nchuk.joyX();
  cooX = map(cooX, 0, 255, MinX, MaxX);

  if(buttonC) { // Gestion bouton
    Serial.println(" Clic ! C");
    Mouse.press(MOUSE_LEFT);
  }
  if(buttonZ) {
    Serial.println(" Clic ! Z");
    Mouse.press(MOUSE_LEFT);
  }

  variateur = analogRead(A0);
  variateur = map(variateur, 0, 1023, 5, 50);

  if(cooX > zoneMleft+variateur){ //Gestion joystick
    Mouse.press(MOUSE_LEFT);
    Serial.print(" Clic ! LEFT");
  }
  if(cooX < zoneMright-variateur){
    Mouse.press(MOUSE_LEFT);
    Serial.println(" Clic ! RIGHT");
  }
  if(cooY < zoneMdown-variateur){
    Mouse.press(MOUSE_LEFT);
    Serial.print(" Clic ! DOWN");
  }
  if(cooY > zoneMup+variateur){
    Mouse.press(MOUSE_LEFT);
    Serial.print(" Clic ! UP");
  }

/*TEST BOF BOF
  variateur = analogRead(A0);
  variateur = map(variateur, 0, 1023, 100, 1200);
  delay(variateur);
  Mouse.release(MOUSE_LEFT);
*/
  
  //Reset clic
  //Serial.println(cooX);
  if(cooX < zoneMleft+variateur && cooX > zoneMright-variateur && cooY > zoneMdown-variateur && cooY < zoneMup+variateur && buttonC == 0 && buttonZ == 0){
    Mouse.release(MOUSE_LEFT);
  }
  
  Serial.println(""); 
}

void calibrate(){
  buttonZ = nchuk.buttonZ();
  buttonC = nchuk.buttonC();
  cooY = nchuk.joyY();
  cooX = nchuk.joyX();
  buttonMode = digitalRead(D1);
  buttonScroll = digitalRead(D2);

  //Init Min et Max
  if(buttonZ && buttonC){
    MinX = 50;
    MaxX = 200;
    MinY = 50;
    MaxY = 200;
    Serial.print(buttonC);Serial.println(buttonZ);
    delay(200);
  }

  while(buttonZ){
    Serial.println("CALIBRATION...");

    nchuk.update();
    buttonZ = nchuk.buttonZ();
    buttonC = nchuk.buttonC();
    cooY = nchuk.joyY();
    cooX = nchuk.joyX();

    if(cooX < 60){
      if(cooX < MinX){MinX = cooX;}
    }if(cooX > 180){
      if(cooX > MaxX){MaxX = cooX;}
    }

    if(cooY < 60){
      if(cooY < MinY){MinY = cooY;}
    }if(cooY > 180){
      if(cooY > MaxY){MaxY = cooY;}
    }

    Serial.print("MinX : "); Serial.println(MinX);
      writeEEPROM(0x50, 5, MinX);
    Serial.print("MaxX : "); Serial.println(MaxX);
      writeEEPROM(0x50, 6, MaxX);
    Serial.print("MinY : "); Serial.println(MinY);
      writeEEPROM(0x50, 7, MinY);
    Serial.print("MaxY : "); Serial.println(MaxY);
      writeEEPROM(0x50, 8, MaxY);

    delay(500);
  }

  affichageMode(mode);

  Serial.print("X: ");Serial.print(cooX);
  Serial.print(" - Y: ");Serial.println(cooY);

}

void affichageMode(int modeAffichage){
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.print("Mode : ");display.println(modeAffichage);

      switch(modeAffichage){
      case 1:

      break;

      case 2:

      break;

      case 3:

      break;

      case 4:

      break;

      case -1:
        display.fillRect( 14, 20, 104, 10, SH110X_BLACK);
        display.drawRect( 14, 20, 104, 10, SH110X_WHITE);
        display.fillRect( 16, 22, map(cooX, 0, 256, 0, 100), 6, SH110X_WHITE);
        
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(14, 31);
        display.print("X : ");display.println(cooX);

        display.fillRect(66, 31, 2, 8, SH110X_WHITE);

        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(77, 31);
        display.print("Y : ");display.println(cooY);

        display.fillRect( 14, 40, 104, 10, SH110X_BLACK);
        display.drawRect( 14, 40, 104, 10, SH110X_WHITE);
        display.fillRect( 16, 42, map(cooY, 0, 256, 0, 100), 6, SH110X_WHITE);
      break;

      default:

      break;
      }
  display.display();
  display.clearDisplay();
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ){
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}
 
byte readEEPROM(int deviceaddress, unsigned int eeaddress){
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

int buttonSelect(int buttonPin){
  unsigned int valButton = analogRead(buttonPin);

  if(valButton > 20 && valButton < 40){return 8;} // UP
  if(valButton > 80 && valButton < 100){return 2;} // DOWN
  if(valButton < 10){return 4;} // LEFT
  if(valButton > 155 && valButton < 175){return 6;} // RIGHT
  if(valButton > 340 && valButton < 360){return 5;} // SELECT
  if(valButton > 1000){return 0;} //RESET
}
