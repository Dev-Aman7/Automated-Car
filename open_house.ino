#include<SPI.h>
#include<Ethernet.h>

int ac=3;
int car=4;
int buzzer=5;
int pir=8;
int engine=9;

byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};
// enter the ip address according to your system. (open cmd ,type ipconfig,check your ip, change the last tuple)
EthernetServer server(12345);
IPAddress ip(192,168,43,150);

String request;



void setup(){
  Ethernet.begin(mac,ip);
  pinMode(ac,OUTPUT);
  pinMode(car,OUTPUT);
  pinMode(9,INPUT);
  pinMode(5,OUTPUT);

  
  Serial.begin(9600);
  Serial.println(Ethernet.localIP());
}

void loop(){
  int motion=digitalRead(pir);
  int p=digitalRead(9);
  

  EthernetClient client = server.available();
  if(client)
  {
    Serial.println("Client is trying to connect......");
    while(client.connected())
    {
      if(client.available())
      {
        char c = client.read();
        request.concat(c);
        if(c=='\n'){
        Serial.print(request);
        if(request.indexOf("ON")>0){
          digitalWrite(ac,HIGH);
          Serial.println("ON");
          }

          else if(request.indexOf("OFF")>0){
            digitalWrite(ac,LOW);
            Serial.println("OFF");
            }
            if(request.indexOf("car_on")>0 && p==1)
            {
              digitalWrite(car,HIGH);
              digitalWrite(5,LOW);
            }
            else if(request.indexOf("car_off")>0 || p==0)
            {
              digitalWrite(car,LOW);
            }
            if(request.indexOf("car_off")>0 && p==1)
            {
              digitalWrite(5,HIGH);
            }
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Refresh: 2");  // refresh the page automatically every 2 sec
        client.println();

        client.println("<!doctype html>");
        client.println("<html>");
        client.println("<head><title>Automated Car System</title><h1 style = ' text-align:center'>Automated car system</h1>");
       
        client.print("<!DOCTYPE HTML><html><body style ='background-color:powderblue;' ><a href ='/?AC_ON'><p style = ' text-align:center'>AC ON</p></a><br><a href ='/?AC_OFF'><p style = ' text-align:center'>AC  OFF</P></a><br>" );
         client.print("<a href ='/?car_on'><p style = ' text-align:center'>CAR ON</p></a><br><a href ='/?car_off'><p style = ' text-align:center'>CAR OFF</p></a><br></body></html>" );
            
            if (motion==1)
             {
              Serial.println("motion detected");
              client.println("Motion Detected");
              
              }

             if(p==1)
             {
              Serial.println("Somebody starting engine");
             
              client.println("Somebody starting engine");
              
             }
             

           
            int tem=analogRead(A0);
            //int ldr=analogRead(A0);
            client.println("Temperature ");
            
            client.print(" is ");
            client.print(tem*0.041);
            int t=tem*0.041;
            
            client.println("<br />");
            //client.print("luminisity is ");
            //client.print(ldr);
            client.println("<br />");
              if(t>35)
            {

              digitalWrite(buzzer,HIGH);            
            }
          
        client.println("</html>");
        client.stop();
        request="";
        
      }
      }
     }
      
  }
  
  
}
