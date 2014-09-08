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
   1 sensor de oxígeno disuelto
   1 sensor de temperatura

 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <MenuSystem.h>
#include <LiquidCrystal.h>
#include <TanTan.h>

#define RX_PH 2
#define TX_PH 3

#define RX_OD_1 4
#define TX_OD_1 5

int ONE_WIRE_BUS = 7;

OneWire _wire (ONE_WIRE_BUS);
DallasTemperature BUS_TEMPERATURA (&_wire);

int SENSOR_pH, SENSOR_OD1;
int MUESTRAS_POR_LECTURA = 5;

DeviceAddress sensores_TEMP[1] = { { 0x28, 0xC3, 0x82, 0xFA, 0x04, 0x00, 0x00, 0x64 } };

int SENSOR_T1 = 0;

Nodo nodo;


void setup()
{
    configurar_sensores();
    nodo.begin();
    imprimir_info();
}

void loop()
{
    nodo.pedir_temperaturas ();
    imprimir_datos_todos ();
}

void configurar_sensores () {
    nodo.configura_pins_pH (RX_PH, TX_PH);
    nodo.configura_bus_temperatura (&BUS_TEMPERATURA);
    SENSOR_OD1 = nodo.pon_sensor_OD (RX_OD_1, TX_OD_1);
}

void imprimir_info () {
    Serial.print(nodo.contar_sensores_temperatura (), DEC);
    Serial.println(" DS18B20");
    Serial.print("T1: ");
    printAddress(sensores_TEMP[SENSOR_T1]);
}

void printAddress (DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16) Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}

void imprimir_datos_todos()
{
    print_TEMP (SENSOR_T1);
    print_OD (SENSOR_OD1);
    print_pH();
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
    delay(1000);
}

void print_OD (int idx)
{
    float _var = leer_muestra(idx, "OD");
    char tempod1[6];
    String _Ts = dtostrf(_var, 1, 2, tempod1);
    String msg = "OD" + String(idx+1) + ":"+ _Ts;
    Serial.println(msg);
    msg = "OD" + String(idx+1) + ": " + _Ts + "    " ;
    delay(1000);
}

void print_pH ()
{
    float _var = leer_muestra(0, "pH");
    char tempod1[6];
    String pHs = dtostrf(_var, 1, 2, tempod1);
    String msg = "pH:" + pHs;
    Serial.println(msg);
    msg = "pH: " + pHs + "     ";
    delay(1000);
}

float tomar_muestra_temperatura (int num_sensor)
{
    int i;
    float suma = 0;
    for (i = 0; i < MUESTRAS_POR_LECTURA; i++) {
        suma += nodo.leer_temperatura(num_sensor);
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
            suma += nodo.read_OD(num_sensor);
        }
        return suma / (float)MUESTRAS_POR_LECTURA;
    } else if (tipo_sensor.equals("pH")) {
        for (i = 0; i < MUESTRAS_POR_LECTURA; i++) {
            suma += nodo.read_pH();
        }
        return suma / (float)MUESTRAS_POR_LECTURA;
    }
    return 0.0;
}
