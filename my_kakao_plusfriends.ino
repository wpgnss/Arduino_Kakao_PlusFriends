
#include <SPI.h>
#include <Ethernet.h>
#include "kakao.h"


#define BUFSIZE 255

/* my Server config*/
byte mac[] = { 0x00, 0x08, 0xDC, 0x1C, 0xAB, 0x99 };
IPAddress ip(192, 168, 1, 222);
EthernetServer server(3000);

/* kakao config */
kakao kakao;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.print(F("[Init ethernet.. ]  "));
  // start the Ethernet connection and the server:
  
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.println(Ethernet.localIP());
}

void loop(){
 
  char clientline[BUFSIZE];
  int index = 0;
  int count = 0;
  int readCount = 0;
  int headerIndex = 0;
  
  EthernetClient userClient = server.available();
  
  if(userClient){
    String rxbuf;
    index = 0;

    while(userClient.connected()){
      while(count = userClient.available()){
          if( count > BUFSIZE - 1 ) readCount = BUFSIZE - 1;
          else readCount = count;
          userClient.read(clientline, readCount);
          rxbuf  = String( rxbuf + clientline );
          memset(clientline, 0, sizeof(clientline));
      }
    
      userClient.flush();
      
      if( rxbuf.indexOf("GET /keyboard") > -1 ){
        headerIndex = rxbuf.indexOf("\r\n\r\n");
        rxbuf.remove(0, headerIndex + 4);

        kakao.keyboard(&userClient, rxbuf);
      }
      else if( rxbuf.indexOf("POST /message") > -1 ){
        headerIndex = rxbuf.indexOf("\r\n\r\n");
        rxbuf.remove(0, headerIndex + 4);

        kakao.get_message(&userClient, rxbuf);
      }
      else if( rxbuf.indexOf("POST /thingplug/post") > -1 ){
        headerIndex = rxbuf.indexOf("\r\n\r\n");
        rxbuf.remove(0, headerIndex + 4);
        
        kakao.tp_subscription(&userClient, rxbuf);
      }
      else if( rxbuf.indexOf("GET /test?") > -1 ){
        kakao.tp_dummydata(&userClient, rxbuf);
      }
      else{
        Serial.println("404 NULL");
        userClient.println("HTTP/1.1 404 NOT FOUND\r\n");
      }
     
      rxbuf = "";
      
      delay(1);
      // close the connection
      userClient.stop();
      while(userClient.status() != 0){
        delay(5);
      }
    }
  }
}


