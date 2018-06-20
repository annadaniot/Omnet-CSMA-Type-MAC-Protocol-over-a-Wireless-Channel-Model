/***Author: Anna****/

#ifndef __WSN_CHANNEL_H_ 
#define __WSN_CHANNEL_H_ 
 
#include <omnetpp.h> 
#include "messages.h" 
using namespace omnetpp; 
 
/**  
* TODO - Generated class  
*/ 

class Channel : public cSimpleModule 
{   
  protected:     
    int numTransmitters; 
  
  virtual void initialize();     
  virtual void handleMessage(cMessage *msg); 
 }; 
 
#endif
