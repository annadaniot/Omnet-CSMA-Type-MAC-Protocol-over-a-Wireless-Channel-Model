/* Author: Anna */

#include "MAC.h" 
  
Define_Module(MAC); 
 
void MAC::initialize() 
{     
  EV_DEBUG << getParentModule()->getName() << ": initialising MAC module\n";    //EV_DEBUG for LOGLEVEL_DEBUG     
  bufferSize=par("bufferSize");     
  maxBackoffs=par("maxBackoffs") ;     
  backoffDistribution=par("backoffDistribution") ;     
  isProcessing=false; 
 
  /** logging, only log if transmitter and nodeid = 0, all mac modules should have the 
    *  same amount of losses, maybe +- 5 if the sim stops at a weird time  
  */
  
  if(strcmp(getParentModule()->getName(),"transmitterNode")  == 0 && 
   (int) getParentModule()->par("nodeIdentifier") == 0)
   {         
      macLogFileName = generate_log_file_name(par("logFileName").stringValue(), true);         
      EV << "MAC Using log file: " << macLogFileName << endl;     
    }     
    
    numRecvFromGen = 0;     
    numLostPacketsBuffer = 0;     
    numLostPacketsBackOff = 0;     
    numPacketsSentToTrans = 0; 
 } 
 
void MAC::finish() 
{     
  EV<<"Exit MAC."<<endl;     
  if(strcmp(getParentModule()->getName(),"transmitterNode")  == 0 &&             
      (int) getParentModule()->par("nodeIdentifier") == 0)
      {         
          EV << "Writing to MAC log file: " << macLogFileName << endl;         
          std::ofstream file;         
          file.open(macLogFileName, std::ios::app);   //this appends to end of file         
          file << (int) getParentModule()->par("numTransmitters") << ", "<<numRecvFromGen<< ", "
               << numLostPacketsBuffer << ", " << numLostPacketsBackOff << endl;         
          file.close();         
          EV << "Logged " << (int) getParentModule()->par("numTransmitters") << ", "<<numRecvFromGen<< ", "
               << numLostPacketsBuffer << ", " << numLostPacketsBackOff << endl;     
       } 
 } 
 
 void MAC::handleMessage(cMessage *msg) 
 {     
    EV << "MAC::handleMessage" << endl; 
 
    if(msg->getKind()==APP_MSG)  //if message is from packet generator     
    {         
        EV<<"Received message from packet generator."<<endl;         
        AppMessage* appMsg = check_and_cast<AppMessage *>(msg); 
        handleApplicationMessage(appMsg);     
    }     
    
    else if(msg->getKind()==MAC_MSG)  //if message is from transceiver     
    {         
        EV<<"Received MAC message."<<endl;         
        MacMessage* macMsg = check_and_cast<MacMessage *>(msg);         
        handleMACMessage(macMsg);     
    }     
    
    else if(msg->getKind()==CS_RESPONSE)     
    {         
        EV<<"Received Carrier Sense Response message from transceiver."<<endl;         
        CSResponse* msgCSRes = check_and_cast<CSResponse *>(msg);         
        //        CSResponse *msgCSRes=new CSResponse();         
        handleCarrierResponse(msgCSRes);     
     }     
     
     else if(msg->getKind()==CS_REQUEST)     
     {         
        EV<<"Received Carrier Sense Request selfmessage from MAC."<<endl;         
        CSRequest* csReqmsg = check_and_cast<CSRequest *>(msg);         
        handleCarrierRequest(csReqmsg);     
     }  
     
     else if(msg->getKind()==TRANSMISSION_CONFIRM)     
     {         
        EV<<"Received Transmission Confirm message from transceiver."<<endl;         
        TransmissionConfirm* transCmsg = check_and_cast<TransmissionConfirm *>(msg); 
         //        TransmissionConfirm *transCmsg=new TransmissionConfirm();         
        handleTransmissionConfirm(transCmsg);     
      }     
      
      else if (msg->getKind()==TRANSMISSION_INDICATION)     
      {         
          EV<<"Received Transmission Indication message from transceiver."<<endl;         
          TransmissionIndication* transImsg = check_and_cast<TransmissionIndication *>(msg);         
          //        TransmissionIndication *transImsg=new TransmissionIndication();         
          handleTransmissionIndication(transImsg);     
      }     
      else     
      {         
          EV<<"MAC is dropping message "<<"of kind "<< (msg->getKind()) <<endl;         
          delete(msg);     
       } 
   } 
 
