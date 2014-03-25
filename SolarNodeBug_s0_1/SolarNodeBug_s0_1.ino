/*
SolarNodeBug.cpp - Low power wireless communication based on MSP430G2452
NFR24L01 and energia.nu environment. 

This code implemebnts basic packet communication via a serial connection.

Copyright (c) 2014 Luka Mustafa - Musti (musti [at] wlan-si.net). All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Enrf24.h>
#include <nRF24L01.h> //for RF comunication 
#include <SPI.h>
#include "packet.h" //for custom RF packets

//chip is MSP430G2452
//Define pins 
#define BAT_V P1_0 //A0-there is a bug, must do analog read directly with A?
#define STATUS_LED P1_1 //uart tx on the ping as well, user rst also - will blink on serial comunication
#define MAIN_LED P1_2
#define SOLAR_V P1_3//A3
#define IRQ P1_4
//P1_5 is SCLK for RF chip
//P1_6 is DI for RF chip
//P1_7 is DO for RF chip
#define CE P2_7
#define CSN P2_6

//Create raio objects and addresses
Enrf24 radio(CE, CSN, IRQ);  // CE, CSN, IRQ pins
const uint8_t rxaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };//broadcast for now  //all caomunication must be on the same address
const uint8_t txaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };//broadcast for now

packet_t packet_tx; //location for saveing reciving and tranmitting packets
packet_t packet_rx;

//optionally add initial values here
const boolean debug=true; //output to console
const byte dotLengt=250; //how long last 1 dot

void setup()
{
  if(debug){
    Serial.begin(9600);//1200);//9600); // pinMode must be after Serial, because we are overriding RX
   // clearAndHome();
    Serial.println("GO");
  }else{
    pinMode(STATUS_LED, OUTPUT); // if serial is enabled, status led should not be used
  }
  //analog inputs via potential dividers, est is to measre and calibrate
  // roughly multiply measured voltage with 2.6
  // 1024bits 1.5V range
  // multiply the measured number of bits with roughly 3.81 for the result in mV
  pinMode(BAT_V, INPUT);
  digitalWrite(BAT_V, LOW); // making sure the pull/up is off
  pinMode(SOLAR_V, INPUT);
  digitalWrite(SOLAR_V, LOW); // making sure the pull/up is off 
  pinMode(MAIN_LED, OUTPUT); 
  
  analogReference(INTERNAL1V5); // 1.5V reference set
  
  analogRead(TEMPSENSOR); // first reading usually wrong
  
  // SPI setup
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  
  // Radio setup
  
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  radio.setRXaddress((void*)rxaddr);
  radio.setTXaddress((void*)txaddr);
  radio.setChannel(5);
  radio.enableRX();
  
  // Power on self-check
    if(debug){
     // dump_radio_status_to_serialport(radio.radioState());
      // This will as well blink the status led 
    }else{
      // if serial is enabled, status led should not be used
      
      digitalWrite(STATUS_LED, LOW);
      delay(100);
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
      digitalWrite(STATUS_LED, HIGH);
      
    } 
/*    //com gets out fo sync
  for(int i=0;i<=255;i++){
    analogWrite(MAIN_LED,i);
    delay(20);
  } */ 
  digitalWrite(MAIN_LED, LOW);
  delay(100);
  digitalWrite(MAIN_LED, HIGH);
  delay(100);
  digitalWrite(MAIN_LED, LOW);
  delay(100);
  digitalWrite(MAIN_LED, HIGH);
  delay(100);
  digitalWrite(MAIN_LED, LOW);
  
  if(!debug){
    digitalWrite(STATUS_LED, LOW);
    digitalWrite(MAIN_LED, HIGH);
    delay(5000);
    digitalWrite(STATUS_LED, HIGH);
    digitalWrite(MAIN_LED, LOW);
  }
  
  
  SetString();//TX string is always the same
  
}

