#include "radio.h"




/*
 * This function configures the transceiver
 */
int radioConfig(){

  int byteNumber = 0;
  int cmdNumber =1;
  Serial.print(cfg[byteNumber] != 0x00);
  while(cfg[byteNumber] != 0x00){ // while the length of the command is different of zero (while there is any command left) 
    
    uint8_t cmdLength = cfg[byteNumber]; //the first byte is the length of the command
    uint8_t cmd[cmdLength]; //creates a table to save the current command
    byteNumber++;
    
    memcpy(cmd, &cfg[byteNumber], cmdLength); //copies the command into the array previously created
   
    if (radioCommand(cmd, cmdLength)){//sends command to the radio and checks if the result is true
      if (cmdNumber ==1){
        uint8_t cmd[]= {GET_INT_STATUS, 0x00, 0x00, 0x00}; //Cmd: Clear all interrupt flag
        radioCommand(cmd, sizeof(cmd));
      }
      byteNumber += cmdLength; //moves to the next command
      cmdNumber++; 
    }
    else return cmdNumber; // Returns which configuration command was not exacuted properly
  }
  uint8_t answer[1];
  uint8_t command[]= {SET_PROPERTY, 0x00, 0x01, 0x02, 0x08}; // Sets the low battery threshold to 1.9V
  if (radioCommand(command, sizeof(command), answer, sizeof(answer))){
    return 0; // Radio was configured properly
  }

}


/*
 * This function execute the initialzing sequence of the tranceiver

 */
int radioInit(){
  //sets SS_Radio, SDN, Bypass and EN_PA as output
  RADIO_CTRL_PORT_DIR |= (SS_radio | SDN | Bypass | EN_PA); 
  
  // sets nIRQ as input
  RADIO_INT_PORT_DIR &= ~nIRQ; 
  
  //sets SS_Radio, SDN and bypass HIGH
  RADIO_CTRL_PORT |=(SS_radio | SDN); 

  //sets EN_PA HIGH
  RADIO_CTRL_PORT |= EN_PA;

  //sets Bypass LOW
  RADIO_CTRL_PORT &= ~Bypass;

  //forces the transceiver to reset
  radioShutdown();
  radioPowerUp();

  //Configures the transceiver
  int cfgResult = radioConfig();

  if (cfgResult ==0){ // if the configuration was successful
    uint8_t cmd[]= {GET_INT_STATUS, 0xFB, 0x7F, 0x7F}; //Cmd: Clear all interrupt flag
    if (radioCommand(cmd, sizeof(cmd))){
      return 0; // At this time the radio should be fully configured and READY
    }
  }
  else return cfgResult;
   
}



/*
 * This function will check if the transceiver is ready to accept a new command
 */
bool radioReady(){
  bool stat = false; //If stat = true then the transceiver is ready to receive a new command.
  int count =0;

  //We will give it a thousand try before given up, to let it time to finish what is is doing 
  while ((false!=true) && (count < 1000)){
      RADIO_CTRL_PORT &= ~SS_radio;
      SPI.transfer(READ_CMD_BUFF);
      if (SPI.transfer(0) == CTS){
        stat = true;
      }
      count++;
      RADIO_CTRL_PORT |= SS_radio;
    }
  return stat;
}





/*
 * This function forces the transceiver to shutdown
 */

void radioShutdown(){
  RADIO_CTRL_PORT |= SDN; //turns SDN pin HIGH 
  delay(10);
   
}

/*
 * This function forces the shutdown pin back to LOW, turning on the transceiver
 */
uint8_t radioPowerUp(){
  //Power On Reset
  RADIO_CTRL_PORT &= ~SDN; //sets SDN pin LOW
  delay(20); //the si4464 needs a maximum of 6ms before being able to do anything after powering up
}



/*
 * This function will send a command to the transceiver and recover potential answers
 * 
 * const char* write_buf: address of the array containing the command bytes
 * char write_len: Number of bytes in the command
 * char* read_buf: (Optional) address of the array that will store the answer
 * char* read_len: (Optional) length of the answer
 */
bool radioCommand(const char* write_buf, char write_len, char* read_buf, char read_len)
{
    bool done = false; //if command is executed then done= true
    int count=0;

      
    //Let send the command bytes to the transceiver over SPI
    RADIO_CTRL_PORT &= ~SS_radio; // sets the chip select pin LOW
    if (write_buf && write_len){ 
      while (write_len--){
        SPI.transfer(*write_buf++);
      }
    }
    RADIO_CTRL_PORT |= SS_radio; //sets the chip select pin HIGH


    //Now let's make sure the command was executed and read the answer if there is any
    while ((done!=true) && (count < 15000)){ // Up to 1500 retry to check if the radio is ready 

      RADIO_CTRL_PORT &= ~SS_radio;
      SPI.transfer(READ_CMD_BUFF);
      //Let's check if the radio is ready
      if (SPI.transfer(0) == CTS){
        //and receive the answer if there is any
        if (read_buf && read_len){ 
          while (read_len--){
            *read_buf++ = SPI.transfer(0x00); 
            
          }
        }
        done = true;
      }
      count++;
      RADIO_CTRL_PORT |= SS_radio;
    }
    
    return done; // False if too many attempts at CTS
}


/*
 * This function reads the ADC internal to the transceiver to access the voltage and the temperature of the transceiver
 * 
 * float *battery: address of the variable to store the voltage of the transceiver
 * float *degree: address of the variable to store the temperature of the transceiever
 */
void radioBatTemp(float *battery, float *degree){
  
   if (radioReady()==true){ //Checking that the transceiver is not busy
      uint8_t cmd[]={GET_ADC_READING, 0x18}; // reads only temperature and battery voltage
      uint8_t info[6];
      radioCommand(cmd, sizeof(cmd), info, sizeof(info));
      
      // The raw values of voltage is store on bytes 2 & 3; temperature on bytes 4 & 5
      uint16_t batByte = (info[2]<<8) | info[3];
      uint16_t tempByte = (info[4]<<8) | info[5];

      //Now let's convert that into actual volts and degrees
      *battery = (3*float(batByte))/float(1280);
      *degree = (float(tempByte)*(899/float(4096)))-293;
   }
}
