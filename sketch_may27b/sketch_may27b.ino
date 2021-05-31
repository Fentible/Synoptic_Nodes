/**
  ******************************************************************************
  * @file    readings_node.ino
  * @author  T. Buckingham
  * @brief   Sketch file for children nodes that record and send readings to the 
  *           user via the bridge [see bridge_node.ino]
  *
  *          The file contains ::
  *           + Functions for handling requests and responding to the bridge
	*						+ Node specific details such as its string name     
  *
  * FOR MORE DETAILS ON LIBRARIES OR EXAMPLES USED PLEASE SEE THE README
  *
  ******************************************************************************
  */

/*-- Includes --*/

#include "painlessMesh.h"   // Mesh network header
#include "namedMesh.h"      // Mesh network with names implemented - see Nodes GitHub version for any changes

#include <ArduinoJson.h>    // JSON parsing and encoding
#include <SD.h>             // SD Storage header
#include <SPI.h>            // Serial Peripheral Interface

/*-- Global Definitions --*/

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

/*-- Global Variables --*/
// Scheduler userScheduler;

namedMesh  mesh;                /* namedMesh network class, used to interface with the network */
String nodeName = "lobitos";                   /* Name for this specific node */

/*-- Prototypes --*/

/** @brief Function used to send stored readings as a string to the bridge node when the user requests
 *  @param Void.
 *  @return Void.
 */
void sendReadings(unsigned char ticket_number); 

/** @brief Basic function used to test the connections between the root during development
 *  @param Void.
 *  @return Void.
 */
void sendMessage() ;              


void sendReadings(unsigned char ticket_number)
{

  /* Read from SD card 
  *  Format of string to be parsed :: 
  *  [ticket_number]:[data{timestamp,reading1,reading2,reading3}:{timestamp,reading1,reading2,reading3}]
  */
  /* File file = open("filename"); // each line will have a timestamp with readings in correct format
  *  String readings = String(ticket_number);
  *  while(file.nextline != EOF) {
  *     readings += file.nextline();
  *  }
  */
  String root = "root";
  String readings = String(ticket_number) + ":" + "1622119135383,9.214,3.45453,1.3424:1622119135383,12.354,3.44753,6.37424";
  mesh.sendSingle(root, readings);

}

void sendMessage() 
{
  String msg = "Hi from node1";
  msg += mesh.getNodeId();
  String to = "root";
  mesh.sendSingle(to, msg);
}

void newConnectionCallback(uint32_t nodeId) // when new node gets connected alert program?
{
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void setup() 
{
  Serial.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP ); 

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );

  mesh.setName(nodeName); 

  mesh.onReceive([](String &from, String &msg) 
  {
    String reply = "hello from lobitos, i got " + msg;
    mesh.sendSingle(from, reply);
    Serial.printf("Received message by name from: %s, %s\n", from.c_str(), msg.c_str());
  });

}

//void receivedCallback( const uint32_t &from, const String &msg ) {
  //String rep = "High from lobitos";
  //mesh.sendSingle(from, rep);
//}

void loop() 
{
  mesh.update();
}
