message TransmissionConfirm {         int status; //TODO use enum (using macros will work fine) } 
 
TransmissionIndication.msg cplusplus {{ #include "MacMessage_m.h" typedef MacMessage *MacMessagePtr; }} class noncobject MacMessagePtr; // // TODO generated message class // message TransmissionIndication {     MacMessagePtr mpkt; } TransmissionRequest.msg cplusplus {{ #include "MacMessage_m.h" typedef MacMessage *MacMessagePtr; //typedef othertype }} class noncobject MacMessagePtr; 
 
message TransmissionRequest {     MacMessagePtr macMsg; } 
