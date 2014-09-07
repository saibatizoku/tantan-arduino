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

#ifndef TanTan_h
#define TanTan_h

#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <MenuSystem.h>
#include <LiquidCrystal.h>



class Nodo
{
    public:
        Nodo();
        ~Nodo();

        String info ();
        void configura_pins_pH (int rx, int tx);
        int pon_sensor_serial (String tipo, int rx, int tx);
        int pon_sensor_OD (int rx, int tx);
        int pon_sensor_pH (int rx, int tx);
        void begin ();
        void modo_standby ();

        String leer_sensor (String tipo, int idx=0, String comando="r\r");
        float read_pH ();
        float read_pH (float temp);
        float read_OD (int num_sensor);

    private:
        boolean pins_pH_configurados;
        int pin_rx_pH;
        int pin_tx_pH;
        SoftwareSerial *pH_serial;
        SoftwareSerial **sensores_pH;
        SoftwareSerial **sensores_OD;
        int num_sensores_pH;
        int num_sensores_OD;
};

#endif
