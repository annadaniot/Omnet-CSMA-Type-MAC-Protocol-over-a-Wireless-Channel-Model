/** Author: Haydn */

#include "Transceiver.h" 
 
#include <cmath> 
#include <ctgmath> 
 
Define_Module(Transceiver); 
int statNumSentPackets = 0; 
//called when the simulation starts 

void Transceiver::initialize() 
{     
   EV << getParentModule()->getName() << ": initialsing Transceiver\n";   
   turnAroundTime = par("turnAroundTime");  
   txPowerDBm = par("txPowerDBm");    
   bitRate = par("bitRate");   
   csThreshDBm = par("csThreshDBm"); 
   noisePowerDBm = par("noisePowerDBm");  
   csTime = par("csTime"); 
 
   Nnodes = getParentModule()->par("numTransmitters");     for(int i = 0; i<Nnodes; i++){         currentTransmissionsList[i] = nullptr;     } 
 
    transceiverState = receive; 
 
    //logging     if(strcmp(getParentModule()->getName(),"receiverNode")  == 0 && //check if conditions meet second tests, log packetsLost             Nnodes>1){         TransceiverLogFileName = generate_log_file_name(par("TransceiverFile2").stringValue(), true);         EV << "Transceiver Using log file: " << TransceiverLogFileName << endl;     }     else if(Nnodes == 1){//settings meet first test, log statNumSentPackets         TransceiverLogFileName = generate_log_file_name(par("TransceiverFile1").stringValue(), true);         EV << "Transceiver Using log file: " << TransceiverLogFileName << endl;     } //    statNumSentPackets = 0; //    packetsLost = 0; 
 
} 
 
//called when the simulation finishes void Transceiver::finish() {     //do not delete anything or cancel timers, all cleanup must be done in the destructor     //use this for statistics     EV <<"NUM SENT PACKETS " << statNumSentPackets << endl;     std::ofstream file; 
