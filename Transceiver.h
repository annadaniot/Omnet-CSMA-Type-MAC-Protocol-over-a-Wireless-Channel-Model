/** Author : Haydn */

#ifndef __WSN_TRANSCEIVER_H_ #define __WSN_TRANSCEIVER_H_ 
 
#include <omnetpp.h> #include <sstream> #include <fstream> #include "messages.h" #include "logging.h" 
 
 
 
#define dBm2mW(y) pow(y,1/10) //converts value of dBm to mW #define mW2dBm(y) 10*log10(y) //converts value of mW to dBm #define MAX_TRANSCEIVER_COUNT 50 //TODO remove this and make the currentTransmissionsList dynamic using namespace omnetpp; 
 
 
class Transceiver : public cSimpleModule {     //is okay to omit the constructor altogether because the compiler generated one is suitable. 
 
    //will have a list of these structs to store current transmissions     //records important features of current transmissions, includes own transmissions     typedef struct currentTransmissions{         int senderID;         float receivedPower; //dbm     }currentTransmissions; 
 
    //either transmit or receive.     //If receive state, the transceiver is ready to receive data or is receiving data     //If transmit state, the transmitter is preparing to transmit data or is transmitting data     //default is to be in the receive state. TODO: on init set to recv state     //Transmit state is entered upon request from the MAC module.     enum RecvTransState{receive, transmit}; 
 
public:     virtual void initialize();     virtual void finish();     void handleMessage(cMessage *msg);     void handleTransmissionRequest(TransmissionRequest *msg);     void handleSignalStart(SignalStart *msg);     void handleSignalStop(SignalStop *msg); 
