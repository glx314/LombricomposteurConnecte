# LoRaSender

Small piece of code to put in a "The Things Uno" to transform it into
a LoRaWAN modem. It can communicate with a computer through the USB port 
receiving requests and forwarding downlinks from the network (to be implemented).

It is processing inputs from the USB serial port formatted like this:
`<base64 encoded data>\n`

It decodes the message into a binary buffer, which is then interpreted 
using the lorasender.proto probuf prototype.

The SEND request takes the binary buffer called "data" and transmits it
on the LoRaWAN network. 

(TODO: more requests to add : STATUS, ...)

Dependencies to be installed:
* Google [protocol buffer](https://developers.google.com/protocol-buffers) framework
* [Nanopb](https://jpa.kapsi.fi/nanopb/) : Protocol buffer generator for arduino
* To use the makefile, please check that the path in the file protoc-nano points to your installation of Nanopb; fix it locally if needed.