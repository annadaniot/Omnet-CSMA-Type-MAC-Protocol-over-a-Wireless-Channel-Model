module ReceiverNode 
{     
      parameters: int nodeXPosition;
                  int nodeYPosition;  
                  int numTransmitters;         
                  bool receiver=true;      
                  @display("bgb=195,369");    
      gates:         
              input in;    
              output out;     
      submodules:     
              transceiver: Transceiver {    
                    @display("p=78,294"); 
              }        
              mac: MAC {       
                    @display("p=78,146");
              }        
       packetSink: PacketSink {             @display("p=78,31");         }     connections:       mac.mac2TxOut --> transceiver.tx2MacIn;       transceiver.tx2ChanOut --> out;       in --> transceiver.tx2ChanIn;       transceiver.tx2MacOut -->mac.mac2TxIn;       mac.mac2PGoSOut --> packetSink.in; }
