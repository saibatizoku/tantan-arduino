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

#define ONE_WIRE_BUS 23

#define RX_OD_1 10
#define TX_OD_1 6

#define RX_OD_2 11
#define TX_OD_2 7

#define RX_OD_3 12
#define TX_OD_3 8

#define RX_OD_4 13
#define TX_OD_4 9


class Nodo
{
    public:
        Nodo();
        ~Nodo();

        void configura_pins_pH (int rx, int tx);
        void begin();
        void modo_standby();

        float read_pH();

        int pon_sensor_OD (int rx, int tx);

        float valor_pH;
        float valor_OD1;
        float valor_OD2;
        float valor_OD3;
        float valor_OD4;
        float valor_T1;
        float valor_T2;
        float valor_T3;
        float valor_T4;

        float read_pH(float temp);
        float read_OD (int num_sensor);
        float read_OD1();
        float read_OD2();
        float read_OD3();
        float read_OD4();
    private:
        boolean pins_pH_configurados;
        int pin_rx_pH;
        int pin_tx_pH;
        SoftwareSerial *pH_serial;

        SoftwareSerial **sensores_OD;
        int num_sensores_OD;
};

#endif
