#include <NintendoExtensionCtrl.h>
#include <Mouse.h>
#include <EEPROM.h>

/*
  -Voir haut droite sensi douce (probleme)
  -
*/

int cooX;
int cooY;
int accelX;
int accelY;
int accelZ;
boolean buttonZ;
boolean buttonC;
boolean buttonMode;
long variateur = 0;
int mode = 0;
boolean etat = 0;

//Setup zone morte:
int zoneMup = 129;
int zoneMdown = 126;
int zoneMright = 124;
int zoneMleft = 127;

/*
PIN variateur:         A0
PIN bouton Mode:       12
PIN led mode1:         11
PIN led mode2:         10
PIN led mode3:          9
*/

Nunchuk nchuk;

void setup(){

  Serial.begin(9600);
  mode = EEPROM.read(0);

  //mode = -1;  //MODE CALIBRATION ACTIF !!!!!!!

  Mouse.begin();
  nchuk.begin();

  pinMode(A0, INPUT);       //Variateur Vitesse
  pinMode(12, INPUT_PULLUP); //Bouton Mode
  pinMode(9, OUTPUT);       //Led Mode1
  pinMode(10, OUTPUT);       //Led Mode2
  pinMode(11, OUTPUT);       //Led Mode3

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
  buttonMode = digitalRead(12);

  Serial.println(buttonMode);

  if(!etat){
    Serial.println("! Nunchuk deconnecte !");
    digitalWrite(9, 0); digitalWrite(10, 0); digitalWrite(11, 0);
    delay(1000);
    nchuk.connect();
  }else{
    if(!buttonMode){
      mode++;
      if(mode > 4){
        mode = 1;
        EEPROM.write(0, mode);
        Serial.print("Nouveau mode: "); Serial.println(mode);
      }else{
        EEPROM.write(0, mode);
        Serial.print("Nouveau mode: "); Serial.println(mode);
      }
      delay(240);
    }

    switch(mode){
      case 1:
        Serial.println("Mode 1");
        digitalWrite(9, 1); digitalWrite(10, 0); digitalWrite(11, 0);
        modeSimple();
      break;

      case 2:
        Serial.println("Mode 2");
        digitalWrite(9, 0); digitalWrite(10, 1); digitalWrite(11, 0);
        modeIntermediaire();
      break;

      case 3:
        Serial.println("Mode 3");
        digitalWrite(9, 0); digitalWrite(10, 0); digitalWrite(11, 1);
        modeComplet();
      break;

      case 4:
        Serial.println("Mode 4");
        digitalWrite(9, 0); digitalWrite(10, 0); digitalWrite(11, 0);
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

  if(buttonZ || buttonC) {
    Serial.println("Double Click");
    Mouse.press(MOUSE_LEFT);
    delay(50);
    Mouse.release(MOUSE_LEFT);
    delay(100);
    Mouse.press(MOUSE_LEFT);
    delay(50);
    Mouse.release(MOUSE_LEFT);
  }

  cooY = nchuk.joyY();
  cooX = nchuk.joyX();

  variateur = analogRead(A0);
  variateur = map(variateur, 0, 1023, 50, 500);

  if(cooY < zoneMdown) {
      Serial.println("Scroll Down");
      Mouse.move(0, 0, -1);
      delay(variateur);
    }
    if(cooY > zoneMup) {
      Serial.println("Scroll Up");
      Mouse.move(0, 0, 1);
      delay(variateur);
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
  cooX = nchuk.joyX();

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
  cooX = nchuk.joyX();

  if(buttonC) { // Gestion bouton
    Serial.println(" Clic ! C");
    Mouse.press(MOUSE_LEFT);
  }
  if(buttonZ) {
    Serial.println(" Clic ! Z");
    Mouse.press(MOUSE_LEFT);
  }

  if(cooX > zoneMleft+3){ //Gestion joystick
    Mouse.press(MOUSE_LEFT);
    Serial.print(" Clic ! LEFT");
  }
  if(cooX < zoneMright-3){
    Mouse.press(MOUSE_LEFT);
    Serial.println(" Clic ! RIGHT");
  }
  if(cooY < zoneMdown-3){
    Mouse.press(MOUSE_LEFT);
    Serial.print(" Clic ! DOWN");
  }
  if(cooY > zoneMup+3){
    Mouse.press(MOUSE_LEFT);
    Serial.print(" Clic ! UP");
  }

  //Reset clic
  Serial.println(cooX);
  if(cooX < zoneMleft+3 && cooX > zoneMright-3 && cooY > zoneMdown-3 && cooY < zoneMup+3 && buttonC == 0 && buttonZ == 0){
    Mouse.release(MOUSE_LEFT);
    Serial.println("");
    Serial.println("RELEASE");
  }
  Serial.println(""); 
}

void calibrate(){
  buttonZ = nchuk.buttonZ();
  buttonC = nchuk.buttonC();
  if(buttonZ) {
    Serial.print("Z ");
  }
  if(buttonC) {
    Serial.print("C ");
  }Serial.println("");

  cooY = nchuk.joyY();
  cooX = nchuk.joyX();

  Serial.print("X: ");Serial.print(cooX);
  Serial.print(" - Y: ");Serial.println(cooY);

  accelX = nchuk.accelX(); Serial.print("accelX: "); Serial.print(accelX);
  accelY = nchuk.accelY(); Serial.print(" - accelY: "); Serial.print(accelY);
  accelZ = nchuk.accelZ(); Serial.print(" - accelZ: "); Serial.println(accelZ);
}
