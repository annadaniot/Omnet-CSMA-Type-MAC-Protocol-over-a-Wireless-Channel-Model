/** Author: Anna */

#include "PacketGenerator.h" 
 
Define_Module(PacketGenerator); 
 
void PacketGenerator::initialize() 
{     
   iatDistribution = par("iatDistribution")   ;    
   messageSize = par("messageSize"); 
   seqno = par("seqno") ;   
   nodeID = getParentModule()->par("nodeIdentifier"); //transmitter ID     
   generatePacket = new cMessage("GenAppMessage");    
   scheduleAt(simTime() + exponential(iatDistribution/1000), generatePacket); 
} 
 
AppMessage *PacketGenerator::generateMessage() 
{     
   out<< nodeID<< "," <<seqno<<endl;  
   AppMessage *applMsg = new AppMessage();   
   applMsg -> setTimeStamp(simTime());  
   applMsg ->setSenderId(nodeID);  
   applMsg -> setSequenceNumber(seqno++);   
   applMsg -> setMsgSize(messageSize);     
   applMsg ->setKind(APP_MSG);    
   return applMsg; 
} 

void PacketGenerator::handleMessage(cMessage *msg) {     if(msg==generatePacket)     {         scheduleAt(simTime() + exponential(iatDistribution/1000), msg);         AppMessage *applMsg =  generateMessage();         send(applMsg,"out");         EV<<"Generating application message."<<endl;         packetSent++; 
