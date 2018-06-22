/** Author: Anna */

simple MAC 
{     
    parameters:   int bufferSize= default(5);  
                  int maxBackoffs = default(5);         
                  double backoffDistribution @unit(s) = default(3ms); 
                        
                  //TODO check if when getting this function it returns seconds or ms, the exponential function needs to know      
                        
    string logFileName = "macPacketLoss";    
                        
    gates:        input mac2TxIn;         
                  output mac2TxOut;         
                  input mac2PGoSIn @loose; //loose means the sim will run if the gate isnt connected         
                  output mac2PGoSOut @loose; 
}
