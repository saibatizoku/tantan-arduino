#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include "MenuSystem.h"
#include "LiquidCrystal.h"
#include "TanTan.h"

OneWire oneWire(ONE_WIRE_BUS);

SoftwareSerial pHserial(RX_PH, TX_PH);
SoftwareSerial OD_1serial(RX_OD_1, TX_OD_1);
SoftwareSerial OD_2serial(RX_OD_2, TX_OD_2);
SoftwareSerial OD_3serial(RX_OD_3, TX_OD_3);
SoftwareSerial OD_4serial(RX_OD_4, TX_OD_4);


//DallasTemperature busTEMP(&oneWire);

Nodo::Nodo() {
    DeviceAddress termo1 = { 0x28, 0x80, 0x04, 0xFB, 0x04, 0x00, 0x00, 0x90 };
    DeviceAddress termo2 = { 0x28, 0x30, 0x4D, 0xFB, 0x04, 0x00, 0x00, 0x8D };
    DeviceAddress termo3 = { 0x28, 0x46, 0x9A, 0xFA, 0x04, 0x00, 0x00, 0x27 };
    DeviceAddress termo4 = { 0x28, 0xC3, 0x82, 0xFA, 0x04, 0x00, 0x00, 0x64 };
    float valor_pH;
    float valor_OD1;
    float valor_OD2;
    float valor_OD3;
    float valor_OD4;
    float valor_T1;
    float valor_T2;
    float valor_T3;
    float valor_T4;
}

void Nodo::begin() {
  //busTEMP.begin();
  Serial.begin(38400);
  Serial.println("Iniciando...");
  pHserial.begin(38400);|addph
  Serial.println("pH...");
  OD_1serial.begin(38400);
  Serial.println("OD 1...");
  OD_2serial.begin(38400);
  Serial.println("OD 2...");
  OD_3serial.begin(38400);
  Serial.println("OD 3...");
  OD_4serial.begin(38400);
  Serial.println("OD 4...");
}
void Nodo::modo_standby() {
  OD_1serial.print("e\r");
  delay(50);
  OD_1serial.print("e\r");

  OD_2serial.print("e\r");
  delay(50);
  OD_2serial.print("e\r");

  OD_3serial.print("e\r");
  delay(50);
  OD_3serial.print("e\r");

  OD_4serial.print("e\r");
  delay(50);
  OD_4serial.print("e\r");

  pHserial.print("e\r");
  delay(50);
  pHserial.print("e\r");

}
//void Nodo::add_T1(uint8_t* _termo) {
//    //termo1 = _termo;
//}
//float Nodo::read_T(DallasTemperature bus, DeviceAddress _termo) {
//    float _T = bus.getTempC(_termo);
//    return _T;
//}
//float Nodo::read_T1(DeviceAddress _termo) {
//    float _T = busTEMP.getTempC(_termo);
//    valor_T1 = _T;
//    return valor_T1;
//}
//float Nodo::read_T2(DeviceAddress _termo) {
//    float _T = busTEMP.getTempC(_termo);
//    valor_T2 = _T;
//    return valor_T2;
//}
//float Nodo::read_T3(DeviceAddress _termo) {
//    float _T = busTEMP.getTempC(_termo);
//    valor_T3 = _T;
//    return valor_T3;
//}
//float Nodo::read_T4(DeviceAddress _termo) {
//    float _T = busTEMP.getTempC(_termo);
//    valor_T4 = _T;
//    return valor_T4;
//}
//void Nodo::add_pH(SoftwareSerial &serial) {
//    _pH = &serial;
//}
//void Nodo::add_OD1(SoftwareSerial &serial) {
//    _OD1 = &serial;
//}
//void Nodo::add_OD2(SoftwareSerial &serial) {
//    _OD2 = &serial;
//}
//void Nodo::add_OD3(SoftwareSerial &serial) {
//    _OD3 = &serial;
//}
//void Nodo::add_OD4(SoftwareSerial &serial) {
//    _OD4 = &serial;
//}
float Nodo::read_pH() {
    byte _rec = 0;
    char _data[20];
    pHserial.listen();
    pHserial.print("r\r");
    delay(280);
    if (pHserial.available() > 0) {
	_rec = pHserial.readBytesUntil('\r', _data, BUFFER_ATLAS);
	_data[_rec] = 0;
    }
    valor_pH = atof(_data);
    return valor_pH;
}

float Nodo::read_pH(float _temp) {
    byte _rec = 0;
    char _data[20];
    pHserial.listen();
    pHserial.print(_temp);
    pHserial.print("\r");
    pHserial.print("r\r");
    delay(280);
    if (pHserial.available() > 0) {
        _rec = pHserial.readBytesUntil('\r', _data, BUFFER_ATLAS);
        _data[_rec] = 0;
    }
    valor_pH = atof(_data);
    return valor_pH;
}
float Nodo::read_OD1() {
    byte _rec = 0;
    char _data[20];
    OD_1serial.listen();
    OD_1serial.print("r\r");
    delay(250);
    if (OD_1serial.available() > 0) {
        _rec = OD_1serial.readBytesUntil('\r', _data, BUFFER_ATLAS);
        _data[_rec] = 0;
    }
    valor_OD1 = atof(_data);
    return valor_OD1;
}
float Nodo::read_OD2() {
    byte _rec = 0;
    char _data[20];
    OD_2serial.listen();
    OD_2serial.print("r\r");
    delay(250);
    if (OD_2serial.available() > 0) {
        _rec = OD_2serial.readBytesUntil('\r', _data, BUFFER_ATLAS);
        _data[_rec] = 0;
    }
    valor_OD2 = atof(_data);
    return valor_OD2;
}
float Nodo::read_OD3() {
    byte _rec = 0;
    char _data[20];
    OD_3serial.listen();
    OD_3serial.print("r\r");
    delay(250);
    if (OD_3serial.available() > 0) {
        _rec = OD_3serial.readBytesUntil('\r', _data, BUFFER_ATLAS);
        _data[_rec] = 0;
    }
    valor_OD3 = atof(_data);
    return valor_OD3;
}
float Nodo::read_OD4() {
    byte _rec = 0;
    char _data[20];
    OD_4serial.listen();
    OD_4serial.print("r\r");
    delay(250);
    if (OD_4serial.available() > 0) {
        _rec = OD_4serial.readBytesUntil('\r', _data, BUFFER_ATLAS);
        _data[_rec] = 0;
    }
    valor_OD4 = atof(_data);
    return valor_OD4;
}
