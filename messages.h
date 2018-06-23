/** Author: Anna */

#include <iostream> 
#include <stdlib.h>     /* malloc, free, rand */ 
#include <stdio.h> 
#include <omnetpp.h> 
#include "AppMessage_m.h" 
#include "CSRequest_m.h" 
#include "CSResponse_m.h" 
#include "MacMessage_m.h" 
#include "SignalStart_m.h" 
#include "SignalStop_m.h" 
#include "TransmissionConfirm_m.h" 
#include "TransmissionIndication_m.h" 
#include "TransmissionRequest_m.h" 
#include "TransmissionIndication_m.h" 
 
 
using namespace omnetpp; 
 
// messages type: 

enum {     
  APP_MSG,     
  MAC_MSG,     
  CS_REQUEST,     
  CS_RESPONSE,     
  SIGNAL_START,     
  SIGNAL_STOP,     
  TRANSMISSION_REQUEST,     
  TRANSMISSION_REQUEST_STATE_1, //First self message     
  TRANSMISSION_REQUEST_STATE_2, //Second self message     
  TRANSMISSION_CONFIRM,     
  TRANSMISSION_INDICATION,     
  BITRATE_WAIT 
}; 
 
 
 
//Field in TransmissionConfim message. If the transceiver is in the transmit state //when TransmissionRequest arrives, it responds with a message of type TransmissionConfirm //in which the field status is set to statusBusy, the Transceiver does no further action. //Transceiver sends statusOK when successful transmission after transition from sending to recv state. #define statusOK 1 #define statusBusy 2 //enum transmissionStatus{statusBusy, statusOK}; 