void SetString(){
  
  packet_tx.data.name[0]='L';
  packet_tx.data.name[1]='U';
  packet_tx.data.name[2]='K';
  packet_tx.data.name[3]='A'; 
  packet_tx.data.name[4]=' '; 
  packet_tx.data.name[5]='M';
  packet_tx.data.name[6]='U';
  packet_tx.data.name[7]='S';
  packet_tx.data.name[8]='T'; 
  packet_tx.data.name[9]='A'; 
  packet_tx.data.name[10]='F';
  packet_tx.data.name[11]='A';
  packet_tx.data.name[12]=' ';
  packet_tx.data.name[13]='M';
  packet_tx.data.name[14]='U';
  packet_tx.data.name[15]='S';
  packet_tx.data.name[16]='T';
  packet_tx.data.name[17]='I';
  packet_tx.data.name[18]=' ';
  packet_tx.data.name[19]=' ';
  packet_tx.data.name[20]=' ';
  packet_tx.data.name[21]=' ';
  packet_tx.data.name[22]=' ';
  packet_tx.data.name[23]=' ';
  packet_tx.data.name[24]=' ';
  packet_tx.data.name[25]=' ';
  packet_tx.data.name[26]=' ';
  packet_tx.data.name[27]=' ';
  packet_tx.data.name[28]=' ';    
  packet_tx.data.name[29]=' ';  
  
/*  packet_tx.data.name[0]='M';
  packet_tx.data.name[1]='I';
  packet_tx.data.name[2]='H';
  packet_tx.data.name[3]='A'; 
  packet_tx.data.name[4]=' '; 
  packet_tx.data.name[5]='M';
  packet_tx.data.name[6]='I';
  packet_tx.data.name[7]='H';
  packet_tx.data.name[8]='A'; 
  packet_tx.data.name[9]='R'; 
  packet_tx.data.name[10]='I';
  packet_tx.data.name[11]='X';
  packet_tx.data.name[12]=' ';
  packet_tx.data.name[13]=' ';
  packet_tx.data.name[14]=' ';
  packet_tx.data.name[15]=' ';
  packet_tx.data.name[16]=' ';
  packet_tx.data.name[17]=' ';
  packet_tx.data.name[18]=' ';
  packet_tx.data.name[19]=' ';
  packet_tx.data.name[20]=' ';
  packet_tx.data.name[21]=' ';
  packet_tx.data.name[22]=' ';
  packet_tx.data.name[23]=' ';
  packet_tx.data.name[24]=' ';
  packet_tx.data.name[25]=' ';
  packet_tx.data.name[26]=' ';
  packet_tx.data.name[27]=' ';
  packet_tx.data.name[28]=' ';    
  packet_tx.data.name[29]=' ';    */
}
 // digitalWrite(MAIN_LED, packet_rx.data.led); //controlling the LED

boolean NodeBugOff=false;

void loop()
{
// SetString();
  //morse(packet_tx.data.name);
//}
//  if(!NodeBugOff){

  randomSeed(analogRead(TEMPSENSOR));
  for(byte tests=0; tests<30; tests++){ //30 becasue max string is 30
      
   if(!debug){
   //  get_button(); //watch button to turn off
     digitalWrite(STATUS_LED, LOW );
   //  get_button(); //watch button to turn off
     
   }
   delay(random(dotLengt*9,dotLengt*149)); //minimal and maximal duration of displaying the morse on led (students have names+surnames between 9 and 30)
   if(!debug){
   //  get_button(); //watch button to turn off
     digitalWrite(STATUS_LED, HIGH);
   //  get_button(); //watch button to turn off
     delay(100); //just to see the led blink
    }
    if(!NodeBugOff){
      if(receive_package()){break;} //call to receive a packet - packet_rx structure will be updated
    }
  }
  
  if(!debug){
     digitalWrite(STATUS_LED, HIGH); //packet recivet loop is breaked so turn off led     
    }


  measure_temeparture();
   measure_volt();
   SetString();
   
  transmit_package();
    
 /* }else{
    
    if(!debug){
  //    get_button(); //watch button to turn on
      digitalWrite(STATUS_LED, HIGH); //turn off red LED
      digitalWrite(MAIN_LED, LOW); //turn off main LED
      delay(3000);
    }
    
  }*/
   
   
   
  

  


}


void dump_packet(packet_t* packet,boolean dir){ //data packet, 1 incomming 0 outgoing
 if(debug){
   if(dir){
   //  clearAndHome();
     Serial.print("->Incomming ");
    }else{
     Serial.print("<-Outgoing ");
    }

    Serial.print("packet: ");
    for(char i=0;i<sizeof(struct_packet_t);i++){
      Serial.print(packet->byte_packet[i],HEX);
    }
    Serial.println("");
    Serial.println("Packet dump: ");
    Serial.print("solar_voltage: ");
    Serial.println(packet->data.solar_voltage);
    Serial.print("temperature: ");
    Serial.println(packet->data.temperature);
    for(int a=0; a<30; a++){
      Serial.print(packet->data.name[a]);
    }
    Serial.println();
  }
}


