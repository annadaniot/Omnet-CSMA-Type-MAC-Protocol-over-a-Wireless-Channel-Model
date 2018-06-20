/* Author: Anna */
simple Channel 
{   
  parameters:       
    int numTransmitters;          
  
  gates:       
    input in[numTransmitters+1];     //gates for Transmitters and  Receiver       
    output out[numTransmitters+1];    //Transmitters + Receiver          
 }
