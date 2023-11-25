#ifndef RADIO_PINOUT_H_
#define RADIO_PINOUT_H_

#define RADIO_CTRL_PORT       PORTH
#define RADIO_CTRL_PORT_DIR   DDRH

#define RADIO_INT_PORT        PORTE
#define RADIO_INT_PORT_DIR    DDRE

#define SS_radio    0b10000000
#define SDN         0b01000000
#define Bypass      0b00100000
#define EN_PA       0b00010000
#define nIRQ        0b01000000

#endif /* RADIO_PINOUT_H_ */
