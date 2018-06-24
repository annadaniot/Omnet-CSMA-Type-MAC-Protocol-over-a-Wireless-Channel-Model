#ifndef __WSN_PACKETSINK_H_ 
#define __WSN_PACKETSINK_H_ 
 
#include <omnetpp.h> 
#include <fstream>  //can be ifstream or ofstream 
#include "messages.h" 
#include "AppMessage_m.h" 
 
 
using namespace omnetpp; 
 
/**  * TODO - Generated class  */ 
class PacketSink : public cSimpleModule 
{   
   protected:     std::stringstream out;    //stream class to operate on 'out' gate 
 
   virtual void initialize();   
   virtual void handleMessage(cMessage *msg); 
