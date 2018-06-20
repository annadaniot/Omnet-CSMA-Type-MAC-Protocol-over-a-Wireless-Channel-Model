/*Author: Anna
* AppMessage should contain at least the following ﬁelds:
* 1. timeStamp: simulation time at which the message has been generated.
* 2. senderId: is the node identiﬁcation of the TransmitterNode sending this message – this has to be obtained from the parent module 
*   (which should be a TransmitterNode compound module).
* 3. sequenceNumber: the current value of the seqno variable is copied into this ﬁeld and afterwards seqno is incremented.
* 4. msgSize: indicates the total message size in bytes, to be taken from the messageSize module parameter.
*/

message AppMessage 
{     
  simtime_t timeStamp; //sim time     
  int senderId;     
  int sequenceNumber;     
  int msgSize; 
}
