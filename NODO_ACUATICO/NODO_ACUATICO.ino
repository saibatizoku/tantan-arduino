/*
   Copyright 2014 Óscar Justo, Joaquín Rosales
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 
 Programa para un nodo acuático con:
 1 sensor de pH
 4 sensores de oxígeno disuelto
 4 sensores de temperatura
 
 Xalapa, Veracruz. México 2014
*/
 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <MenuSystem.h>
#include <LiquidCrystal.h>
#include <TanTan.h>

#define RX_PH 50
#define TX_PH 51

#define RX_OD_1 10
#define TX_OD_1 6

#define RX_OD_2 11
#define TX_OD_2 7

#define RX_OD_3 12
#define TX_OD_3 8

#define RX_OD_4 13
#define TX_OD_4 9

OneWire dsWire(ONE_WIRE_BUS);
DallasTemperature sensoresT(&dsWire);

DeviceAddress termo1 = { 0x28, 0x80, 0x04, 0xFB, 0x04, 0x00, 0x00, 0x90 };
DeviceAddress termo2 = { 0x28, 0x30, 0x4D, 0xFB, 0x04, 0x00, 0x00, 0x8D };
DeviceAddress termo3 = { 0x28, 0x46, 0x9A, 0xFA, 0x04, 0x00, 0x00, 0x27 };
DeviceAddress termo4 = { 0x28, 0xC3, 0x82, 0xFA, 0x04, 0x00, 0x00, 0x64 };

int W_BUTTON = 18;
int S_BUTTON = 19;
int A_BUTTON = 2;
int D_BUTTON = 3;

// Menu variables
MenuSystem ms;
Menu mm("Info estanques");
//MenuItem mm_mi1("Medir pH");
MenuItem mm_mi2("Estanque 1");
MenuItem mm_mi3("Estanque 2");
MenuItem mm_mi4("Estanque 3");
MenuItem mm_mi5("Estanque 4");
Menu mpH1("Sensor de pH");
MenuItem mpH1_mi1("En estanque 1");
MenuItem mpH1_mi2("En estanque 2");
MenuItem mpH1_mi3("En estanque 3");
MenuItem mpH1_mi4("En estanque 4");
Menu mu1("Modo Calibracion");
MenuItem mu1_mi1("Sensor pH");
MenuItem mu1_mi2("OD 1");
MenuItem mu1_mi3("OD 2");
MenuItem mu1_mi4("OD 3");
MenuItem mu1_mi5("OD 4");

LiquidCrystal lcd(30,31,32,33,34,35);

//Variable de inicio
boolean inicial = true;
Nodo nodo;
volatile boolean BOTON_OPRIMIDO = false;
volatile int BOTON_NOMBRE;

void on_item1_selected(MenuItem* p_menu_item)
{
  print_pH();
}

void print_pH()
{
  float pHLect = nodo.read_pH();
  lcd.setCursor(0,1);
  char tempph[5];
  String pHs = dtostrf(pHLect, 1, 2, tempph);
  String msg = "pH:" + pHs;
  Serial.println(msg);
  msg = "pH: " + pHs + "   ";
  lcd.print(msg);
  delay(1500); // so we can look the result on the LCD
}

void on_item2_selected(MenuItem* p_menu_item)
{
  print_OD1();
  print_T1();
}
void print_T1()
{
  //float T1 = nodo.read_T(sensoresT, termo1);
  String pref = "T1:";
  float _T = sensoresT.getTempC(termo1);
  char tempod1[6];
  String _Ts = dtostrf(_T, 1, 2, tempod1);
  lcd.setCursor(0,1);
  String msg = pref + _Ts;
  Serial.print(pref);
  Serial.print(_T);
  msg = pref + " " + _Ts + "    " ;
  lcd.print(msg);
  delay(3000); // so we can look the result on the LCD
}
void print_T2()
{
  //float T1 = nodo.read_T(sensoresT, termo1);
  String pref = "T2:";
  float _T = sensoresT.getTempC(termo2);
  char tempod1[6];
  String _Ts = dtostrf(_T, 1, 2, tempod1);
  lcd.setCursor(0,1);
  String msg = pref + _Ts;
  Serial.print(pref);
  Serial.println(_T);
  msg = pref + " " + _Ts + "    " ;
  lcd.print(msg);
  delay(3000); // so we can look the result on the LCD
}
void print_T3()
{
  //float T1 = nodo.read_T(sensoresT, termo1);
  String pref = "T3:";
  float _T = sensoresT.getTempC(termo3);
  char tempod1[6];
  String _Ts = dtostrf(_T, 1, 2, tempod1);
  lcd.setCursor(0,1);
  String msg = pref + _Ts;
  Serial.print(pref);
  Serial.print(_T);
  msg = pref + " " + _Ts + "    " ;
  lcd.print(msg);
  delay(3000); // so we can look the result on the LCD
}
void print_T4()
{
  //float T1 = nodo.read_T(sensoresT, termo1);
  String pref = "T4:";
  float _T = sensoresT.getTempC(termo4);
  char tempod1[6];
  String _Ts = dtostrf(_T, 1, 2, tempod1);
  lcd.setCursor(0,1);
  String msg = pref + _Ts;
  Serial.print(pref);
  Serial.print(_T);
  msg = pref + " " + _Ts + "    " ;
  lcd.print(msg);
  delay(3000); // so we can look the result on the LCD
}
void print_OD1()
{
  float OD1 = nodo.read_OD1();
  char tempod1[5];
  String OD1s = dtostrf(OD1, 1, 2, tempod1);
  lcd.setCursor(0,1);
  String msg = "OD1:" + OD1s;
  Serial.println(msg);
  msg = "OD1: " + OD1s + "  " ;
  lcd.print(msg);
  delay(3000); // so we can look the result on the LCD
}

