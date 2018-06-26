simple Transceiver 
{     
      parameters:         //parameters can only be double, int, bool, string and xml     
              volatile double turnAroundTime @unit(s) = default(0.01s);     //seconds
              volatile double txPowerDBm @unit(dBm) = default(0dBm);        //radiated power in dBm^2    
              volatile int bitRate @unit(bps) = default(250000bps);         //specifies the transmission bitrate in bits/s       
              volatile double csThreshDBm @unit(dBm) = default(-50dBm);     //specifies the observed signal power (in dBm) aboce which a carrier-sensing station will indicate the medium as busy.         
              volatile double noisePowerDBm  @unit(dBm) = default(-120dBm); //specifies the nose power (which here we purport to be expressed in units of dBm, which is not entierly correct).       
              volatile double csTime @unit(s) = default(0.000125s);         // Specifies the time required to carry out a carrier sense operation, seconds         int nunTransmitters = default(1);         string TransceiverFile1 = "transceiverLog1"; //for the first set of simulations         string TransceiverFile2 = "transceiverLog2"; //for the second set of simulations, only receiver node has access to this file     gates:        input tx2MacIn;        output tx2MacOut;        input tx2ChanIn;         output tx2ChanOut; } 
