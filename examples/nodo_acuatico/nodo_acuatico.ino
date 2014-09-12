/*
   Copyright 2014 Óscar Justo, Joaquín Rosales

   Xalapa, Veracruz. México 2014

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

int ONE_WIRE_BUS = 23;
OneWire _wire (ONE_WIRE_BUS);
DallasTemperature BUS_TEMPERATURA (&_wire);

int SENSOR_OD1, SENSOR_OD2, SENSOR_OD3, SENSOR_OD4;
int MUESTRAS_POR_LECTURA = 5;

DeviceAddress sensores_TEMP[4] = {
    { 0x28, 0x80, 0x04, 0xFB, 0x04, 0x00, 0x00, 0x90 },
    { 0x28, 0x30, 0x4D, 0xFB, 0x04, 0x00, 0x00, 0x8D },
    { 0x28, 0x46, 0x9A, 0xFA, 0x04, 0x00, 0x00, 0x27 },
    { 0x28, 0xC3, 0x82, 0xFA, 0x04, 0x00, 0x00, 0x64 } };

int SENSOR_T1 = 0;
int SENSOR_T2 = 1;
int SENSOR_T3 = 2;
int SENSOR_T4 = 3;
int W_BUTTON = 18;
int S_BUTTON = 19;
int A_BUTTON = 2;
int D_BUTTON = 3;

MenuSystem menu_principal;
Menu menu_info               ("Info estanques >");
MenuItem mi_estanque_1       ("   Estanque 1  >");
MenuItem mi_estanque_2       ("<  Estanque 2  >");
MenuItem mi_estanque_3       ("<  Estanque 3  >");
MenuItem mi_estanque_4       ("<  Estanque 4  >");
Menu menu_info_pH            ("< Sensor de pH >");
MenuItem mi_pH_1             ("<< Estanque 1 >>");
MenuItem mi_pH_2             ("<< Estanque 2 >>");
MenuItem mi_pH_3             ("<< Estanque 3 >>");
MenuItem mi_pH_4             ("<< Estanque 4   ");
Menu menu_calibrar           ("< Calibrar      ");
MenuItem mc_sensor_serial_1  ("<< Sensor pH    ");
MenuItem mc_sensor_serial_2  ("<<    OD 1    >>");
MenuItem mc_sensor_serial_3  ("<<    OD 2    >>");
MenuItem mc_sensor_serial_4  ("<<    OD 3    >>");
MenuItem mc_sensor_serial_5  ("<<    OD 4      ");

LiquidCrystal lcd(30,31,32,33,34,35);

//Variable de inicio
boolean inicial = true;
Nodo nodo;
volatile boolean BOTON_OPRIMIDO = false;
volatile int BOTON_NOMBRE;


float tomar_muestra_temperatura (int num_sensor)
{
    int i;
    float suma = 0;
    for (i = 0; i < MUESTRAS_POR_LECTURA; i++) {
        suma += nodo.leer_temperatura(sensores_TEMP[num_sensor]);
    }
    return suma / (float)MUESTRAS_POR_LECTURA;
}

float leer_muestra (int num_sensor, String tipo_sensor)
{
    int i;
    float suma = 0;
    if (tipo_sensor.equals("T")) {
        return tomar_muestra_temperatura (num_sensor);
    } else if (tipo_sensor.equals("OD")) {
        for (i = 0; i < MUESTRAS_POR_LECTURA; i++) {
            suma += nodo.leer_OD(num_sensor);
        }
        return suma / (float)MUESTRAS_POR_LECTURA;
    } else if (tipo_sensor.equals("pH")) {
        for (i = 0; i < MUESTRAS_POR_LECTURA; i++) {
            suma += nodo.leer_pH();
        }
        return suma / (float)MUESTRAS_POR_LECTURA;
    }
    return 0.0;
}

void print_TEMP (int idx)
{
    String pref = "T";
    int numidx = idx + 1;
    float _T = leer_muestra(idx, "T");
    char tempod1[6];
    String _Ts = dtostrf(_T, 1, 2, tempod1);
    String msg = pref  + ":"+ _Ts;
    Serial.print(pref + String(numidx)+":");
    Serial.println(_T);
    msg = pref + String(numidx) + ": " + _Ts + "    " ;
    lcd.setCursor(0,1);
    lcd.print(msg);
    delay(1000); // so we can look the result on the LCD
}

void print_OD (int idx)
{
    float _var = leer_muestra(idx, "OD");
    char tempod1[6];
    String _Ts = dtostrf(_var, 1, 2, tempod1);
    String msg = "OD" + String(idx+1) + ":"+ _Ts;
    Serial.println(msg);
    msg = "OD" + String(idx+1) + ": " + _Ts + "    " ;
    lcd.setCursor(0,1);
    lcd.print(msg);
    delay(1000); // so we can look the result on the LCD
}

void print_pH()
{
    float _var = leer_muestra(0, "pH");
    char tempod1[6];
    String pHs = dtostrf(_var, 1, 2, tempod1);
    String msg = "pH:" + pHs;
    Serial.println(msg);
    msg = "pH: " + pHs + "     ";
    lcd.setCursor(0,1);
    lcd.print(msg);
    delay(1000); // so we can look the result on the LCD
}

void on_item1_selected(MenuItem* p_menu_item)
{
    print_pH();
}

void on_item2_selected(MenuItem* p_menu_item)
{
    print_OD (SENSOR_OD1);
    print_TEMP (SENSOR_T1);
}

void on_item3_selected(MenuItem* p_menu_item)
{
    print_OD (SENSOR_OD2);
    print_TEMP(SENSOR_T2);
}

void on_item4_selected(MenuItem* p_menu_item)
{
    print_OD (SENSOR_OD3);
    print_TEMP(SENSOR_T3);
}

void on_item5_selected(MenuItem* p_menu_item)
{
    print_OD (SENSOR_OD4);
    print_TEMP(SENSOR_T4);
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

void configurar_botones() {
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
    float tempC = nodo.leer_temperatura(deviceAddress);
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
void configurar_sensores() {
    nodo.configura_pins_pH (RX_PH, TX_PH);
    nodo.configura_bus_temperatura (&BUS_TEMPERATURA);
    SENSOR_OD1 = nodo.pon_sensor_OD (RX_OD_1, TX_OD_1);
    SENSOR_OD2 = nodo.pon_sensor_OD (RX_OD_2, TX_OD_2);
    SENSOR_OD3 = nodo.pon_sensor_OD (RX_OD_3, TX_OD_3);
    SENSOR_OD4 = nodo.pon_sensor_OD (RX_OD_4, TX_OD_4);
}
void imprimir_info() {
    Serial.print(nodo.contar_sensores_temperatura (), DEC);
    Serial.println(" DS18B20");
    Serial.print("T1: ");
    printAddress(sensores_TEMP[SENSOR_T1]);
    Serial.println();
    Serial.print("T2: ");
    printAddress(sensores_TEMP[SENSOR_T2]);
    Serial.println();
    Serial.print("T3: ");
    printAddress(sensores_TEMP[SENSOR_T3]);
    Serial.println();
    Serial.print("T4: ");
    printAddress(sensores_TEMP[SENSOR_T4]);
    Serial.println();
}

void configurar_menu ()
{
    Serial.println("Construyendo menu.");
    menu_info_pH.add_item(&mi_pH_1, &on_item1_selected);
    menu_info_pH.add_item(&mi_pH_2, &on_item2_selected);
    menu_info_pH.add_item(&mi_pH_3, &on_item3_selected);
    menu_info_pH.add_item(&mi_pH_4, &on_item4_selected);

    menu_info.add_menu(&menu_info_pH);
    menu_info.add_item(&mi_estanque_1, &on_item2_selected);
    menu_info.add_item(&mi_estanque_2, &on_item3_selected);
    menu_info.add_item(&mi_estanque_3, &on_item4_selected);
    menu_info.add_item(&mi_estanque_4, &on_item5_selected);

    menu_info.add_menu(&menu_calibrar);
    menu_calibrar.add_item(&mc_sensor_serial_2, &on_cal2_selected);
    menu_calibrar.add_item(&mc_sensor_serial_3, &on_cal3_selected);
    menu_calibrar.add_item(&mc_sensor_serial_4, &on_cal4_selected);
    menu_calibrar.add_item(&mc_sensor_serial_5, &on_cal5_selected);

    menu_calibrar.add_menu(&menu_info_pH);

    menu_principal.set_root_menu(&menu_info);
    Serial.println("Menu construido.");
}

void setup()
{
    configurar_botones();
    configurar_sensores();

    nodo.begin();
    imprimir_info();
    configurar_menu();

    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0,0);
    // Display the menu
    lcd.print("Nodo Acuicola   ");
    lcd.setCursor(0,1);
    lcd.print("TanTan v0.1     ");
    delay(2500);
    menu_principal.next();
    displayMenu();
}

void imprimir_datos_todos()
{
    print_TEMP(SENSOR_T1);
    print_OD (SENSOR_OD1);
    print_TEMP(SENSOR_T2);
    print_OD (SENSOR_OD2);
    print_TEMP(SENSOR_T3);
    print_OD (SENSOR_OD3);
    print_TEMP(SENSOR_T4);
    print_OD (SENSOR_OD4);
    //  print_pH();
}

void loop()
{
    nodo.pedir_temperaturas();
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
                menu_principal.prev();
                displayMenu();
                break;
            case 's': // Next item
                menu_principal.next();
                displayMenu();
                break;
            case 'a': // Back presed
                menu_principal.back();
                displayMenu();
                break;
            case 'd': // Select presed
                menu_principal.select();
                displayMenu();
                break;
        }
    }
}

void displayMenu() {
    lcd.clear();
    lcd.setCursor(0,0);
    // Display the menu
    Menu const* cp_menu = menu_principal.get_current_menu();
    lcd.print(cp_menu->get_name());

    lcd.setCursor(0,1);
    lcd.print(cp_menu->get_selected()->get_name());
}

void serialHandler() {
    char inChar;
    if((inChar = Serial.read())>0) {
        switch (inChar) {
            case 'w': // Previus item
                menu_principal.prev();
                displayMenu();
                break;
            case 's': // Next item
                menu_principal.next();
                displayMenu();
                break;
            case 'a': // Back presed
                menu_principal.back();
                displayMenu();
                break;
            case 'd': // Select presed
                menu_principal.select();
                displayMenu();
                break;
            case '0': // pH
                print_pH();
                displayMenu();
                break;
            case '1': // pH
                print_OD (SENSOR_OD1);
                displayMenu();
                break;
            case '2': // pH
                print_OD (SENSOR_OD2);
                displayMenu();
                break;
            case '3': // pH
                print_OD (SENSOR_OD3);
                displayMenu();
                break;
            case '4': // pH
                print_OD (SENSOR_OD4);
                displayMenu();
                break;
            case '5': // pH
                imprimir_datos_todos();
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
