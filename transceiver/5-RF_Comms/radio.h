//#include <avr/io.h>

#include "radio_config_Si4464.h"

#include <SPI.h>
//the list of following commans comes from the Silabs AN633

//Boot command
#define POWER_UP                   0x02
//common commands
#define NOP                        0x00
#define PART_INFO                  0x01
#define FUNC_INFO                  0x10
#define SET_PROPERTY               0x11
#define GET_PROPERTY               0x12
#define GPIO_PIN_CFG               0x13
#define FIFO_INFO                  0x15
#define GET_INT_STATUS             0x20
#define REQUEST_DEVICE_STATE       0x33
#define CHANGE_STATE               0x34
#define READ_CMD_BUFF              0x44
#define FRR_A_READ                 0x50
#define FRR_B_READ                 0x51
#define FRR_C_READ                 0x53
#define FRR_D_READ                 0x57
//Cal commands
#define IRCAL                      0x17
#define IRCAL_MANUAL               0x19
//TX commands
#define START_TX                   0x31
#define TX_HOP                     0x31
#define WRITE_TX_FIFO              0x66
//RX commands
#define PACKET_INFO                0x16
#define GET_MODEM_STATUS           0x22
#define START_RX                   0x32
#define RX_HOP                     0x36
#define READ_RX_FIFO               0x77
//Advanced commands
#define GET_ADC_READING            0x14
#define GET_PH_STATUS              0x21
#define GET_CHIP_STATUS            0x23


#define CTS 0xFF


#define RADIO_CTRL_PORT       PORTH
#define RADIO_CTRL_PORT_DIR   DDRH
//#define RADIO_CTRL_PORT       PORTL
//#define RADIO_CTRL_PORT_DIR   DDRL

#define RADIO_INT_PORT        PORTE
#define RADIO_INT_PORT_DIR    DDRE

#define SS_radio    0b10000000
#define SDN         0b01000000
#define Bypass      0b00100000
#define EN_PA       0b00010000
#define nIRQ        0b01000000

/*#define SS_radio    0b00000010

#define SDN         0b00001000
#define EN_PA       0b00010000
#define Bypass      0b00100000*/


static const uint8_t cfg[] = RADIO_CONFIGURATION_DATA_ARRAY;



int radioInit();
void radioShutdown();
uint8_t radioPowerUp();
bool radioCommand( const char* write_buf, char write_len , char* read_buf=0, char read_len=0);
bool radioReady();
void radioBatTemp(float *battery, float *degree);
