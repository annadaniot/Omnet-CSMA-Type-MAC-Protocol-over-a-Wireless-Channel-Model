module TransmitterNode 
{     
   parameters:         
       int nodeIdentifier;         
       int nodeXPosition;  
       int nodeYPosition;
       int numTransmitters;
       bool receiver = false;  
       @display("bgb=181,359"); 
   gates:         
       input in;    
       output out;   
   submodules:      
       transceiver: Transceiver 
       {             
               @display("p=73,290"); 
       }    
       packetGenerator: PacketGenerator 
       {
               @display("p=73,37");    
       }
       mac: MAC 
       { 
            @display("p=73,163");  
       }   
       connections:     
             packetGenerator.out --> mac.mac2PGoSIn; 
             mac.mac2TxOut --> transceiver.tx2MacIn; 
             transceiver.tx2ChanOut --> out; 
             in --> transceiver.tx2ChanIn;  
             transceiver.tx2MacOut --> mac.mac2TxIn; 
 } 
