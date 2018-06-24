#include "PacketSink.h" 
#include "logging.h" 
 
Define_Module(PacketSink); 
 
void PacketSink::initialize() 
{     
   //set log file name     
   logFileName = generate_log_file_name(par("sinkFile").stringValue(), true);  
   EV << "PacketSink Using log file: " << logFileName << endl;   
   statNumRecveivedPackets = 0; 
} 
 
void PacketSink::handleMessage(cMessage *msg) 
{     
    EV << "PacketSink::handleMessage" << endl; 
 
    if(msg->getKind()==APP_MSG) //if msg kind is an application message     
    {       
        EV<<"App message received";     
        AppMessage *amsg = (AppMessage *)msg;      
        int senderId = amsg ->getSenderId();
        int sequenceNumber = amsg->getSequenceNumber();    
        statNumRecveivedPackets++;    
        EV << "senderId:" << senderId << " sequenceNumber" << sequenceNumber << endl;   
      }     
   delete msg; 
} 
 
void PacketSink::finish() 
{    
    EV<<"Exit Packet Sink. Logging data"<<endl;     std::ofstream file;     file.open(logFileName, std::ios::app); //this appends to end of file     double R = getParentModule()->getParentModule()->par("R");     file << R << ", "<< statNumRecveivedPackets << endl;     file.close(); } 