boolean receive_package(){
  // RX sections
  if(radio.available(true)){
    if (radio.read(&packet_rx)){
      if(debug){     
       
        dump_packet(&packet_rx,1);
        
      }
      
      morse(packet_rx.data.name);//packet_rx.data.name);   
      return true; 
    }
    return false;
  }
}

void clearAndHome() 
{ 
  Serial.write(27); 
  Serial.print("[2J"); // clear screen 
  Serial.write(27); // ESC 
  Serial.print("[H"); // cursor to home 
}

void transmit_package(){  
  //create a pointer to the struct and increment it, alternatively passing chars directly could work as well
  byte *ptr = (byte *) &packet_tx;  
  for(char i=0;i<sizeof(packet_tx);i++){
    radio.write(*ptr++);
  }
  radio.flush();  // Force transmit (don't wait for any more data)  
  if(debug){      
    dump_packet(&packet_tx,0);
    dump_radio_status_to_serialport(radio.radioState());  // Should report receive mode
  }else{
    //morse();
  }  
}
/*
void get_button(){
boolean Bstate=false;
  if(!debug){
    
  pinMode(STATUS_LED, INPUT);
  Bstate=!digitalRead(STATUS_LED);
  pinMode(STATUS_LED, OUTPUT);
  
  
  

if(Bstate){
  NodeBugOff=!NodeBugOff;
  digitalWrite(STATUS_LED, HIGH); //turn off red LED
  digitalWrite(MAIN_LED, LOW); //turn off main LED
  delay(5000);
}

  }
}*/
//int volti=0;
void measure_volt(){
//  packet_tx.data.battery_voltage=analogRead(A0);
//volti=analogRead(SOLAR_V);
//volti=analogRead(SOLAR_V);
//Serial.println(volti);
//delay(1000);
packet_tx.data.solar_voltage=(uint8_t)(analogRead(BAT_V));   
/*
if(debug){
       packet_tx.data.solar_voltage=2;//(uint8_t)(analogRead(SOLAR_V));
    }else{
       packet_tx.data.solar_voltage=(uint8_t)(analogRead(BAT_V));   
    }

  packet_tx.data.solar_voltage=5;//(uint8_t)(analogRead(SOLAR_V));
  */
 // packet_tx.data.supply_voltage=analogRead(BAT_V);
// Serial.println(battery_voltage);
}

void measure_temeparture(){
  packet_tx.data.temperature=(uint8_t)(((uint32_t)analogRead(TEMPSENSOR)*27069 - 18169625) *10 >> 16);
//  delay(1000);
}



// DEBUGGING CODE ONLY! TO BE REMOVED IN PRODUCTION

void dump_radio_status_to_serialport(uint8_t status)
{
  if(debug){
  Serial.print("Enrf24 radio transceiver status: ");
  switch (status) {
    case ENRF24_STATE_NOTPRESENT:
      Serial.println("NO TRANSCEIVER PRESENT");
      break;

    case ENRF24_STATE_DEEPSLEEP:
      Serial.println("DEEP SLEEP <1uA power consumption");
      break;

    case ENRF24_STATE_IDLE:
      Serial.println("IDLE module powered up w/ oscillators running");
      break;

    case ENRF24_STATE_PTX:
      Serial.println("Actively Transmitting");
      break;

    case ENRF24_STATE_PRX:
      Serial.println("Receive Mode");
      break;

    default:
      Serial.println("UNKNOWN STATUS CODE");
   }
  }
}


 


const uint8_t MorseTable[36]={ B10000000,B01110000,B01010000,B01100000,B10000000,B11010000,B00100000,B11110000,B11000000,B10000000,B01000000,B10110000,B00000000,B01000000,B00000000,B10000000,B00100000,B10100000,B11100000,B00000000,B11000000,B11100000,B10000000,B01100000,B01000000,B00110000,B00000000,B10000000,B11000000,B11100000,B11110000,B11111000,B01111000,B00111000,B00011000,B00001000 };
const uint8_t MorseLenght[26]={ 2,4,4,3,1,4,3,4,2,4,3,4,2,2,3,4,4,3,3,1,3,4,3,4,4,4 };

