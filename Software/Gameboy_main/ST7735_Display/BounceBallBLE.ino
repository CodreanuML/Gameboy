#include <Arduino.h>
#include "mbed.h"
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_ST7735.h>
#include <SD.h>
#include <SPI.h>
#include <ArduinoBLE.h>




#define FLAG_VALUE 123

//---------------------------------------- Debug ON/OFF 
#define DEBUG 1
//----------------------------------------


//---------------------------------------- Pinii dintre chip si Display 
#define TFT_CS        10 // Hallowing display control pins: chip select
#define TFT_RST       8 // Display reset
#define TFT_DC        9 // Display data/command select
#define TFT_BACKLIGHT  12
#define TFT_MOSI 11   //Data out
#define TFT_SCLK 13  // Clock out

//---------------------------------------- Definirea Tastaturii
/* definirea tastaturii */
#define OK 2
#define left 7
#define up 3
#define down 4
#define right 5
#define NOK 6

//---------------------------------------- Declarare Variable BLE SI FUNCTII 

uint8_t adv_button_ok_v =0;
uint8_t adv_button_nok_v = 0 ;
uint8_t adv_button_up_v =0;
uint8_t adv_button_down_v = 0 ;
uint8_t adv_button_left_v =0;
uint8_t adv_button_right_v = 0 ;

uint8_t adv_pozitie_minge_x =0;
uint8_t adv_pozitie_minge_y =0;
uint8_t adv_directie_minge =0;
uint8_t adv_deviatie_minge =0;
uint8_t adv_colider_x_minge =0;
uint8_t adv_colider_y_minge =0;

uint8_t adv_pozitie_platforma_stanga_x = 0;
uint8_t adv_pozitie_platforma_stanga_y = 0;


uint8_t adv_pozitie_platfroma_dreapta_x =0;
uint8_t adv_pozitie_platfroma_dreapta_y =0;



