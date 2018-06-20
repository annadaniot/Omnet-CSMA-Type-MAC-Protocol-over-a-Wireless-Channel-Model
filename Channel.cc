/* Author: Anna */



#include "Channel.h" 
 
Define_Module(Channel); 
 
void Channel::initialize() {     
  numTransmitters=par("numTransmitters");     
  EV << "Starting channel with N number of transmitters: " << numTransmitters + 1; 
} 
 
void Channel::handleMessage(cMessage *msg) 
{ 
  EV<<"Channel::handleMessage"<<endl;     //if messages is of type SignalStart or SignalEnd from Transceiver     
  if(msg->getKind()==SIGNAL_START || msg->getKind()==SIGNAL_STOP)     
  {         
    int i = 0;         
  
    for(i=0; i < numTransmitters; i++) //+1 for receiver 
    {             
     cMessage* copy=msg->dup();             
     send(copy,"out",i);             
     EV << "Sent duplicated incoming message to transmitter/receiver " << i;             
     //   send(i==0?msg : msg->dup(), "out" ,i);         
    }         
   
  send(msg,"out",i++);         
  EV << "Sent duplicated incoming message to transmitter/receiver " << i;     
  } 
} 
