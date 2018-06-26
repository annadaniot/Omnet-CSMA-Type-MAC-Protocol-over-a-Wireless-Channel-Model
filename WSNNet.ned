/** Author: Anna */

//simulation 12.1.1 
//one chan, receiver and transmitter, vary R between 1 and 40m in steps of 1m 
//set iatDistribution in the packetgenerator to have a deterministic distribution with a period of 10ms 
// This leads to a fixed number of packets transmitted through the 1000 seconds of sim time 
//For each distance record the number of received packets and record the packet loss rate 
//show packet loss rate versus distance as a graph 

network WSNet 
{     
    parameters:         
        int numTransmitters = default(2);
        // @prompt("Enter the number of transmitters: ") = default(2); //remains costant    
        double pathLossExponent = default(4); //unitless    
        int R ;//@prompt("Enter the distance between transmitters and the receiver") = default(20);   //vary this in python script 
        double pi = 3.1415;    
        @display("bgb=493,347");
    submodules:      
        chan: Channel 
        {      
           numTransmitters = numTransmitters;  
        }
     receiverNode: ReceiverNode 
     {
        numTransmitters = numTransmitters;    
        nodeXPosition = 0;          
        nodeYPosition = 0;  
     }
     
     transmitterNode[numTransmitters]: TransmitterNode
     { 
        numTransmitters = numTransmitters; 
        nodeIdentifier = index;  
        nodeXPosition = R;
        nodeYPosition = 0;  
     } 
     
     connections:         
        receiverNode.out --> chan.in[numTransmitters];    
        chan.out[numTransmitters] --> receiverNode.in;    
        for i=0..numTransmitters-1 
        {
           transmitterNode[i].out --> chan.in[i];  
           chan.out[i] --> transmitterNode[i].in; 
         }
  } 
 
 
 
