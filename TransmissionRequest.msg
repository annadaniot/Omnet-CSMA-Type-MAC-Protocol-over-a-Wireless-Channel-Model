cplusplus {{ 
   #include "MacMessage_m.h" 
   typedef MacMessage *MacMessagePtr; //typedef othertype 
}} 

class noncobject MacMessagePtr; 
 
message TransmissionRequest 
{ MacMessagePtr macMsg; }