void on_item3_selected(MenuItem* p_menu_item)
{
  print_OD2();
  print_T2();
}

void print_OD2()
{
  float OD2 = nodo.read_OD2();
  char tempod1[5];
  String OD2s = dtostrf(OD2, 1, 2, tempod1);
  lcd.setCursor(0,1);
  String msg = "OD2:" + OD2s;
  Serial.println(msg);
  msg = "OD2: " + OD2s + "  " ;
  lcd.print(msg);
  delay(3000); // so we can look the result on the LCD
}

void on_item4_selected(MenuItem* p_menu_item)
{
  print_OD3();
  print_T3();
}

void print_OD3()
{
  float OD3 = nodo.read_OD3();
  char tempod1[5];
  String OD3s = dtostrf(OD3, 1, 2, tempod1);
  lcd.setCursor(0,1);
  String msg = "OD3:" + OD3s;
  Serial.println(msg);
  msg = "OD3: " + OD3s + "  " ;
  lcd.print(msg);
  delay(3000); // so we can look the result on the LCD
}

void on_item5_selected(MenuItem* p_menu_item)
{
  print_OD4();
  print_T4();
}

void print_OD4()
{
  float OD4 = nodo.read_OD4();
  char tempod1[5];
  String OD4s = dtostrf(OD4, 1, 2, tempod1);
  lcd.setCursor(0,1);
  String msg = "OD4:" + OD4s;
  Serial.println(msg);
  msg = "OD4: " + OD4s + "  " ;
  lcd.print(msg);
  delay(3000); // so we can look the result on the LCD
}

void on_cal1_selected(MenuItem* p_menu_item)
{
  lcd.setCursor(0,1);
  lcd.print("Cal pH @4  ");
  delay(1500);
  lcd.setCursor(0,1);
  lcd.print("Cal pH @7  ");
  delay(1500);
  lcd.setCursor(0,1);
  lcd.print("Cal pH @10 ");
  delay(1500);
}

void on_cal2_selected(MenuItem* p_menu_item)
{
  lcd.setCursor(0,1);
  lcd.print("Cal OD 1  ");
  delay(1500); // so we can look the result on the LCD
}

void on_cal3_selected(MenuItem* p_menu_item)
{
  lcd.setCursor(0,1);
  lcd.print("Cal OD 2  ");
  delay(1500); // so we can look the result on the LCD
}

void on_cal4_selected(MenuItem* p_menu_item)
{
  lcd.setCursor(0,1);
  lcd.print("Cal OD 3  ");
  delay(1500); // so we can look the result on the LCD
}

void on_cal5_selected(MenuItem* p_menu_item)
{
  lcd.setCursor(0,1);
  lcd.print("Cal OD 4  ");
  delay(1500); // so we can look the result on the LCD
}

void inicializar_botones() {
  attachInterrupt(0, ISR_S, CHANGE);
  attachInterrupt(1, ISR_W, CHANGE);
  attachInterrupt(4, ISR_D, CHANGE);
  attachInterrupt(5, ISR_A, CHANGE);
}

void ISR_W()
{
  BOTON_OPRIMIDO = true;
  BOTON_NOMBRE = 'w';
}
void ISR_S()
{
  BOTON_OPRIMIDO = true;
  BOTON_NOMBRE = 's';
}
void ISR_A()
{
  BOTON_OPRIMIDO = true;
  BOTON_NOMBRE = 'a';
}
void ISR_D()
{
  BOTON_OPRIMIDO = true;
  BOTON_NOMBRE = 'd';
}