//GAT SERVICE - BUTTONS//////////////////////////////BLE
// BLE  Service
BLEService Buttons("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEService Gamedetails("29B10000-E8F2-537E-4F6C-D104768A1214"); 
//CARACTERISTICS
//BUTTONS
BLEByteCharacteristic Button_OK("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic Button_NOK("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic Button_UP("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic Button_DOWN("19B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic Button_LEFT("19B10005-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic Button_RIGHT("19B10006-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
//GAME CARACTERISTICS
BLEByteCharacteristic POZITIE_MINGE_X("29B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic POZITIE_MINGE_Y("29B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic DIRECTIE_MINGE("29B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic DEVITATIE_MINGE("29B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic COLIDER_X_MINGE("29B10005-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic COLIDER_Y_MINGE("29B10006-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);

BLEByteCharacteristic POZITIE_PLATFORMA_STANGA_X("29B10007-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic POZITIE_PLATFORMA_STANGA_Y("29B10008-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic POZITIE_PLATFORMA_DREAPTA_X("29B10009-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
BLEByteCharacteristic POZITIE_PLATFORMA_DREAPTA_Y("29B10010-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);


//DESCRIPTORS
BLEDescriptor Button_OK_D("29B10001-E8F2-537E-4F6C-D104768A1214", "Valoare Button OK");
BLEDescriptor Button_NOK_D("29B10002-E8F2-537E-4F6C-D104768A1214", "Valoare Button NOK");
BLEDescriptor Button_UP_D("29B10003-E8F2-537E-4F6C-D104768A1214", "Valoare Button UP");
BLEDescriptor Button_DOWN_D("29B10004-E8F2-537E-4F6C-D104768A1214", "Valoare Button DOWN");
BLEDescriptor Button_LEFT_D("29B10005-E8F2-537E-4F6C-D104768A1214", "Valoare Button LEFT");
BLEDescriptor Button_RIGHT_D("29B10006-E8F2-537E-4F6C-D104768A1214", "Valoare Button RIGHT");

void init_BLE() ;
void run_BLE();


//---------------------------------------- Declarare Variabile 

//platforma
typedef struct R_Rect{
  //pozitionare fata de punctul de referinta
  uint8_t x ;
  uint8_t y ;
  //latimea platformei
  uint8_t w ;
  //inaltimea platformei
  uint8_t h ;
  //culoarea platformei
  uint8_t color;

  
} platforma ; 

  //declararea platformei din dreapta
  platforma  platforma_stanga ;
  //declararea platformei din stanga
  platforma  platforma_dreapta;

//mingea
typedef struct R_Circle{
  //pozitionare fata de punctul de referinta
 uint8_t x;
 uint8_t y;
 //raza mingii
 uint8_t r;
 //culoarea mingii
 uint8_t color;

  //directia de deplasare
 // 1 -left->righ 2 - right->left ; 
 uint8_t direction;
 
 //deviatia fata de directia de deplasare(efect)
 //0- none ; 1 ->up ; 2 ->down ;
 uint8_t deviation;


 //punctele de coliziune ale mingii
  uint8_t x_colider ;
  uint8_t y_colider ;
}ball;

  //declarare minge
  ball minge;  

//tipurile de deviatie pe care le poate lua mingea
enum Deviatie {Fara_Deviatie,Deviatie_Sus,Deviatie_Jos};

//declararea tastaturii
typedef struct tasta{
  uint8_t button_ok=0;
  uint8_t button_nok=0;
  uint8_t button_up=0;
  uint8_t button_down=0;
  uint8_t button_left=0;
  uint8_t button_right=0;


}tastat ;

//declararea tastaturii
tastat tastatura ;

typedef struct scr{
  uint8_t S_right;
  uint8_t S_left;

}scor;

scor score ;

//declararea logicii de control a jocului
typedef struct game_manag{

  //definirea modului in care se afla jocul 
  //0 ecran de pornire 
  //1 joc in derulare single player
  //2 joc in derulare multy player
  uint8_t game_stance=0 ;

  //stare conexiune
  uint8_t conexiune = 0;

  //pauza 
  uint8_t pauza = 0 ;


}game_manager ;

game_manager manager ;

//----------------------------------------

//---------------------------------------- Declarare Functii

//teren
void afisare_teren();

//afisare scor
void afisare_scor();

//control platforma
//initializare platforma
void initializare_platforma (platforma * platforma_temp,uint8_t x,uint8_t y ,uint8_t w ,uint8_t h , uint8_t color) ;

//afisearea pe display a platformei
void afisare_platforma(platforma * platforma_temp) ;

//preluarea pozitiei in care se afla platforma
uint8_t get_pozitia_platforma(platforma * platforma_temp);

//modificare pozitie platforma sus
void modifica_pozitie_platforma_up(platforma * platforma_temp);
//modificare pozitie platforma jos
void modifica_pozitie_platforma_down(platforma * platforma_temp) ;


//control minge
//initializare minge
void initializare_minge (ball * minge,uint8_t x,uint8_t y ,uint8_t r , uint8_t color) ;
//afisare pe display a mingi
void afisare_minge(ball * minge) ;
//preluare pozitiei x a mingii
uint8_t get_pozitia_minge_x(ball * minge);
//preluarea pozitiei y a mingii
uint8_t get_pozitia_minge_y(ball * minge);
//modificare pozitie minge jos
void modifica_pozitie_minge_up(ball * minge);
//modificare pozitie minge sus
void modifica_pozitie_minge_down(ball * minge) ;
//modificare pozitie minge stanga
void modifica_pozitie_minge_left(ball * minge);
//modificare pozitie minge dreapta
void modifica_pozitie_minge_right(ball * minge) ;
//modificare pozitie minge pe diagonala stanga sus 
void modifica_pozitie_minge_diagonal_left_up(ball * minge);
//modificare pozitie minge pe diagonala stanga jos 
void modifica_pozitie_minge_diagonal_left_down(ball * minge) ;
//modificqare pozitie minge pe diagonala dreapta sus 
void modifica_pozitie_minge_diagonal_right_up(ball * minge);
//modificare pozitie minge pe diagonala dreapta jos 
void modifica_pozitie_minge_diagonal_right_down(ball * minge) ;
//prealuare directiei mingii
uint8_t get_directie_minge(ball * minge);
//schimbarea directiei mingii
void schimba_directie_minge(ball * minge);
//setarea punctelor de coliziune a mingii in functie de unde se afla
void set_colider_point(ball * minge);

//----------------------------------------Coliziuni

//verificarea coliziunii intre platforma din dreapta si minge
uint8_t check_colizion_right(platforma * platforma_dreapta,ball * minge);
//verificarea coliziunii intre platforma din stanga si minge
uint8_t check_colizion_left(platforma * platforma_stanga,ball * minge);
//verificarea coliziunii intre partea de sus si minge
uint8_t check_colizion_up(ball * minge);
//verificarea coliziunii intre partea de jos si minge
uint8_t check_colizion_down(ball * minge);

//generare deviatie dreapta
void modifica_deviatia_dreapta(ball *minge);
//generare deviatie stanga
void modifica_deviatia_stanga(ball * minge);

//----------------------------------------Bucla de joc


void game_loop_single_player();
void game_loop_multy_player();
//---------------------------------------- Functionalitatea tastaturii

//debounce citire buton
int debounce(int button);
//citire butoane tastatura
void read_buttons();
//debug tastatura 
void debug_buttons();

//---------------------------------------- Initializare Clasa Control Display

Adafruit_ST7735 tft_ST7735 = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


void stergere_platforma(platforma * platforma_temp);

void stergere_minge(ball * minge);

void stergere_ecran() ;

void stergere_scor();

void afisare_ecran();

void afisare_ecran_pornire();
//----------------------------------------Initializare Thread


rtos::Thread thread ;
rtos::Mutex  connection_lock;

//----------------------------------------Initializarea Generala


void setup(void) {
  
  // Initializare 
  init_BLE();

  initializare_ecran();
  initializare_serial();


  pinMode(OK,INPUT);
  pinMode(left,INPUT);
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(right,INPUT);


  
  //----------------------------------------
  delay(1000);
  //----------------------------------------

  score.S_right=0;
  score.S_left=0;

  initializare_platforma(&platforma_stanga,0,60,8,40,ST7735_WHITE);
  initializare_platforma(&platforma_dreapta,120,60,8,40,ST7735_WHITE);
  initializare_minge(&minge,60,76,4,ST7735_WHITE);


  

  afisare_ecran();
  afisare_teren();
  
 
  
 
  thread.start(run_BLE);

}

//----------------------------------------Bucla Principala

void loop() {
  
  //Logica jocului depinde de stanceul in care se afla jocul
  switch(manager.game_stance) {
    //selectie mod joc
    case 0 :
      
      read_buttons();
      afisare_ecran_pornire();
      break;
    //single player
    case 1 :
      stergere_ecran();
      stergere_scor();
  
      read_buttons();
      game_loop_single_player();

      afisare_teren();
      afisare_ecran();
      afisare_scor();
      break;
    //multy player
    case 2 :
      stergere_ecran();
      stergere_scor();
  
      read_buttons();
      game_loop_multy_player();

      afisare_teren();
      afisare_ecran();
      afisare_scor();
      break;
  }
  rtos::ThisThread::sleep_for(50);
}

//-----------------------------------------Declarare Functii
//initializare BLE
void init_BLE(){
   if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");
  }
  // set advertised local name and service UUID:
  BLE.setLocalName("Gameboy");
  BLE.setAdvertisedService(Buttons);
  BLE.setAdvertisedService(Gamedetails);
  // add the characteristic to the service
  Buttons.addCharacteristic(Button_OK);
  Buttons.addCharacteristic(Button_NOK);
  Buttons.addCharacteristic(Button_UP);
  Buttons.addCharacteristic(Button_DOWN);  
  Buttons.addCharacteristic(Button_LEFT);
  Buttons.addCharacteristic(Button_RIGHT);

  Gamedetails.addCharacteristic(POZITIE_MINGE_X);
  Gamedetails.addCharacteristic(POZITIE_MINGE_Y);
  Gamedetails.addCharacteristic(DIRECTIE_MINGE);
  Gamedetails.addCharacteristic(DEVITATIE_MINGE);
  Gamedetails.addCharacteristic(COLIDER_X_MINGE);
  Gamedetails.addCharacteristic(COLIDER_Y_MINGE);

  Gamedetails.addCharacteristic(POZITIE_PLATFORMA_STANGA_X);
  Gamedetails.addCharacteristic(POZITIE_PLATFORMA_STANGA_Y);
  Gamedetails.addCharacteristic(POZITIE_PLATFORMA_DREAPTA_X);
  Gamedetails.addCharacteristic(POZITIE_PLATFORMA_DREAPTA_Y);

  // add descriptors to caracteristics
  Button_OK.addDescriptor(Button_OK_D);
  Button_NOK.addDescriptor(Button_NOK_D);
  Button_UP.addDescriptor(Button_UP_D);
  Button_DOWN.addDescriptor(Button_DOWN_D);
  Button_LEFT.addDescriptor(Button_LEFT_D);
  Button_RIGHT.addDescriptor(Button_RIGHT_D);
  //advertise services
  BLE.addService(Buttons);
  BLE.addService(Gamedetails);
  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral, waiting for connections....");
}
//rulare BLE
void run_BLE(){
 
 while (1){ 



  // listen for BLE peripherals to connect:
  // wait for connection
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:

  if (central) {

    Serial.print("Connected to central: ");

    // print the central's MAC address:

    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {

        
        manager.conexiune=1 ;
        manager.pauza = 0;
        

        adv_pozitie_minge_x = minge.x;
        adv_pozitie_minge_y = minge.y;
        adv_directie_minge = minge.direction;
        adv_deviatie_minge = minge.deviation;
        adv_colider_x_minge = minge.x_colider;
        adv_colider_y_minge = minge.y_colider;
        adv_pozitie_platforma_stanga_x = platforma_stanga.x;
        adv_pozitie_platforma_stanga_y = platforma_stanga.y;
        adv_pozitie_platfroma_dreapta_x = platforma_dreapta.x;
        adv_pozitie_platfroma_dreapta_y = platforma_dreapta.y;

        POZITIE_MINGE_X.writeValue(adv_pozitie_minge_x);
        POZITIE_MINGE_Y.writeValue(adv_pozitie_minge_y);
        DIRECTIE_MINGE.writeValue(adv_directie_minge);
        DEVITATIE_MINGE.writeValue(adv_deviatie_minge);
        COLIDER_X_MINGE.writeValue(adv_colider_x_minge);
        COLIDER_Y_MINGE.writeValue(adv_colider_y_minge);

        POZITIE_PLATFORMA_STANGA_X.writeValue(adv_pozitie_platforma_stanga_x);
        POZITIE_PLATFORMA_STANGA_Y.writeValue(adv_pozitie_platforma_stanga_y);
        POZITIE_PLATFORMA_DREAPTA_X.writeValue(adv_pozitie_platfroma_dreapta_x);
        POZITIE_PLATFORMA_DREAPTA_Y.writeValue(adv_pozitie_platfroma_dreapta_y);

        //waiting for notifications   
        if(Button_OK.written()){
          Button_OK.readValue(adv_button_ok_v);
          Serial.println("Valoare Buton ok :");
          Serial.print(adv_button_ok_v);
          Serial.print("\n");
        }
        if(Button_NOK.written()){
          Button_OK.readValue(adv_button_nok_v);
          Serial.println("Valoare Buton nok :");
          Serial.print(adv_button_nok_v);
          Serial.print("\n");
        }
        if(Button_UP.written()){
          Button_UP.readValue(adv_button_up_v);
          Serial.println("Valoare Buton up :");
          Serial.print(adv_button_up_v);
          Serial.print("\n");
        }
        if(Button_DOWN.written()){
          Button_DOWN.readValue(adv_button_down_v);
          Serial.println("Valoare Buton down :");
          Serial.print(adv_button_down_v);
          Serial.print("\n");
        }
        if(Button_LEFT.written()){
          Button_LEFT.readValue(adv_button_left_v);
          Serial.println("Valoare Buton left :");
          Serial.print(adv_button_left_v);
          Serial.print("\n");
        }
        if(Button_RIGHT.written()){
          Button_RIGHT.readValue(adv_button_right_v);
          Serial.println("Valoare Buton right :");
          Serial.print(adv_button_right_v);
          Serial.print("\n");
        }

      } 

        
        manager.conexiune=0 ;
        manager.pauza = 0;
        

    }

   

    Serial.print(F("Disconnected from central: "));

    Serial.println(central.address());

  
    rtos::ThisThread::sleep_for(50);
}
}


void stergere_ecran(){
  //tft_ST7735.fillScreen(ST7735_BLACK);
  stergere_platforma(&platforma_dreapta);
  stergere_platforma(&platforma_stanga);
  stergere_minge(&minge);


}
void afisare_ecran(){
  afisare_platforma(&platforma_dreapta);
  afisare_platforma(&platforma_stanga);
  afisare_minge(&minge);
}
void initializare_ecran(){
  delay(1000);
  tft_ST7735.initR(128*160); //INITR_BLACKTAB
  tft_ST7735.setRotation(2);
  tft_ST7735.fillScreen(ST7735_BLACK);
  delay(1000);
  tft_ST7735.fillScreen(ST7735_WHITE);
  tft_ST7735.setTextColor(ST7735_BLACK);
  tft_ST7735.print("Initializare Ecran");
  delay(1000);
  tft_ST7735.fillScreen(ST7735_BLACK);
}
void initializare_serial(){
  Serial.begin(115200);
}
void afiseaza(char * a){
  if(DEBUG){
    Serial.println(a);
  }
}
void initializare_platforma (platforma * platforma_temp,uint8_t x,uint8_t y ,uint8_t w ,uint8_t h , uint8_t color) {
  platforma_temp->x=x;
  platforma_temp->y=y;
  platforma_temp->w=w;
  platforma_temp->h=h;
  platforma_temp->color=color;
}
void afisare_platforma(platforma * platforma_temp){
  tft_ST7735.fillRect(platforma_temp->x,platforma_temp->y,platforma_temp->w,platforma_temp->h,platforma_temp->color);
}
void modifica_pozitie_platforma_up(platforma * platforma_temp){
  if(platforma_temp->y>0){
  platforma_temp->y=platforma_temp->y-4;
  }
}

void modifica_pozitie_platforma_down(platforma * platforma_temp){
  if(platforma_temp->y<120){
    platforma_temp->y=platforma_temp->y+4;
  }
}

uint8_t get_pozitia_platforma(platforma * platforma_temp){
  return platforma_temp->y;

}

void initializare_minge (ball * minge,uint8_t x,uint8_t y ,uint8_t r , uint8_t color) {
  minge->x=x;
  minge->y=y;
  minge->r=r;
  minge->color=color;
  minge->direction=random(1,3);
  minge->deviation=Fara_Deviatie;
}

void afisare_minge(ball * minge){
  tft_ST7735.fillCircle(minge->x,minge->y ,minge->r, minge->color);
}

uint8_t get_pozitie_minge_x(ball * minge){
  return minge->x;
}

uint8_t get_pozitie_minge_y(ball * minge){
  return minge->y;
}

void modifica_pozitie_minge_up(ball * minge){
  if(minge->y>4){
    minge->y=minge->y-4;
  }
}

void modifica_pozitie_minge_down(ball * minge){
  if(minge->y<156){
    minge->y=minge->y+4;
  }
}

void modifica_pozitie_minge_left(ball * minge){
  if(minge->x>4){
    minge->x=minge->x-4;
  }
}

void modifica_pozitie_minge_right(ball * minge){

  if(minge->x<124){
    minge->x=minge->x+4;
  }

}

void modifica_pozitie_minge_left_up(ball * minge){
  if(minge->y>4){
    minge->y=minge->y-4;
  }
  if(minge->x>4){
    minge->x=minge->x-4;
  }
}

void modifica_pozitie_minge_left_down(ball * minge) {
  if(minge->x>4){
    minge->x=minge->x-4;
  }
    if(minge->y<156){
    minge->y=minge->y+4;
  }
  
}

void modifica_pozitie_minge_right_up(ball * minge){
  if(minge->y>4){
    minge->y=minge->y-4;

  }
    if(minge->x<126){
    minge->x=minge->x+4;
  }

}

void modifica_pozitie_minge_right_down(ball * minge) {
  if(minge->x<124){
    minge->x=minge->x+4;
  }
  if(minge->y<156){
    minge->y=minge->y+4;
  }
}

uint8_t get_directie_minge(ball * minge){
  return minge->direction;
}

void schimba_directie_minge(ball * minge){
  if (minge->direction==1){
    minge->direction=2;
  }
  else {
    minge->direction=1;
  }
}

void set_colider_point(ball * minge){
  if (minge->direction==1){
    minge->x_colider=(minge->x+minge->r);
    minge->y_colider=minge->y;
  }
  if(minge->direction==2){
    minge->x_colider=(minge->x-minge->r);
    minge->y_colider=minge->y;
  }

}

uint8_t check_colizion_right(platforma * platforma_dreapta,ball * minge) {
  
    
   
    if ((minge->y_colider>=platforma_dreapta->y) && (minge->y_colider<=(platforma_dreapta->y+platforma_dreapta->h))){
      return 1;
    }
    else {
      return 0 ;
    }
}

uint8_t check_colizion_left(platforma * platforma_stanga,ball *minge){

    if ((minge->y_colider>=platforma_stanga->y) && (minge->y_colider<=(platforma_stanga->y+platforma_stanga->h))){
      return 1;
    }
    else {
      return 0 ;
    }

}


uint8_t check_colizion_up(ball * minge){
  if(minge->y_colider==4){
    return 1;
  }
  else{
    return 0;
  }
}

uint8_t check_colizion_down(ball * minge){
  if(minge->y_colider==156){
    return 1;
  }
  else{
    return 0 ;
  }

}

void game_loop_single_player(){
  //set colider minge
  set_colider_point(&minge);



  //verifica coliziunea platforma
  if (get_directie_minge(&minge)==1){
   
    if(minge.x_colider==platforma_dreapta.x){
      
      //coliziune platforma
      if(check_colizion_right(&platforma_dreapta,&minge)){
          schimba_directie_minge(&minge);
          modifica_deviatia_dreapta(&minge);
          
      }
      //ratare platforma
      else{
        initializare_minge(&minge,60,76,4,ST7735_WHITE);
        score.S_left++;
      }

    }
  }
  else{
    
    if(minge.x_colider==(platforma_stanga.x+platforma_stanga.w)){
      //coliziune platforma
      if(check_colizion_left(&platforma_stanga,&minge)){
          schimba_directie_minge(&minge);
          modifica_deviatia_stanga(&minge);
      }
      //ratare platforma
      else{
        initializare_minge(&minge,60,76,4,ST7735_WHITE);
        score.S_right++;
      }

    }
  }
  //verifica coliziunea sus / jos 
  if(check_colizion_up(&minge)){
    minge.deviation=Deviatie_Jos;
  }

  if(check_colizion_down(&minge)){
    minge.deviation=Deviatie_Sus;
  }


  //modifica pozitie platforma  stanga
  if(tastatura.button_left && platforma_stanga.y>=4){
    modifica_pozitie_platforma_up(&platforma_stanga);
  }
  if(tastatura.button_up && platforma_stanga.y<156){
    modifica_pozitie_platforma_down(&platforma_stanga);
  }

  //modifica pozitie platforma  dreapta
  if(tastatura.button_right && platforma_dreapta.y>=4){
    modifica_pozitie_platforma_up(&platforma_dreapta);
  }
  if(tastatura.button_down && platforma_dreapta.y<156){
    modifica_pozitie_platforma_down(&platforma_dreapta);
  }

  //modifica pozitie minge in functie de directie si deviatie
  if(minge.direction==2 && minge.deviation==Fara_Deviatie){
    modifica_pozitie_minge_left(&minge);
  }
  else if (minge.direction==2 && minge.deviation==Deviatie_Sus){
    modifica_pozitie_minge_left_up(&minge);
  }
  else if (minge.direction==2 && minge.deviation==Deviatie_Jos){
    modifica_pozitie_minge_left_down(&minge);
  }
  else if (minge.direction==1 && minge.deviation==Fara_Deviatie){
    modifica_pozitie_minge_right(&minge);  
  }
  else if (minge.direction==1 && minge.deviation==Deviatie_Sus){
     modifica_pozitie_minge_right_up(&minge); 
  }
  else if (minge.direction==1 && minge.deviation==Deviatie_Jos){
     modifica_pozitie_minge_right_down(&minge); 
  }
  Serial.println(minge.y_colider);

}


void game_loop_multy_player(){
  //set colider minge
  set_colider_point(&minge);

  if(manager.conexiune){

  //verifica coliziunea platforma
  if (get_directie_minge(&minge)==1){
   
    if(minge.x_colider==platforma_dreapta.x){
      
      //coliziune platforma
      if(check_colizion_right(&platforma_dreapta,&minge)){
          schimba_directie_minge(&minge);
          modifica_deviatia_dreapta(&minge);
          
      }
      //ratare platforma
      else{
        initializare_minge(&minge,60,76,4,ST7735_WHITE);
        score.S_left++;
      }

    }
  }
  else{
    
    if(minge.x_colider==(platforma_stanga.x+platforma_stanga.w)){
      //coliziune platforma
      if(check_colizion_left(&platforma_stanga,&minge)){
          schimba_directie_minge(&minge);
          modifica_deviatia_stanga(&minge);
      }
      //ratare platforma
      else{
        initializare_minge(&minge,60,76,4,ST7735_WHITE);
        score.S_right++;
      }

    }
  }
  //verifica coliziunea sus / jos 
  if(check_colizion_up(&minge)){
    minge.deviation=Deviatie_Jos;
  }

  if(check_colizion_down(&minge)){
    minge.deviation=Deviatie_Sus;
  }


  //modifica pozitie platforma  stanga
  if(adv_button_left_v && platforma_stanga.y>=4){
    modifica_pozitie_platforma_up(&platforma_stanga);
  }
  if(adv_button_up_v && platforma_stanga.y<156){
    modifica_pozitie_platforma_down(&platforma_stanga);
  }

  //modifica pozitie platforma  dreapta
  if(tastatura.button_right && platforma_dreapta.y>=4){
    modifica_pozitie_platforma_up(&platforma_dreapta);
  }
  if(tastatura.button_down && platforma_dreapta.y<156){
    modifica_pozitie_platforma_down(&platforma_dreapta);
  }

  //modifica pozitie minge in functie de directie si deviatie
  if(minge.direction==2 && minge.deviation==Fara_Deviatie){
    modifica_pozitie_minge_left(&minge);
  }
  else if (minge.direction==2 && minge.deviation==Deviatie_Sus){
    modifica_pozitie_minge_left_up(&minge);
  }
  else if (minge.direction==2 && minge.deviation==Deviatie_Jos){
    modifica_pozitie_minge_left_down(&minge);
  }
  else if (minge.direction==1 && minge.deviation==Fara_Deviatie){
    modifica_pozitie_minge_right(&minge);  
  }
  else if (minge.direction==1 && minge.deviation==Deviatie_Sus){
     modifica_pozitie_minge_right_up(&minge); 
  }
  else if (minge.direction==1 && minge.deviation==Deviatie_Jos){
     modifica_pozitie_minge_right_down(&minge); 
  }
  Serial.println(minge.y_colider);
  }
  else{

  }
}

int debounce(int button){

  static int count_m = 0;
  static int button_state_m = 0;

  int current_state_m = digitalRead(button);

      if (current_state_m != button_state_m) {
        
      // Button state is about to be changed, increase counter
        count_m++;
        
      if (count_m >= 4) {
      // The button have not bounced for four checks, change state
        
        button_state_m = current_state_m;
      // If the button was pressed (not released), tell main so
        
        if (button_state_m != 0) {
          
          count_m = 0;
          return 1;
        }
      
      }
      }  
   
    return 0;
    

}


void read_buttons(){
  tastatura.button_ok=debounce(OK);
  tastatura.button_right=debounce(right);
  tastatura.button_up=debounce(up);
  tastatura.button_down=debounce(down);
  tastatura.button_left=debounce(left);
  tastatura.button_nok=debounce(NOK);
}


void debug_buttons(){

  if (tastatura.button_ok){
      Serial.print("OK");
  }

    if (tastatura.button_left){
      Serial.print("print_left");
  }

  if (tastatura.button_up){
      Serial.print("print_up");
  }

    if (tastatura.button_down){
      Serial.print("print_down");   
    }

    
    if (tastatura.button_right){ 
      Serial.print("print_right");
  }

    if (tastatura.button_nok){    
      Serial.print("print_nok");     
    

  }
    Serial.println('\n');

}

//generare deviatie dreapta
void modifica_deviatia_dreapta(ball *minge){
  if(tastatura.button_left && minge->deviation==Deviatie_Jos){
     minge->deviation=Fara_Deviatie; 
  }
  else if (tastatura.button_down && minge->deviation==Deviatie_Sus) {
    minge->deviation=Fara_Deviatie;
  }
  else if (tastatura.button_left && minge->deviation==Fara_Deviatie){
     minge->deviation=Deviatie_Sus;
  }
  else if (tastatura.button_down && minge->deviation==Fara_Deviatie){
    minge->deviation=Deviatie_Jos;
  }
}
//generare deviatie stanga
void modifica_deviatia_stanga(ball * minge){
  if(tastatura.button_right && minge->deviation==Deviatie_Jos){
     minge->deviation=Fara_Deviatie; 
  }
  else if (tastatura.button_up && minge->deviation==Deviatie_Sus) {
    minge->deviation=Fara_Deviatie;
  }
  else if (tastatura.button_right && minge->deviation==Fara_Deviatie){
     minge->deviation=Deviatie_Sus;
  }
  else if (tastatura.button_up && minge->deviation==Fara_Deviatie){
    minge->deviation=Deviatie_Jos;
  }
}
//desenarea terenului de joc
void afisare_teren(){
   tft_ST7735.drawFastHLine(0, 0, 128, ST7735_WHITE);
   tft_ST7735.drawFastHLine(0, 159, 128, ST7735_WHITE); 
   tft_ST7735.drawFastVLine(0, 0, 160, ST7735_WHITE);
   tft_ST7735.drawFastVLine(127, 0, 160, ST7735_WHITE);
   tft_ST7735.drawFastVLine(64, 0, 160, ST7735_WHITE);
   tft_ST7735.drawCircle(64, 76, 10, ST7735_WHITE);
}

//afisare ecranului de inceput si a logicii de inceput
void afisare_ecran_pornire(){

    static uint8_t selectie ;
    
    
    //selectare mod joc
    if (tastatura.button_down ){
        selectie =1 ;
    }
    else if (tastatura.button_up){
      selectie = 0;
    }

    //schimbare mod joc
    if (tastatura.button_ok && selectie==0){
      manager.game_stance = 1;
      
      

    }
    else if (tastatura.button_ok && selectie==1){
       manager.game_stance = 2;
       
    }



    

 
    tft_ST7735.setTextColor(ST7735_WHITE);
    tft_ST7735.setTextSize(0.5);
    tft_ST7735.setCursor(20, 10);
    tft_ST7735.print("Select Game Mode");

   if (!selectie){
    
    //optiune 1
    tft_ST7735.setTextColor(ST7735_WHITE);
    tft_ST7735.setTextSize(0.5);
    tft_ST7735.setCursor(30, 40);
    tft_ST7735.print("Single Player");
   }
  else
    {
    //optiune 2
    tft_ST7735.setTextColor(ST7735_WHITE);
    tft_ST7735.setTextSize(0.5);
    tft_ST7735.setCursor(30, 60);
    tft_ST7735.print("Multy Player");  
    }
    delay(100);
    tft_ST7735.fillScreen(ST7735_BLACK);
}


void afisare_scor(){
  //scor stanga
  tft_ST7735.setTextColor(ST7735_WHITE);
  tft_ST7735.setTextSize(2);
  tft_ST7735.setCursor(4, 4);
  tft_ST7735.print(score.S_left);


  //scor dreapta
  tft_ST7735.setTextColor(ST7735_WHITE);
  tft_ST7735.setTextSize(2);
  tft_ST7735.setCursor(98, 4);
  tft_ST7735.print(score.S_right);
}

//stergere platforma
void stergere_platforma(platforma * platforma_temp){
  tft_ST7735.fillRect(platforma_temp->x,platforma_temp->y,platforma_temp->w,platforma_temp->h,ST7735_BLACK);
}
//stergere minge
void stergere_minge(ball * minge){
  tft_ST7735.fillCircle(minge->x,minge->y ,minge->r, ST7735_BLACK);
}


//stergere scors 
void stergere_scor(){
  //scor stanga
  tft_ST7735.setTextColor(ST7735_BLACK);
  tft_ST7735.setTextSize(2);
  tft_ST7735.setCursor(4, 4);
  tft_ST7735.print(score.S_left);


  //scor dreapta
  tft_ST7735.setTextColor(ST7735_BLACK);
  tft_ST7735.setTextSize(2);
  tft_ST7735.setCursor(98, 4);
  tft_ST7735.print(score.S_right);
}
