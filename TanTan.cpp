/* Copyright 2014 Óscar Justo, Joaquín Rosales
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

String nombre = "Nodo Acuicola";
String version = "v0.2-dev";


Nodo::Nodo ()
{
    PINS_PH_CONFIGURADOS = false;
    sensores_pH     = NULL;
    sensores_OD     = NULL;

    num_sensores_pH = 0;
    num_sensores_OD = 0;
    num_sensores_T = 0;
}

Nodo::~Nodo ()
{
    int i;

    if (PINS_PH_CONFIGURADOS) {
        delete pH_serial;
        pH_serial = NULL;
        PINS_PH_CONFIGURADOS = false;
    }

    for (i = 0; i < num_sensores_OD; i++) {
        delete sensores_OD[i];
        sensores_OD[i] = NULL;
    }

    for (i = 0; i < num_sensores_pH; i++) {
        delete sensores_pH[i];
        sensores_pH[i] = NULL;
    }

    delete[] sensores_OD;
    sensores_OD = NULL;
    num_sensores_OD = 0;

    delete[] sensores_pH;
    sensores_pH = NULL;
    num_sensores_pH = 0;
 
    delete[] bus_temp;
    bus_temp = NULL;
    num_sensores_T = 0;
}

String Nodo::version_info ()
{
    return nombre + " " + version;
}

void Nodo::configura_pins_pH (int rx, int tx)
{
    assert (rx >= 0);
    assert (tx >= 0);
    assert (rx != tx);
    assert (PINS_PH_CONFIGURADOS == false);

    PINS_PH_CONFIGURADOS = true;
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

String Nodo::leer_sensor (String tipo, int idx, String comando)
{
    byte _rec = 0;
    char _data[20];
    char tempval[10];
    float varval;
    String varstr = "";

    assert (idx >= 0);
    assert (idx < num_sensores_OD);
    assert (sizeof (comando) > 0);

    if (tipo == "pH") {
        assert (PINS_PH_CONFIGURADOS != false);

        if (comando == "r\r") {
            varval = leer_pH();
            varstr = dtostrf(varval, 1, 2, tempval);
            return varstr;
        }
        else {
            pH_serial->listen ();
            pH_serial->print (comando);
            delay(280);
            if (pH_serial->available() > 0) {
            _rec = pH_serial->readBytesUntil('\r', _data, sizeof (_data) - 1);
            _data[_rec] = 0;
            }
            return _data;
        }
    }
    if (tipo == "OD") {
        assert (num_sensores_OD > 0);
        if (comando == "r\r") {
            varval = leer_OD(idx);
            varstr = dtostrf(varval, 1, 2, tempval);
            return varstr;
        }
        else {
            sensores_OD[idx]->listen ();
            sensores_OD[idx]->print (comando);
            delay(250);
            if (sensores_OD[idx]->available() > 0) {
                _rec = sensores_OD[idx]->readBytesUntil('\r', _data, sizeof (_data) - 1);
                _data[_rec] = 0;
            }
            return _data;
        }
    }
    return "";
}

int Nodo::pon_sensor_serial (String tipo, int rx, int tx)
{
    SoftwareSerial *sensor = new SoftwareSerial (rx, tx, false);
    int indice;

    assert (rx >= 0);
    assert (tx >= 0);
    assert (rx != tx);
    assert ((tipo == "OD") || (tipo == "pH"));

    if (tipo == "OD") {
        sensores_OD = crece_arreglo_de_SoftwareSerial (sensores_OD, num_sensores_OD);

        indice = num_sensores_OD;
        sensores_OD[indice] = sensor;
        num_sensores_OD++;
    } else if (tipo == "pH") {
        sensores_pH = crece_arreglo_de_SoftwareSerial (sensores_pH, num_sensores_pH);

        indice = num_sensores_pH;
        sensores_pH[indice] = sensor;
        num_sensores_pH++;
    }

    return indice;
}

int Nodo::pon_sensor_OD (int rx, int tx)
{
    return pon_sensor_serial ("OD", rx, tx);
}

int Nodo::pon_sensor_pH (int rx, int tx)
{
    return pon_sensor_serial ("pH", rx, tx);
}

void Nodo::begin ()
{
  Serial.begin(38400);
  Serial.println(version_info());
  Serial.println("Iniciando...");
  if (PINS_PH_CONFIGURADOS)
      pH_serial->begin(38400);

  if (num_sensores_OD > 0) {
      int i;
      for (i = 0; i < num_sensores_OD; i++)
          sensores_OD[i]->begin(38400);
  }
  modo_standby ();
}

void Nodo::modo_standby ()
{
  if (num_sensores_OD > 0) {
      int i;
      for (i = 0; i < num_sensores_OD; i++) {
          sensores_OD[i]->print("e\r");
          delay(50);
          sensores_OD[i]->print("l1\r");
          delay(50);
          sensores_OD[i]->print("e\r");
          Serial.print("OD");
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.println(leer_sensor ("OD", i, "i\r"));
      }
  }

  if (PINS_PH_CONFIGURADOS) {
      pH_serial->print("e\r");
      delay(50);
      pH_serial->print("l1\r");
      delay(50);
      pH_serial->print("e\r");
      Serial.print("pH: ");
      Serial.println(leer_sensor ("pH", 0, "i\r"));
  }
}

float Nodo::leer_pH ()
{
    byte _rec = 0;
    char _data[20];

    assert (PINS_PH_CONFIGURADOS != false);

    pH_serial->listen();
    pH_serial->print("r\r");
    delay(280);
    if (pH_serial->available() > 0) {
	_rec = pH_serial->readBytesUntil('\r', _data, sizeof (_data) - 1);
	_data[_rec] = 0;
    }
    return atof(_data);
}

float Nodo::leer_pH (float _temp)
{
    byte _rec = 0;
    char _data[20];

    assert (PINS_PH_CONFIGURADOS != false);

    pH_serial->listen();
    pH_serial->print(_temp);
    pH_serial->print("\r");
    delay(280);
    if (pH_serial->available() > 0) {
        _rec = pH_serial->readBytesUntil('\r', _data, sizeof (_data) - 1);
        _data[_rec] = 0;
    }
    return atof(_data);
}

float Nodo::leer_OD (int num_sensor)
{
    assert (num_sensor >= 0);
    assert (num_sensor < num_sensores_OD);

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

void Nodo::configura_bus_temperatura (DallasTemperature *_bus_temp)
{
    bus_temp = _bus_temp;
    bus_temp->begin ();
    bus_temp->requestTemperatures ();
    num_sensores_T = bus_temp->getDeviceCount();
}

void Nodo::pedir_temperaturas ()
{
    assert (num_sensores_T > 0);
    bus_temp->requestTemperatures();
}

int Nodo::contar_sensores_temperatura ()
{
    return bus_temp->getDeviceCount();
}

float Nodo::leer_temperatura (uint8_t *sensor)
{
    assert (num_sensores_T > 0);
    return bus_temp->getTempC(sensor);
}

float Nodo::leer_temperatura (int num_sensor)
{
    assert (num_sensores_T > 0);
    assert (num_sensor >= 0);
    assert (num_sensor < contar_sensores_temperatura ());

    return bus_temp->getTempCByIndex(num_sensor);
}
