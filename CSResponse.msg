//message to send back to the MAC module 

message CSResponse 
{     
  //True when the current signal power observed in the first step exceeds the value     
  //of the paramter csThreshDBm otherwise false.     
  bool busyChannel; 
} 