void inicializar_seriales() {
  if (inicial) {
    nodo.modo_standby();
    inicial=false;
  }

}
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensoresT.getTempC(deviceAddress);
  Serial.print(tempC);
  Serial.print(" °C");
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Sensor: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}
void inicializar_sensores() {
  sensoresT.begin();
  Serial.print(sensoresT.getDeviceCount(), DEC);
  Serial.println(" dispositivos.");
  Serial.print("    Sensor temperatura 1: ");
  printAddress(termo1);
  Serial.println();
  Serial.print("    Sensor temperatura 2: ");
  printAddress(termo2);
  Serial.println();
  Serial.print("    Sensor temperatura 3: ");
  printAddress(termo3);
  Serial.println();
  Serial.print("    Sensor temperatura 4: ");
  printAddress(termo4);
  Serial.println();
}

void setup()
{
  nodo.configura_pins_pH(RX_PH, TX_PH);
  nodo.begin();

  inicializar_sensores();
  inicializar_botones();
  inicializar_seriales();

  lcd.begin(16, 2);

  serialPrintHelp();
  Serial.println("Construyendo menu.");
  // Menu setup
  /*
       Menu Structure:
   -Estanque1
   --pH
   -Estanque2
   -Estanque3
   -Estanque4
   -Calibrar
   --pH
   --OD1
   --OD2
   --OD3
   --OD4   
   */
  mm.add_menu(&mpH1);
  mpH1.add_item(&mpH1_mi1, &on_item1_selected);
  mpH1.add_item(&mpH1_mi2, &on_item2_selected);
  mpH1.add_item(&mpH1_mi3, &on_item3_selected);
  mpH1.add_item(&mpH1_mi4, &on_item4_selected);
//  mm.add_item(&mm_mi1, &on_item1_selected);
  mm.add_item(&mm_mi2, &on_item2_selected);
  mm.add_item(&mm_mi3, &on_item3_selected);
  mm.add_item(&mm_mi4, &on_item4_selected);
  mm.add_item(&mm_mi5, &on_item5_selected);
  mm.add_menu(&mu1);
  mu1.add_menu(&mpH1);
//  mu1.add_item(&mu1_mi1, &on_cal1_selected);
  mu1.add_item(&mu1_mi2, &on_cal2_selected);
  mu1.add_item(&mu1_mi3, &on_cal3_selected);
  mu1.add_item(&mu1_mi4, &on_cal4_selected);
  mu1.add_item(&mu1_mi5, &on_cal5_selected);
  ms.set_root_menu(&mm);
  Serial.println("Menu construido.");
  displayMenu();
  print_all();
  displayMenu();
}

void print_all()
{
  print_T1();
  print_T2();
  print_T3();
  print_T4();
//  print_pH();
//  print_OD1();
//  print_OD2();
//  print_OD3();
//  print_OD4();
}

void loop()
{
  sensoresT.requestTemperatures();
  // Handle serial commands
  buttonHandler();
  serialHandler();
  // Wait for two seconds so the output is viewable
  delay(1000);
}

void buttonHandler() {
  if (BOTON_OPRIMIDO) {
    BOTON_OPRIMIDO = false;
//    Serial.print("Boton ");
//    Serial.println((char)BOTON_NOMBRE);
    switch((char)BOTON_NOMBRE) {
    case 'w': // Previus item
      ms.prev();
      displayMenu();
      break;
    case 's': // Next item
      ms.next();
      displayMenu();
      break;
    case 'a': // Back presed
      ms.back();
      displayMenu();
      break;
    case 'd': // Select presed
      ms.select();
      displayMenu();
      break;
    }
  }
}

void displayMenu() {
  lcd.clear();
  lcd.setCursor(0,0);
  // Display the menu
  Menu const* cp_menu = ms.get_current_menu();
  lcd.print(cp_menu->get_name());

  lcd.setCursor(0,1);
  lcd.print(cp_menu->get_selected()->get_name());
}

void serialHandler() {
  char inChar;
  if((inChar = Serial.read())>0) {
    switch (inChar) {
    case 'w': // Previus item
      ms.prev();
      displayMenu();
      break;
    case 's': // Next item
      ms.next();
      displayMenu();
      break;
    case 'a': // Back presed
      ms.back();
      displayMenu();
      break;
    case 'd': // Select presed
      ms.select();
      displayMenu();
      break;
    case '0': // pH
      print_pH();
      displayMenu();
      break;
    case '1': // pH
      print_OD1();
      displayMenu();
      break;
    case '2': // pH
      print_OD2();
      displayMenu();
      break;
    case '3': // pH
      print_OD3();
      displayMenu();
      break;
    case '4': // pH
      print_OD4();
      displayMenu();
      break;
    case '?':
    case 'h': // Display help
      serialPrintHelp();
      break;
    default:
      break;
    }
  }
}

void serialPrintHelp() {
  Serial.println("***************");
  Serial.println("w: go to previus item (up)");
  Serial.println("s: go to next item (down)");
  Serial.println("a: go back (right)");
  Serial.println("d: select \"selected\" item");
  Serial.println("?: print this help");
  Serial.println("h: print this help");
  Serial.println("***************");
}