void MAC::handleApplicationMessage(AppMessage *msg) 
{     
/**If buffer is empty, then      
* extract the oldest AppMessage from macBuffer and encapsulate it into a message mmsg of type MacMessage      
* Initialize a local variable called backoffCOunter to zero      
* While backoffCounter is smaller than maxBackOffs,perform the ff:      
* 1. Perform crrier sense      
* 2. If carrier send was idle, transmit mmsg, set backoffCOunter to zero and go back to first step.      
* 3. If carrier is busy, increment backoffcounter and wait for random time to transmit      
* 4. Go back to step 1.      
* */     

EV<<"MAC::handleApplicationMessage"<<endl;     
numRecvFromGen++;     

if(macBuffer.size() < static_cast<size_t>(bufferSize)) //buffer messages in queue bounded size     
{         
    EV<<"macBuffer.size() < (bufferSize) \n";         
    macBuffer.push(msg);  //insert 
 
    if(macBuffer.size()==1 && isProcessing == false)  //do FIFO         
    { 
        AppMessage *amsg=macBuffer.front();  //access the first element             
        macBuffer.pop();    //removes the first element 
 
        mmsg=new MacMessage();            //generate MacMessage 
        mmsg->setApplMsg(amsg);             
        mmsg->setKind(MAC_MSG);           //encapsulate into MacMessge 
 
        backoffCounter=0;             
        isProcessing = true; 
 
        EV<< "sending carrier sense request to transceiver" << endl;             
        
        CSRequest* msgCSRequest = new CSRequest();             //send carrier sense request to transceiver             
        msgCSRequest->setKind(CS_REQUEST);             
        send(msgCSRequest,"mac2TxOut");                        //send out CSRequest to transceiver         
      }     
   }     
   
   else            //excess message are to be dropped     
   {         
        
        numLostPacketsBuffer++; 
        EV<<"MAC is dropping packets. Buffer is full. Num of drops:"<<numLostPacketsBuffer<<endl; 
        //        out<<((AppMessage *)msg)->getSenderId()<<","<<((AppMessage *)msg)>getSequenceNumber()<<endl; 
        //log this event 
 
        delete msg;     
    } 
 
    EV<<"Exit MAC::handleApplicationMessage"<<endl; 
} 
 
void MAC::handleMACMessage(MacMessage *msg) 
{     
    EV<<"MAC::handleMACMessage"<<endl;     
    AppMessage *applMsg=msg->getApplMsg()->dup();        //copy the message     
    send(applMsg,"mac2PGoSOut");                         //send to packet sink 
 
    delete msg->getApplMsg();     
    delete msg;                  //delete the message 
 } 
 
