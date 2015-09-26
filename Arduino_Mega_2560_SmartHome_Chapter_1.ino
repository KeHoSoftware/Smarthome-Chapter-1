/****************************************************************************
**                                                                         **
** Name:        SmartHome_Chapter_1                                        **
** Author:      Achim Kern - KeHo Software                                 **
** Date:        01.05.2015                                                 **
** Licence:     Freeware                                                   **
** Function:    Main Program                                               **
**                                                                         **
** Notes:       based on idea from star trek                               **
**                                                                         **
** History:                                                                **
**                                                                         **
** 1.00        - 27.04.2015 - initial release                              **
** 1.01        - 01.05.2015 - code clearing                                **
**                                                                         **
*****************************************************************************/
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xBA, 0x88 };
// The IP address will be dependent on your local network:
IPAddress ip(192,168,178,112);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP)
EthernetServer server(80);

// Initialize the setup
 void setup() 
{ 
  // Open serial communications and wait for port to open
  Serial.begin(9600);
  while (!Serial) 
  {
    // wait for serial port to connect. Needed for Leonardo only
  }
  // start the Ethernet connection and the server
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("arduino server is at ");
  Serial.println(Ethernet.localIP()); 
}

/*-------------------------------------------------------------------------------*/
/* Function void loop()                                                          */
/*                                                                               */
/* TASK    : arduino is waiting for clients - poll every 50ms                    */
/* UPADTE  : 01.05.2015 - code clearing                                          */
/*-------------------------------------------------------------------------------*/ 
void loop() 
{
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) 
  {
    Serial.println("new client");
    // Process request
    process(client);
    // Close connection and free resources
    client.stop();
  }    
  // Poll every 50ms
  delay(50); 
}

/*-------------------------------------------------------------------------------*/
/* Function void process(EthernetClient client)                                  */
/*                                                                               */
/* TASK    : arduino receives a command from a client                            */
/* UPDATE  : 01.05.2015 - code clearing                                          */
/*-------------------------------------------------------------------------------*/
void process(EthernetClient client) 
{
  // read the command
  Serial.println("reading command");
  String command = client.readStringUntil('/');
  Serial.println(command);
  command = client.readStringUntil('/');
  Serial.println(command);
  command = client.readStringUntil('/');
  Serial.println(command);
  
  // is "version" command?
  if (command == "version") 
  { versionCommand(client); }    
  
  // is "digital" command?
  if (command == "digital") 
  { digitalCommand(client); }  
}

/*-------------------------------------------------------------------------------*/
/* Function void versionCommand(EthernetClient client)                           */
/*                                                                               */
/* TASK    : arduino receives "version" command from a client                    */
/* UPDATE  : 01.05.2015 - code clearing                                          */
/*-------------------------------------------------------------------------------*/
void versionCommand(EthernetClient client) 
{
  Serial.println("command version detected");
  String arduino_version = client.readStringUntil(' ');
  // is "version" command?
  Serial.println(arduino_version);  
  // verifiying the connection Hollywood SmartHome application to arduino gateway
  if (arduino_version == "arduino") 
   { 
     // send a standard http response header
     sendheader(client); 
     // send the actual version nr
     client.print(F("Arduino-Gateway-V-1.01"));
   }  
}

/*-------------------------------------------------------------------------------*/
/* Function digitalCommand(EthernetClient client)                                */
/*                                                                               */
/* TASK    : arduino receives "digital" command from a client                    */
/* UPDATE  : 01.05.2015 - code clearing                                          */
/*-------------------------------------------------------------------------------*/
void digitalCommand(EthernetClient client) 
{
  int pin, value;

  Serial.println("command digital detected");
  // Read pin number
  pin = client.parseInt();
  Serial.println(pin);
  // set pin mode output
  pinMode(pin,OUTPUT);

  // If the next character is a '/' it means we have an URL
  // with a value like: "/digital/13/1"
  if (client.read() == '/') {
    value = client.parseInt();
    digitalWrite(pin, value);
  } 
    else 
  {
    value = digitalRead(pin);
  }
  // Send feedback to client
  // send a standard http response header
  sendheader(client);   
  client.print(F("Pin D"));
  client.print(pin);
  client.print(F(" set to "));
  client.println(value);  
}

/*-------------------------------------------------------------------------------*/
/* Function sendheader(EthernetClient client)                                    */
/*                                                                               */
/* TASK    : this function sends a standard http response header to the client   */
/* UPDATE  : 01.05.2015                                                          */
/*-------------------------------------------------------------------------------*/
void sendheader(EthernetClient client) 
{
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  
  // the connection will be closed after completion of the response
  client.println("Refresh: 30");  
  // refresh the page automatically every 30 sec
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 }

