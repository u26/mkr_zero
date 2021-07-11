/* 
 *  MKR ZERO + ETH Seield
 *  
 *  
 *  Ethernet Library
 * https://www.arduino.cc/en/Reference/Ethernet
 * 
 * OSC Library
 * 
 * @copyright mknod.jp
 * 
 */


#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include <OSCMessage.h>
#include <OSCBundle.h>


//----------------
// Ethernet 
//----------------
EthernetUDP Udp_tx;
EthernetUDP Udp_rx;

IPAddress ip(192, 168, 11, 110);
IPAddress ip_dest(192, 168, 11, 3);

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };

unsigned int port_tx = 12345;      // local port to listen on
unsigned int port_rx = 12346;      // local port to listen on


char rx_msg[32] = {0};

void setup() {

  Serial.begin(115200);

  //Ethernet.init(5); //MKR ETH Shield
  
  Ethernet.begin(mac,ip);
  Udp_rx.begin(port_rx);

}

void loop() {

  checkLink();

//  //the message wants an OSC address as first argument
//  OSCMessage msg("/analog/0");
//  msg.add((int32_t)analogRead(0));
//  
//  Udp_tx.beginPacket(ip_dest, port_tx);
//  msg.send(Udp_tx);
//  Udp_tx.endPacket();
//  msg.empty();


  int size;
  OSCBundle bundleIN;

  if( (size = Udp_rx.parsePacket()) > 0)
   {
//    Serial.println( "received" );  
     while(size--){
       bundleIN.fill(Udp_rx.read());
     }
//     if(!bundleIN.hasError()){
        bundleIN.route("/ispace", ispace_callback);
//     }else{
//        Serial.println( "err" );  
//     }
   }
}

void ispace_callback(OSCMessage &msg,  int addressOffset){

//  Serial.println( "ispace_callback" );

  msg.getString(0, &rx_msg[0]);
  Serial.println( rx_msg );
}


#define LINK_ON  1
#define LINK_OFF 0
#define LINK_UNKOWN -1

int checkLink(){

  auto link = Ethernet.linkStatus();
  int ret = LINK_UNKOWN;
  
  switch (link) {
    
    case LinkON:
     ret = LINK_ON;
      break;
      
    case LinkOFF:
      ret = LINK_OFF;
      break;
      
    case Unknown:
    default:
     ret = LINK_UNKOWN;
     break;
  }
  
  return ret;
}
