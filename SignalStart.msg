cplusplus {{ #include "MacMessage_m.h" typedef MacMessage *MacMessagePtr; //typedef othertype }} class noncobject MacMessagePtr; 
 
message SignalStart {     double transmitPowerDBm; //copy of the transceivers txPowerDBm value     double positionX; //copy of the transmitter or receivers nodeXPosition (stored in transceiver)     double positionY; //copy of the transmitter or receivers nodeYPosition (stored in transceiver)     int identifier; //contains the value of the nodeIdentifier parameter of the surrounding compound module 
21 
 
    bool collidedFlag = false; //initialized to FALSE     MacMessagePtr macMsg; } 
