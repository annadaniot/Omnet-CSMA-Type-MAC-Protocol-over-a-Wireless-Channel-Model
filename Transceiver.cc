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
 
   Nnodes = getParentModule()->par("numTransmitters");     
   for(int i = 0; i<Nnodes; i++)
   {         
      currentTransmissionsList[i] = nullptr;
   } 
 
    transceiverState = receive; 
 
    //logging     
    //check if conditions meet second tests, log packetsLost
    if(strcmp(getParentModule()->getName(),"receiverNode")  == 0 &&  Nnodes>1)
    {       
       TransceiverLogFileName = generate_log_file_name(par("TransceiverFile2").stringValue(), true);
       EV << "Transceiver Using log file: " << TransceiverLogFileName << endl;    
    }     
    else if(Nnodes == 1)
    {
       //settings meet first test, log statNumSentPackets 
       TransceiverLogFileName = generate_log_file_name(par("TransceiverFile1").stringValue(), true);  
       EV << "Transceiver Using log file: " << TransceiverLogFileName << endl;
    } 
    //    statNumSentPackets = 0; 
    //    packetsLost = 0; 
 } 
 
//called when the simulation finishes 

void Transceiver::finish() 
{     
   //do not delete anything or cancel timers, all cleanup must be done in the destructor  
   //use this for statistics    
   EV <<"NUM SENT PACKETS " << statNumSentPackets << endl;
   std::ofstream file; 

   if(strcmp(getParentModule()->getName(),"receiverNode")  == 0 && Nnodes>1)
   {
      //log packetsLost   
      EV << "Finishing transceiver, Logging to" << TransceiverLogFileName;   
      file.open(TransceiverLogFileName, std::ios::app); //this appends to end of file       
      file << Nnodes << ", "<< statNumSentPackets<<", "<< packetsLost << endl; 
      EV << "Logged "<< Nnodes << ", "<<statNumSentPackets<<", "<< packetsLost << endl;  
      file.close();     
   }     
   else if(Nnodes == 1)
   { 
      //log statNumSentPackets  
      EV << "Finishing transceiver, Logging statNumSentPackets to " << TransceiverLogFileName <<endl;   
      file.open(TransceiverLogFileName, std::ios::app); //this appends to end of file       
      double R = getParentModule()->getParentModule()->par("R"); 
      file << R << ", "<< statNumSentPackets << endl;   
      EV <<"logged " << R << ", "<< statNumSentPackets << endl;     
      file.close();     
   }
} 
  
 //Called whenever the transceiver receives a message void Transceiver::handleMessage(cMessage *msg) {     EV << "Handling message";     short msgKind = msg->getKind();     EV << " cMessage received of kind " << msgKind << endl; 
 if(msgKind == TRANSMISSION_REQUEST || msgKind == TRANSMISSION_REQUEST_STATE_1 || msgKind == TRANSMISSION_REQUEST_STATE_2)
 {
     EV << "TRANSMISSION_REQUEST received\n"; 
     TransmissionRequest* msgTransmissionRequest = check_and_cast<TransmissionRequest *>(msg);   
     handleTransmissionRequest(msgTransmissionRequest);
 }     
 else if(msgKind == CS_REQUEST)
 {
    EV << "CS_REQUEST received\n";   
    CSRequest* msgCSResquest = dynamic_cast<CSRequest *>(msg);       
    handleCSRequest(msgCSResquest);  
 }
 else if(msgKind == SIGNAL_START)
 { 
    EV << "SIGNAL_START received\n";   
    SignalStart* msgSignalStart = check_and_cast<SignalStart *>(msg); 
    handleSignalStart(msgSignalStart);   
 }  
 else if(msgKind == SIGNAL_STOP)
 { 
    EV << "SIGNAL_STOP received\n";    
    SignalStop* msgSignalStop = check_and_cast<SignalStop *>(msg);      
    handleSignalStop(msgSignalStop);    
  }    
  else if(msgKind == CS_RESPONSE)
  {
    EV << "Self message of CS_RESPONSE received\n";    
    CSResponse* msgSignalStop = check_and_cast<CSResponse *>(msg); 
    handleCSResponse(msgSignalStop);  
  }
  else if(msgKind == BITRATE_WAIT)
  {
     EV << "Self message of BITRATE_WAIT received\n";   
     SignalStop* msgBitRateWait = check_and_cast<SignalStop *>(msg);   
     handleBitRateWait(msgBitRateWait); 
  } 
  else
  {    
   //should never get here    
   //abort program 
   EV<<"Transceiver is dropping message of kind "<< (msg->getKind()) <<endl;     
   delete msg; 
    //        throw cRuntimeError("Unrecognised message, aborting");      
    //        endSimulation();     
  } 
} 
 
