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
        nodeXPosition = 0;             nodeYPosition = 0;         }         transmitterNode[numTransmitters]: TransmitterNode {             numTransmitters = numTransmitters;             nodeIdentifier = index;             nodeXPosition = R;             nodeYPosition = 0;         }     connections:         receiverNode.out --> chan.in[numTransmitters];         chan.out[numTransmitters] --> receiverNode.in;         for i=0..numTransmitters-1 {             transmitterNode[i].out --> chan.in[i];             chan.out[i] --> transmitterNode[i].in;         } } 
//simulation 12.1.2 //Vary number, M, of transmitternodes, fix R=20 and vary M {2,4,6,8,10,12,14,16,18,20} //set iatDistribution in the packetgenerator to have a deterministic distribution with a period of 20ms //For each transmitter, record the amount of packets dropped by the mac due to buffer overflows //Packets can be lost in the channel, ie, they are not properly received at the receiver node, due to collisions? //record the amount lost in this scenario //graph M vs packet loss in mac and packet loss in channel //network WSNet //{ //    parameters: //        int numTransmitters @prompt("Enter the number of transmitters: ") = default(5); //vary this 2->20 in steps of 2 //        double pathLossExponent = default(4); //unitless //        int R @prompt("Enter the distance between transmitters and the receiver") = default(20); //this stays fixed //        double pi = 3.1415; //        @display("bgb=493,347"); //    submodules: 
  chan: Channel { //            numTransmitters = numTransmitters; //        } //        receiverNode: ReceiverNode { //            numTransmitters = numTransmitters; //            nodeXPosition = 0; //            nodeYPosition = 0; //        } //        transmitterNode[numTransmitters]: TransmitterNode { //             //             //            numTransmitters = numTransmitters; //            nodeIdentifier = index; //            nodeXPosition = cos(2*pi*index/numTransmitters)*R; //            nodeYPosition = sin(2*pi*index/numTransmitters)*R; //             //             //        } //    connections: //        receiverNode.out --> chan.in[numTransmitters]; //        chan.out[numTransmitters] --> receiverNode.in; //        for i=0..numTransmitters-1 { //            transmitterNode[i].out --> chan.in[i]; //            chan.out[i] --> transmitterNode[i].in; //        } //} 
 
 
 
 
