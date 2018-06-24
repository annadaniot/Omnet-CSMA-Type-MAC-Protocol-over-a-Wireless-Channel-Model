/** Author: Anna */
simple PacketGenerator 
{     
  parameters:  double iatDistribution = default(10);         
               int messageSize=64;       
               int seqno = default(0);       
  gates:      
     //   input in;         
     output out;       
 } 
 
