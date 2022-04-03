#include "KidMotorV3.h"

#define ADDR 0x08

KidMotorV3::KidMotorV3(TwoWire *wire) {
    _wire = wire;
}

void KidMotorV3::write(uint8_t address, uint8_t data) {
  _wire->beginTransmission(ADDR);
  _wire->write(address);
  _wire->write(data);
  int error = _wire->endTransmission();
  if (error != 0) {
    Serial.println(error);
  }
}

uint8_t KidMotorV3::read(uint8_t address) {
  _wire->beginTransmission(ADDR);
  _wire->write(address);
  int error = _wire->endTransmission();
  if (error != 0) {
    Serial.println(error);
    return 0;
  }

  int n = _wire->requestFrom(ADDR, 1);
  if (n < 1) {
    Serial.println("Read data error");
    return 0;
  }

  return _wire->read();
}

uint8_t KidMotorV3::read_bytes(uint8_t address, uint8_t *buff, int n) {
  _wire->beginTransmission(ADDR);
  _wire->write(address);
  int error = _wire->endTransmission();
  if (error != 0) {
    Serial.println(error);
    return 0;
  }

  _wire->flush();
  
  int len = _wire->requestFrom(ADDR, n);
  if (len < n) {
    return 0;
  }
  
  return _wire->readBytes(buff, n);
}

void KidMotorV3::setMotor(uint8_t ch, uint8_t dir, uint8_t speed) {
  ch = ch - 1;
  speed = speed * (80.0 / 100.0);
  this->write(ch == 0 ? 0x00 : 0x01, (dir << 7) | (speed & 0x7F));
}

void KidMotorV3::mode(uint8_t ch, uint8_t mode) {
  ch = ch - 1;
  static uint8_t mode_tmp = 0;
  if (mode == 1) {
    mode_tmp |= 1 << ch;
  } else {
    mode_tmp &= ~(1 << ch);
  }
  this->write(0x02, mode_tmp);
}

void KidMotorV3::setOutput(uint8_t ch, uint8_t val) {
    this->mode(ch, MODE_OUTPUT);
  ch = ch - 1;
  static uint8_t out_tmp = 0;
  if (val) {
    out_tmp |= 1 << ch;
  } else {
    out_tmp &= ~(1 << ch);
  }
  this->write(0x04, out_tmp);
}

int KidMotorV3::getInput(uint8_t ch) {
    this->mode(ch, MODE_INPUT);
  ch = ch - 1;
  return (this->read(0x03) & (1 << ch)) ? 1 : 0;
}

int KidMotorV3::getADC(uint8_t ch) {
  ch = ch - 1;
  this->write(0x05, 0x80 | (ch & 0x07)); // Write ADC ch and set FLAG
  do {
    delay(1);
  } while(this->read(0x05) & 0x80 == 0);
  uint8_t buff[2];
  this->read_bytes(0x06, buff, 2);
  return (buff[0] << 8) | buff[1];
}
