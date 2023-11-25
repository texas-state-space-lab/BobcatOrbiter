#ifndef RADIO_PINOUT_H_
#define RADIO_PINOUT_H_

#define RADIO_CTRL_PORT       PORTL
#define RADIO_CTRL_PORT_DIR   DDRL

#define RADIO_INT_PORT        PORTE
#define RADIO_INT_PORT_DIR    DDRE

#define SS_radio    0b00000100
#define SDN         0b00001000
#define Bypass      0b00010000
#define EN_PA       0b00100000
#define nIRQ        0b00100000

#endif /* RADIO_PINOUT_H_ */
