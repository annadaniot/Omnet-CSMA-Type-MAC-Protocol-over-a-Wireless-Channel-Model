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

 if(strcmp(getParentModule()->getName(),"receiverNode")  == 0 && Nnodes>1){//log packetsLost         EV << "Finishing transceiver, Logging to" << TransceiverLogFileName;         file.open(TransceiverLogFileName, std::ios::app); //this appends to end of file         file << Nnodes << ", "<< statNumSentPackets<<", "<< packetsLost << endl;         EV << "Logged "<< Nnodes << ", "<<statNumSentPackets<<", "<< packetsLost << endl;         file.close();     }     else if(Nnodes == 1){ //log statNumSentPackets         EV << "Finishing transceiver, Logging statNumSentPackets to " << TransceiverLogFileName <<endl;         file.open(TransceiverLogFileName, std::ios::app); //this appends to end of file         double R = getParentModule()->getParentModule()->par("R");         file << R << ", "<< statNumSentPackets << endl;         EV <<"logged " << R << ", "<< statNumSentPackets << endl;         file.close();     } } 
  
//Called whenever the transceiver receives a message void Transceiver::handleMessage(cMessage *msg) {     EV << "Handling message";     short msgKind = msg->getKind();     EV << " cMessage received of kind " << msgKind << endl; 
 
    if(msgKind == TRANSMISSION_REQUEST ||             msgKind == TRANSMISSION_REQUEST_STATE_1 ||             msgKind == TRANSMISSION_REQUEST_STATE_2){         EV << "TRANSMISSION_REQUEST received\n";         TransmissionRequest* msgTransmissionRequest = check_and_cast<TransmissionRequest *>(msg);         handleTransmissionRequest(msgTransmissionRequest);     }     else if(msgKind == CS_REQUEST){         EV << "CS_REQUEST received\n";         CSRequest* msgCSResquest = dynamic_cast<CSRequest *>(msg);         handleCSRequest(msgCSResquest);     }     else if(msgKind == SIGNAL_START){         EV << "SIGNAL_START received\n";         SignalStart* msgSignalStart = check_and_cast<SignalStart *>(msg);         handleSignalStart(msgSignalStart);     }     else if(msgKind == SIGNAL_STOP){         EV << "SIGNAL_STOP received\n";         SignalStop* msgSignalStop = check_and_cast<SignalStop *>(msg);         handleSignalStop(msgSignalStop);     }     else if(msgKind == CS_RESPONSE){         EV << "Self message of CS_RESPONSE received\n";         CSResponse* msgSignalStop = check_and_cast<CSResponse *>(msg);         handleCSResponse(msgSignalStop);     }     else if(msgKind == BITRATE_WAIT){         EV << "Self message of BITRATE_WAIT received\n";         SignalStop* msgBitRateWait = check_and_cast<SignalStop *>(msg);         handleBitRateWait(msgBitRateWait);     } 
 
