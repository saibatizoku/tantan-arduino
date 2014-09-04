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

*/

#include <assert.h>
#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include "MenuSystem.h"
#include "LiquidCrystal.h"
#include "TanTan.h"



Nodo::Nodo ()
{

    pins_pH_configurados = false;

    sensores_OD     = NULL;
    num_sensores_OD = 0;
}

Nodo::~Nodo ()
{
    int i;

    if (pins_pH_configurados) {
        delete pH_serial;
        pH_serial = NULL;
    }

    for (i = 0; i < num_sensores_OD; i++) {
        delete sensores_OD[i];
        sensores_OD[i] = NULL;
    }
    delete[] sensores_OD;
    sensores_OD = NULL;

    num_sensores_OD = 0;
}

void Nodo::configura_pins_pH (int rx, int tx)
{
    assert (rx >= 0);
    assert (tx >= 0);
    assert (rx != tx);
    assert (pins_pH_configurados == false);

    pins_pH_configurados = true;
    pin_rx_pH = rx;
    pin_tx_pH = tx;

    pH_serial = new SoftwareSerial (pin_rx_pH, pin_tx_pH, false);
}

static SoftwareSerial **
crece_arreglo_de_SoftwareSerial (SoftwareSerial **arr, int n_elementos_existentes)
{
    SoftwareSerial **nuevo_arr = new SoftwareSerial *[n_elementos_existentes + 1];
    int i;

    for (i = 0; i < n_elementos_existentes; i++)
        nuevo_arr[i] = arr[i];

    nuevo_arr[i] = NULL;
    delete[] arr;

    return nuevo_arr;
}

int Nodo::pon_sensor_OD (int rx, int tx)
{
    SoftwareSerial *sensor = new SoftwareSerial (rx, tx, false);
    int indice;

    sensores_OD = crece_arreglo_de_SoftwareSerial (sensores_OD, num_sensores_OD);

    indice = num_sensores_OD;
    sensores_OD[indice] = sensor;
    num_sensores_OD++;

    return indice;
}

void Nodo::begin ()
{
  Serial.begin(38400);
  Serial.println("Iniciando...");
  if (pins_pH_configurados)
      pH_serial->begin(38400);

  if (num_sensores_OD > 0) {
      int i;
      for (i = 0; i < num_sensores_OD; i++)
          sensores_OD[i]->begin(38400);
  }
}

void Nodo::modo_standby ()
{
  if (num_sensores_OD > 0) {
      int i;
      for (i = 0; i < num_sensores_OD; i++) {
          sensores_OD[i]->print("e\r");
          delay(50);
          sensores_OD[i]->print("e\r");
      }
  }

  if (pins_pH_configurados) {
      pH_serial->print("e\r");
      delay(50);
      pH_serial->print("e\r");
  }
}

float Nodo::read_pH ()
{
    byte _rec = 0;
    char _data[20];

    assert (pins_pH_configurados != false);

    pH_serial->listen();
    pH_serial->print("r\r");
    delay(280);
    if (pH_serial->available() > 0) {
	_rec = pH_serial->readBytesUntil('\r', _data, sizeof (_data) - 1);
	_data[_rec] = 0;
    }
    return atof(_data);
}

float Nodo::read_pH (float _temp)
{
    byte _rec = 0;
    char _data[20];

    assert (pins_pH_configurados != false);

    pH_serial->listen();
    pH_serial->print(_temp);
    pH_serial->print("\r");
    pH_serial->print("r\r");
    delay(280);
    if (pH_serial->available() > 0) {
        _rec = pH_serial->readBytesUntil('\r', _data, sizeof (_data) - 1);
        _data[_rec] = 0;
    }
    return atof(_data);
}

float Nodo::read_OD (int num_sensor)
{
    assert (num_sensor >= 0);
    assert (num_sensor < num_sensores_OD);

    //... sensores_OD[num_sensor] ...
    byte _rec = 0;
    char _data[20];
    sensores_OD[num_sensor]->listen();
    sensores_OD[num_sensor]->print("r\r");
    delay(250);
    if (sensores_OD[num_sensor]->available() > 0) {
        _rec = sensores_OD[num_sensor]->readBytesUntil('\r', _data, sizeof (_data) - 1);
        _data[_rec] = 0;
    }
    return atof(_data);
}
