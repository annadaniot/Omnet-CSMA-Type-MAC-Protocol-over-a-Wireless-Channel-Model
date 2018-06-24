/** Author: Anna */

#ifndef __WSN_PACKETGENERATOR_H_ 
#define __WSN_PACKETGENERATOR_H_ 
 
#include <omnetpp.h> 
#include "AppMessage_m.h" 
#include "messages.h" 
 
 
using namespace omnetpp; 
 
/**  * TODO - Generated class  */ 

class PacketGenerator : public cSimpleModule 
{   protected:     double iatDistribution;     int messageSize;     int seqno;     int nodeID; //transmitter ID     int packetSent=0; 
 
    cMessage *generatePacket; 
 
    std::stringstream out; //stream class to operate in out gate 
 
    AppMessage *generateMessage(); 
 
    virtual void initialize();     virtual void handleMessage(cMessage *msg);     virtual void finish(); }; 
 
#endif 