void morse(char ConvertThis[30]) {
  
   byte  MLenght=0;
  byte mchar=0;
  
  /*
  MorseTable[0]=B10000000; //A 2
  MorseTable[1]=B01110000; //B 4
  MorseTable[2]=B01010000; //C 4
  MorseTable[3]=B01100000; //D 3
  MorseTable[4]=B10000000; //E 1
  MorseTable[5]=B11010000; //F 4
  MorseTable[6]=B00100000; //G 3
  MorseTable[7]=B11110000; //H 4
  MorseTable[8]=B11000000; //I 2
  MorseTable[9]=B10000000; //J  4
  MorseTable[10]=B01000000; //K 3
  MorseTable[11]=B10110000; //L 4
  MorseTable[12]=B00000000; //M 2
  MorseTable[13]=B01000000; //N 2
  MorseTable[14]=B00000000; //O 3
  MorseTable[15]=B10000000; //P 4
  MorseTable[16]=B00100000; //Q 4
  MorseTable[17]=B10100000; //R 3
  MorseTable[18]=B11100000; //S 3 
  MorseTable[19]=B00000000; //T 1
  MorseTable[20]=B11000000; //U 3
  MorseTable[21]=B11100000; //V 4
  MorseTable[22]=B10000000; //W 3
  MorseTable[23]=B01100000; //X 4
  MorseTable[24]=B01000000; //Y 4
  MorseTable[25]=B00110000; //Z 4

  MorseTable[26]=B00000000; //0 5
  MorseTable[27]=B10000000; //1 5
  MorseTable[28]=B11000000; //2 5
  MorseTable[29]=B11100000; //3 5
  MorseTable[30]=B11110000; //4 5 
  MorseTable[31]=B11111000; //5 5
  MorseTable[32]=B01111000; //6 5
  MorseTable[33]=B00111000; //7 5 
  MorseTable[34]=B00011000; //8 5
  MorseTable[35]=B00001000; //9 5

  */
  
  /*
  MorseLenght[0]=2; //A 2
  MorseLenght[1]=4; //B 4
  MorseLenght[2]=4; //C 4
  MorseLenght[3]=3; //D 3
  MorseLenght[4]=1; //E 1
  MorseLenght[5]=4; //F 4
  MorseLenght[6]=3; //G 3
  MorseLenght[7]=4; //H 4
  MorseLenght[8]=2; //I 2
  MorseLenght[9]=4; //J  4
  MorseLenght[10]=3; //K 3
  MorseLenght[11]=4; //L 4
  MorseLenght[12]=2; //M 2
  MorseLenght[13]=2; //N 2
  MorseLenght[14]=3; //O 3
  MorseLenght[15]=4; //P 4
  MorseLenght[16]=4; //Q 4
  MorseLenght[17]=3; //R 3
  MorseLenght[18]=3; //S 3 
  MorseLenght[19]=1; //T 1
  MorseLenght[20]=3; //U 3
  MorseLenght[21]=4; //V 4
  MorseLenght[22]=3; //W 3
  MorseLenght[23]=4; //X 4
  MorseLenght[24]=4; //Y 4
  MorseLenght[25]=4; //Z 4
  
  */
  

  
  if(debug){
  Serial.println("");
  }
 
  
  for(byte MS=0; MS<30; MS++){

    if(debug){  
    Serial.print(ConvertThis[MS]);
    }
    
    if((ConvertThis[MS]>0x2F && ConvertThis[MS]<0x3A) || (ConvertThis[MS]>0x40 && ConvertThis[MS]<0x5B) ){ //is number or char
      
      if(ConvertThis[MS]>0x2F && ConvertThis[MS]<0x3A){ //is a number
        mchar=ConvertThis[MS]-0x16;
        MLenght=5;
//        Serial.print(mchar);
      }else{
         mchar=ConvertThis[MS]-0x41; //is regular char
         MLenght=MorseLenght[mchar];
      }
    
    
  for (byte ii = 0; ii < MLenght ; ii++) {
      if  (MorseTable[mchar] & (B10000000 >> ii)) {
        if(!debug){
          digitalWrite(MAIN_LED,HIGH); // send 1
          delay(dotLengt); //wait after turning the dot off
          digitalWrite(MAIN_LED,LOW); // send 0
        }else{
          Serial.print(".");
        }       
   }
   else {
     if(!debug){
       digitalWrite(MAIN_LED,HIGH); // send 1
       delay(dotLengt*3); //wait after turning the dash off
       digitalWrite(MAIN_LED,LOW); // send 0
     }else{
       Serial.print("-");
     }     
   }
  delay(dotLengt); //wait after dash or dot 

    }
    
    //wait after letter
    delay(dotLengt); //wait betwene letters 2-1 for compensate after last dash/dot
     
    }else{
      if(ConvertThis[MS-1]==' '){
        break;
      }
         // Serial.print("|");
         // delay(10); //
          delay(dotLengt*6); //wait after space  7-1 for compesate for wait after letter
    }
       
       if(debug){
    Serial.println("");
       }

  }

}

