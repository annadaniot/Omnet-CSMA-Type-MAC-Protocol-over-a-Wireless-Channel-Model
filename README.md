# Omnet-CSMA-Type-MAC-Protocol-over-a-Wireless-Channel-Model


# Project Description

In this project we used OMNet++ to investigate the performance of a simple CSMA type medium access control protocol over a wireless channel model. An overview over the considered system is shown in the system setup diagram (system setup .jpg). 

In the system, it has a variable number of transmitter nodes, which sends data packets to a single reciever node over a common and shared wireless channel. A transmitter node consists of the following components:
    1. The packet generator module generates application layer messages with some (stochastic) interarrival time distribution that can be        chosen in the conﬁguration ﬁle. These messages are sent to the MAC module of the same station.
    2. The MAC (medium access control) module accepts messages to transmit from its higher layers, buﬀers these in a queue of bounded size        (excess messages are to be dropped) and executes a CSMA-type medium access control protocol to transmit these messages over the            channel. 
       To do this, it interacts with the local transceiver for transmission and reception of packets and for carrier-sense operations. In        the other direction, any packets being received from the lower layers are eventually sent to the higher layers after minor       
       processing.
    3. The transceiver module actually transmits packets on the channel and helps the MAC with carrier-sensing. Furthermore, it receives  
       packets from the channel and determines whether these packets are correctly received – if not, it will drop the received packet.