void MAC::handleCarrierResponse(CSResponse *msg) 
{     
    if(msg->getBusyChannel()==false)         //if channel is not busy     
    {         
        EV<<"CSResponse BusyChannel False" << endl; 
 
        backoffCounter=0;                    //No longer doing backoffs so reset         
        isProcessing=false; 
 
        //Create TransmissionRequest and send to Transceiver         
        TransmissionRequest *transReq=new TransmissionRequest();         
        transReq->setKind(TRANSMISSION_REQUEST);         
        numPacketsSentToTrans++;         
        MacMessage* newMmsg = new MacMessage();         
        newMmsg->setApplMsg(mmsg->getApplMsg()->dup());         
        transReq->setMacMsg(newMmsg);         
        delete mmsg->getApplMsg();         
        delete mmsg;         
        send(transReq, "mac2TxOut");          //send transmission request to transceiver         
        EV<< "Sent TransmissionRequest to transceiver" << endl; 
 
        //Last mmsg was successful, get next one         
        if(macBuffer.size()>0)         
        {             
              AppMessage *amsg=macBuffer.front();            //1st element to AppMessage pointer             
              macBuffer.pop();             
              mmsg=new MacMessage();             
              mmsg->setApplMsg(amsg);             
              mmsg->setKind(MAC_MSG);             
              EV << "Set mmsg to first element in FIFO" << endl; 
              isProcessing=true;              //in process of doing carrier response 
 
 
             //create CSRequest and send             
             EV<< "sending carrier sense request to transceiver" << endl;             
             CSRequest* newCSRequest = new CSRequest();             
             newCSRequest->setKind(CS_REQUEST);             
             send(newCSRequest,"mac2TxOut");             //send Carrier sense request to transceiver         
          } 
     } 
      
     else         //channel is busy     
     {         
          EV<<"Channel is busy. "<< endl;         
          backoffCounter++;         
          EV<<"backoffCounter: " << backoffCounter<<endl;         
          if(backoffCounter < maxBackoffs)         
          {             
              isProcessing=true; 
              CSRequest* newCSRequest = new CSRequest();             
              newCSRequest->setKind(CS_REQUEST);             
              scheduleAt(simTime()+exponential(backoffDistribution),newCSRequest);    //schedule a request for carrier sense to itself            
              EV<<"Sending self a csReq self message after T=backoffDistribution" << endl;         
           }         
           else         
           {             
              EV<< "Backoff counter limit reached" << endl;             
              if(macBuffer.size()>0)             
              {                 
                    EV<<"macBuffer.size()>0, Reset backoffCounter to zero, dropped message \n";  
                    backoffCounter=0;     
                    AppMessage *amsg=macBuffer.front();       
                    macBuffer.pop(); 
 
                    EV << "Deleting mmsg with seqno: " << mmsg->getApplMsg()>getSequenceNumber() 
                       << "that failed to be transmitted due to back off counter limit" << endl;                 
                    numLostPacketsBackOff++;  
                    delete mmsg; 
      
                    mmsg=new MacMessage();                 
                    mmsg->setApplMsg((AppMessage *)amsg);       
                    mmsg->setKind(MAC_MSG); 
 
                    CSRequest *newCSR = new CSRequest(); 
                    newCSR->setKind(CS_REQUEST);    
                    send(newCSR,"mac2TxOut"); 
 
                    isProcessing=true;             
                }   
                else           //FiFO empty             
                {                
                    isProcessing = false;             
                }         
             }     
           }     
           delete msg; 
        } 
 
       //MAC module sends self messages of type CSRequest after waiting backoff time 
       void MAC::handleCarrierRequest(CSRequest *msg) 
       {     
            EV<<"Got self CSRequest, sending to Transceiver";     
            msg->setKind(CS_REQUEST);     
            send(msg,"mac2TxOut"); 
        } 
        
        void MAC::handleTransmissionConfirm(TransmissionConfirm* msg) 
        {     
            /**  If the transceiver is in the transmit state 
              *  when TransmissionRequest arrives, it responds with a message of type TransmissionConfirm     
              *  in which the field status is set to statusBusy, the Transceiver does no further action.     
              *  Transceiver sends statusOK when successful transmission after transition from sending to recv state. 
            **/
 
           EV<<"handleTransmissionConfirm"<<endl;     
           TransmissionConfirm* transConf= (TransmissionConfirm* )msg;     
           if (transConf->getStatus()== statusBusy)        //statusBusy from TRansceiver     
           {         
                /**TODO perhaps some logging*/         
                EV<<"Transceiver is busy."<<endl;     
            }     
            else if(transConf->getStatus()== statusOK) //statusOkay from TRansceiver     
            {         
                EV<<"Status is okay. Transmission is successful."<<endl;     
            }     
            delete msg;         //delete the message 
         } 
 
        void MAC::handleTransmissionIndication(TransmissionIndication* msg) 
        {     
          //transmitter node should delete the message, receiver should process it     
          if(strcmp(getParentModule()->getName(),"receiverNode") == 0)     
          {         
              EV<<"MAC::handleTransmissionIndication, on receiver node, processing message"; 
      
              if(msg->getMpkt() != nullptr)         
              {             
                    if(msg->getMpkt()->getApplMsg() != nullptr)     
                    {                 
                          AppMessage *applMsg = msg->getMpkt()->getApplMsg()->dup(); 
                          delete msg->getMpkt()->getApplMsg();      
                          applMsg->setKind(APP_MSG);    
                          send(applMsg,"mac2PGoSOut");                 
                          EV<<"Sending application message to packet Sink";   
                     }
               }         
               
               else         
               { 
                    //shoud never reach here           
                    EV << "MAC extracted an applMsg with a nullptr, sending nullptr to packetsink\n";     
               }
           }     
           
           else
           {        
                EV<<"MAC::handleTransmissionIndication, on transmitter node, dropping message"; 
           }     
           
           delete msg->getMpkt();     
           delete msg; 
     }
       
