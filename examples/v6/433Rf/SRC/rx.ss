/*
* Copyright (C) 2015 Braxton Nunnally
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
* rx.ss
* simple 433Mhz rf reciever
* driver.
*
* Author: Braxton Nunnally
* Date: 6/24/2015
*/

import <io/gpio>
import <io/signal>

class: Rf {
   loadi PIN 27
   loadi BIT 0
   .recieveBit:
     loadi readBit 1
     cp GPIO.PIN PIN
     wloop readBit readBitb
           call GPIO.read
           same bit GPIO.SIG[signal.HIGH]
           do bit
               cp BIT GPIO.SIG
               return recieveBit 1 
           end
           same bit GPIO.SIG[signal.LOW]
           do bit
               cp BIT GPIO.SIG
               return recieveBit 1
           end
     endwl readBit readBitb     
     ret

   string packet '00000000'
   .recievePacket:
     adr ref packet
     loadi packet 8
     inc ref
     loop getPacket getPacketb 8
          call recieveBit
          r_load ref BIT
     endl getPacket getPacketb
     ret
}

class: Main {
   .RfSetup:
     cp GPIO.PIN Rf.PIN
     cp GPIO.DIR signal.IN
     call GPIO.export
     call GPIO.direction
     printf 'Rf Reciever setup.../n' 
     ret


   .main:
     call RfSetup
     call loop
     ret



   .loop: ; loop recieving bits of information
     call Rf.recieveBit
     printf 'recieved bit: <v,Rf.BIT>/n'
     call Rf.recieveBit
     printf 'recieved bit: <v,Rf.BIT>/n'
     call Rf.recieveBit
     printf 'recieved bit: <v,Rf.BIT>/n'
     call Rf.recieveBit
     printf 'recieved bit: <v,Rf.BIT>/n'
     call Rf.recieveBit
     printf 'recieved bit: <v,Rf.BIT>/n'
     call Rf.recieveBit
     printf 'recieved bit: <v,Rf.BIT>/n'
     call Rf.recieveBit
     printf 'recieved bit: <v,Rf.BIT>/n'
     call Rf.recieveBit
     printf 'recieved bit: <v,Rf.BIT>/n'
     ret

}

call Main.main
