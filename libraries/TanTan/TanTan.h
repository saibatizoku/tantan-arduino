/*
   TanTan.h - Library for managing 4 aquaculture tanks
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

        void configura_pins_pH (int rx, int tx);
        int pon_sensor_OD (int rx, int tx);
        void begin();
        void modo_standby();

        float read_pH();
        float read_pH(float temp);
        float read_OD (int num_sensor);

    private:
        boolean pins_pH_configurados;
        int pin_rx_pH;
        int pin_tx_pH;
        SoftwareSerial *pH_serial;

        SoftwareSerial **sensores_OD;
        int num_sensores_OD;
};

#endif
