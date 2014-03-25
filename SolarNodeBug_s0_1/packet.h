/*
packet.h - Low power wireless communication based on MSP430G2452
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


//Create packet data structures, must not exceed 32bytes
typedef struct struct_packet_t{
  char name[30]; //30x 1 byte = 30 byte
  uint8_t solar_voltage; //8 bit = 1 byte
  uint8_t temperature; //8 bit = 1 byte
};
//sum: 32 byte

//Making a union so reading as struct or as char array is possible
typedef union packet_t{
 struct_packet_t data;
 uint8_t byte_packet[sizeof(struct_packet_t)];
};
