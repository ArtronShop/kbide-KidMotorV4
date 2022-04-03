#ifndef __KIDMOTORV3_H__
#define __KIDMOTORV3_H__

#include "Arduino.h"
#include "Wire.h"

#define MODE_INPUT 0
#define MODE_OUTPUT 1

class KidMotorV3 {
    private:
        TwoWire *_wire;

        void write(uint8_t address, uint8_t data) ;
        uint8_t read(uint8_t address) ;
        uint8_t read_bytes(uint8_t address, uint8_t *buff, int n) ;

    public:
        KidMotorV3(TwoWire *wire = &Wire) ;

        void setMotor(uint8_t ch, uint8_t dir, uint8_t speed) ;
        void mode(uint8_t ch, uint8_t mode) ;
        void setOutput(uint8_t ch, uint8_t val) ;
        int getInput(uint8_t ch) ;
        int getADC(uint8_t ch) ;

};

#endif