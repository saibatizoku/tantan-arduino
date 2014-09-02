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

#define BUFFER_ATLAS 20
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


class Nodo
{
    public:
        Nodo();
        ~Nodo();

	void configura_pins_pH (int rx, int tx);
	
        float valor_pH;
        float valor_OD1;
        float valor_OD2;
        float valor_OD3;
        float valor_OD4;
        float valor_T1;
        float valor_T2;
        float valor_T3;
        float valor_T4;
        void begin();
        void modo_standby();
        //void add_T1(uint8_t* _termo);
        //void add_pH(SoftwareSerial &serial);
        //void add_OD1(SoftwareSerial &serial);
        //void add_OD2(SoftwareSerial &serial);
        //void add_OD3(SoftwareSerial &serial);
        //void add_OD4(SoftwareSerial &serial);
        //float atlas_command(SoftwareSerial &serial, String command);
        float read_pH();
        float read_pH(float temp);
        float read_OD1();
        float read_OD2();
        float read_OD3();
        float read_OD4();
        //float read_T1(DeviceAddress _termo);
        //float read_T2(DeviceAddress _termo);
        //float read_T3(DeviceAddress _termo);
        //float read_T4(DeviceAddress _termo);
        //float read_T(DallasTemperature bus, DeviceAddress _termo);
    private:
        Stream* _pH;
        Stream* _OD1;
        Stream* _OD2;
        Stream* _OD3;
        Stream* _OD4;

	boolean pins_pH_configurados;
	int pin_rx_pH;
	int pin_tx_pH;
    SoftwareSerial *pH_serial;
};

#endif
