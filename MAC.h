/** Author: Anna */

#ifndef __WSN_MAC_H_ #define __WSN_MAC_H_ 
#include <omnetpp.h> 
#include <queue> 
#include <sstream> 
#include "messages.h" 
#include "logging.h" 
 
using namespace omnetpp; 
 
class MAC : public cSimpleModule 
{   
    protected:     int bufferSize;     int maxBackoffs;     
    double backoffDistribution; 
    int backoffCounter;     
    bool isProcessing; 
 
    std::queue<AppMessage *> macBuffer; 
 
    string macLogFileName;     
    int numLostPacketsBuffer;     
    int numLostPacketsBackOff;     
    int numRecvFromGen;     
    int numPacketsSentToTrans; 
 
    MacMessage *mmsg; 
 
    virtual void initialize();     
    virtual void handleMessage(cMessage *msg);     
    virtual void finish();     
    void handleApplicationMessage(AppMessage *msg);     
    void handleMACMessage(MacMessage *msg);     
    void handleCarrierResponse(CSResponse *msg);     
    void handleCarrierRequest(CSRequest *msg);     
    void handleTransmissionConfirm(TransmissionConfirm* msg);     
    void handleTransmissionIndication(TransmissionIndication* msg); 
 
}; 
 
#endif 