void Transceiver::handleBitRateWait(SignalStop* msg)
{     
  //program wated packetlen/bitrate, now send signal stop to channel     
  msg->setKind(SIGNAL_STOP);     
  send(msg,"tx2ChanOut"); 
 
  TransmissionRequest* newMsg = new TransmissionRequest();     
  //Wait for a time specified by the TurnaroundTime   
  newMsg->setKind(TRANSMISSION_REQUEST_STATE_2); 
  //set context pointer to turnAroundState
  scheduleAt(simTime()+turnAroundTime, newMsg); 
  //Send the message to itself after waiting turnAroundTime + time to send signalStop 
 } 
 
void Transceiver::handleCSResponse(CSResponse* msg)
{  
   EV << "Sending CSResponse to MAC" << endl;  
   send(msg, "tx2MacOut"); 
} 
 
//regardless of the state the transceiver is in, it has to process these messages 
//TODO do transmit power calculation here 
void Transceiver::handleSignalStart(SignalStart *msg)
{     
  EV << "handleSignalStart\n"; 
 
  int emptyIndex = -1;   
  int numberOfTransmissions = 0; 

  //traverse list to see if the transmission is present    
  for(int i = 0;i<Nnodes; i++)
  {       
     if(currentTransmissionsList[i] == nullptr)
     {    
        emptyIndex = i; //for adding the message later    
        continue;  
     } 
    //message in current transmissionslist   
    else if(msg->getIdentifier() == currentTransmissionsList[i]->getIdentifier())
    {           
       throw cRuntimeError("Received signalStart message but message already in current transmissions, aborting");    
       endSimulation();    
    }  
    else
    {
       numberOfTransmissions++; //There should at most be one transmission at a time         }     } 
       EV << "numberOfTransmissions: " << numberOfTransmissions << endl;
       if(emptyIndex >=0)
       {
          //add message to currentTransmissionsList   
          currentTransmissionsList[emptyIndex] = msg;  
          numberOfTransmissions++;
        }
        else
        {
           //list is full, should never reach this  
           throw cRuntimeError("currentTransmissionsList full, aborting"); 
           endSimulation();   
        } 
 
        if(numberOfTransmissions > 1)
        {
           double normalRecvPower = 0; 
           double dBmRecvPower = -100; //set this to low number so if no transmissions the following statements wont detect one     
           int i;     
           for(i = 0;i<Nnodes ;i++ )
           {         
              if(currentTransmissionsList[i] != nullptr)
              {
                 normalRecvPower = normalRecvPower + dBm2mW(calcReceivedPowerDBM(currentTransmissionsList[i]));   
              }    
            }
            if(normalRecvPower != 0)
            {
               dBmRecvPower = mW2dBm(normalRecvPower); 
            }      
         
           //Collision detected, see if recv power is above threshold, if so set flag to true 
           if(dBmRecvPower > csThreshDBm)
           {
              for(i = 0;i<Nnodes ;i++ )
              {                 
                 //possible for multiple transmissions to be on the channel but no collision (hidden terminal)  
                 if(currentTransmissionsList[i] != nullptr)
                 {
                    currentTransmissionsList[i]->setCollidedFlag(true);  
                 }  
               }
            }
        }
     
        //no need to delete msg here as it is stored in currentTransmissionsList 
    }

  double Transceiver::calcReceivedPowerDBM(SignalStart* SNRsignal)
  {
     //calculate euclidean distance between this node and the sender node 
     double euclideanDistance = sqrt(pow((SNRsignal->getPositionX()- (double) getParentModule()->par("nodeXPosition")),2)   
                                     + pow((SNRsignal->getPositionY()- (double) getParentModule()>par("nodeYPosition")),2));
     EV << "euclideanDistance(m): " << euclideanDistance << endl; 
     //path loss calculation   
     double pathLossExponent = getParentModule()->getParentModule()>par("pathLossExponent");  
     double pathLoss; //dB   
     
     if(euclideanDistance > 1)
     {
        pathLoss = 10*log10(pow(euclideanDistance,pathLossExponent));
     }
     else
     {
        pathLoss = 10*log10(1);  
     }
     EV << "pathLoss (dB): " << pathLoss << endl;     //convert to DB domain 
 
     //pathLossDB and DBm incompatible
     double receivedPowerDBM = SNRsignal->getTransmitPowerDBm() - pathLoss;   
     EV << "receivedPowerDBM (dBm): " << receivedPowerDBM << endl; 
 
     return receivedPowerDBM; 
  } 
 
   void Transceiver::handleSignalStop(SignalStop *msg)
   {
    
      EV << "SignalStop message received\n"; 
      bool msgFound = false; //see if corresopnding signalStart message in currentTransmissionsList    
      SignalStart* currentSignalStart = nullptr; 
 
      for(int i = 0; i<Nnodes ;i++)
      {
         currentSignalStart= currentTransmissionsList[i];        
         //        currentTransmissionsList[i] = nullptr; 
       
        //dereference the msg in the list 
        if(currentSignalStart != nullptr)
        {       
            if(msg->getIdentifier() == currentSignalStart->getIdentifier())
            {
                msgFound = true; 
 
                if(currentSignalStart->getCollidedFlag() == true)
                {                     //delete SignalStop and SignalStart message, collided messages are always dropped                     EV << "Signal Stop received, collision detected, signalstop message dropped\n";                     packetsLost++;                     cancelAndDelete(currentTransmissionsList[i]);                     currentTransmissionsList[i]= nullptr;                 }                 else{                     double receivedPowerDBM = calcReceivedPowerDBM(currentSignalStart); 
 
                    double signalToNoiseRatio = receivedPowerDBM - (noisePowerDBm + 10*log10(bitRate));                     EV << "signalToNoiseRatio : " << signalToNoiseRatio << endl; 
 
                    double bitErrorRate = erfc(sqrt(2*pow(10,(signalToNoiseRatio/10))));                     EV << "bitErrorRate (Berror): " << bitErrorRate << endl; 
 
                    int msgSize = currentSignalStart->getMacMsg()->getApplMsg()>getMsgSize() * 8;                     double packetErrorRate = 1 - pow((1 - bitErrorRate),msgSize);                     EV << "Packet Error Rate :" << packetErrorRate << endl;                     double u = ((double) rand() / (RAND_MAX)); 
 
                    if(u<packetErrorRate){                         //drop message                         EV << "packet exceeds bit error rate, dropping\n"; 
 
                        //TODO log packet dropped                     }                     else{                         //extract out MAC packet and put in transmissionindication                         EV << "Sending transmission indication message\n";                         TransmissionIndication* newTransmissionIndication = new TransmissionIndication();                         MacMessage* extractedMacMessage = currentSignalStart>getMacMsg()->dup(); //dup message, because deleting currentSignalStart in a few lines                         newTransmissionIndication->setMpkt(extractedMacMessage);                         newTransmissionIndication->setKind(TRANSMISSION_INDICATION);                         send(newTransmissionIndication, "tx2MacOut");                         //TODO log packet received                     }                     EV << "Removing SignalStart from currentTransmissionsList where ID = " << currentSignalStart->getIdentifier() << endl; 

                    cancelAndDelete(currentSignalStart); //remove message also sets currentTransmissionsList[i] to nullptr                     currentTransmissionsList[i] = nullptr;                 }             }         }     } 
 
    if(!msgFound){         throw cRuntimeError("Signal stop message received but no corresponding signalStart message in TransmissionsList. Aborting.");         endSimulation();     } 
 
    delete msg; //remove the SignalStop message } 
 
void Transceiver::handleTransmissionRequest(TransmissionRequest* msg){     EV << "TransmissionRequest message received\n"; 
 
    long kind = msg->getKind(); //set context pointer to HandeTransmissionRequestState 
 
    if(kind == TRANSMISSION_REQUEST){         if(transceiverState == receive){             transceiverState = transmit; 
 
            //Wait for a time specified by the TurnaroundTime             msg->setKind(TRANSMISSION_REQUEST_STATE_1); //set context pointer to turnAroundState             scheduleAt(simTime()+turnAroundTime, msg); //Send the message to itself         }         else{ //respond with message of type TransmissionConfirm to the MAC with the field status set to statusBusy             EV <<"Transmission Request received but in the transmit state, sending statusBusy" << endl;             TransmissionConfirm* msgTransmissionConfirm = new TransmissionConfirm();             msgTransmissionConfirm->setStatus(statusBusy);             msgTransmissionConfirm->setKind(TRANSMISSION_CONFIRM);             send(msgTransmissionConfirm, "tx2MacOut"); 
 
            cancelAndDelete(msg);         }     }     else if(kind == TRANSMISSION_REQUEST_STATE_1){         //extract message size, do this here before sending signal start as the receiver might delete the msg before extracting the length         //msgSize is bits, getMsgSize() is bytes         int msgSize = msg->getMacMsg()->getApplMsg()->getMsgSize() * 8;         double msgDelay = (double) msgSize/ (double) bitRate;         EV << "Extracted appmsg of size (bits) " << msgSize  << "\nSneding signal stop in "<< msgDelay << "seconds"<< endl; 
 
        //signal start of transmission by sending message SignalStart to the channel         SignalStart* msgSignalStart = new SignalStart();         msgSignalStart->setTransmitPowerDBm(txPowerDBm);         msgSignalStart->setPositionX(getParentModule()->par("nodeXPosition"));         msgSignalStart->setPositionY(getParentModule()->par("nodeYPosition"));         msgSignalStart->setIdentifier(getParentModule()->par("nodeIdentifier"));         msgSignalStart->setCollidedFlag(false);         msgSignalStart->setKind(SIGNAL_START);         msgSignalStart->setMacMsg(msg->getMacMsg());         statNumSentPackets++;         send(msgSignalStart, "tx2ChanOut"); 

      //start creation of signal stop         SignalStop* msgSignalStop = new SignalStop(); //signal stop contains no feilds         msgSignalStop->setKind(BITRATE_WAIT);         msgSignalStop->setIdentifier(getParentModule()->par("nodeIdentifier")); 
 
        scheduleAt(simTime() + msgDelay, msgSignalStop); //This message will trigger TRANSMISSION_REQUEST_STATE_2         cancelAndDelete(msg); 
 
    }     else if(kind == TRANSMISSION_REQUEST_STATE_2){         transceiverState = receive;         TransmissionConfirm* msgTransmissionConfirm = new TransmissionConfirm();         msgTransmissionConfirm->setKind(TRANSMISSION_CONFIRM);         msgTransmissionConfirm->setStatus(statusOK);         send(msgTransmissionConfirm, "tx2MacOut"); 
 
        //finally delete the message         cancelAndDelete(msg);     } } 
 
//Calculates the current signal power (in dBm) observed on the channel. //Then waits csTime parameter //Sends back CSResponse with busy channel field which is True when the current signal power observed in the first step exceeds the value //of the paramter csThreshDBm otherwise false. // https://stackoverflow.com/questions/4089726/how-do-i-cast-a-parent-class-as-thechild-class void Transceiver::handleCSRequest(CSRequest *msg){ 
 
    EV << "CSRequest message received\n";     CSResponse* macResponse = new CSResponse(); //create message     bool SignalPresent = false;     double normalRecvPower = 0;     double dBmRecvPower = -1000;     //traverse currentTransmissionsList     for(int i = 0;i<Nnodes; i++){         SignalStart* curSignalStart = currentTransmissionsList[i];         if(curSignalStart != nullptr){             SignalPresent = true;             normalRecvPower = normalRecvPower + dBm2mW(calcReceivedPowerDBM(curSignalStart));         }     }     macResponse->setBusyChannel(false);     if(SignalPresent){         dBmRecvPower = mW2dBm(normalRecvPower);         EV << "Traversed current transmissions, calculated in dBm:" << dBmRecvPower << endl << "Threshold = " << csThreshDBm << endl;         if(dBmRecvPower >= csThreshDBm){             EV << "dBmRecvPower >= csThreshDBm" << endl;             macResponse->setBusyChannel(true);         }     }     else{         EV << "Traversed current transmissions, no signals present" << endl;     } 
 
    //if transceiver in the transmit state, tell MAC its busy     if(transceiverState == transmit){         EV << "Transceiver in transmit state" << endl; 
 macResponse->setBusyChannel(true);     } 
 
    macResponse->setKind(CS_RESPONSE);     EV << "Sending CS_RESPONSE with BusyChannel: " << macResponse->getBusyChannel() << endl;     EV << "CS_RESPOSE will arrive at T= " << simTime() + csTime << endl;     //wait for a time corresponding to the csTime parameter     scheduleAt(simTime()+csTime, macResponse);     //    sendDelayed(macResponse, simTime() + csTime, "tx2MacOut"); 
 
    delete msg; }
