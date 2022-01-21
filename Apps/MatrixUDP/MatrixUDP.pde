/**
 * (./) udp.pde - how to use UDP library as unicast connection
 * (cc) 2006, Cousot stephane for The Atelier Hypermedia
 * (->) http://hypermedia.loeil.org/processing/
 *
 * Create a communication between Processing<->Pure Data @ http://puredata.info/
 * This program also requires to run a small program on Pd to exchange data
 * (hum!!! for a complete experimentation), you can find the related Pd patch
 * at http://hypermedia.loeil.org/processing/udp.pd
 *
 * -- note that all Pd input/output messages are completed with the characters
 * ";\n". Don't refer to this notation for a normal use. --
 */

// import UDP library
import hypermedia.net.*;
import java.util.Arrays;


UDP udp;  // define the UDP object

String[] areaPos01 = {};
String[] areaPos02 = {};
String[] areaPos03 = {};
String[] areaPos04 = {};


void setup() {

  size(1320, 500);

  // create a new datagram connection on port 6000
  // and wait for incomming message
  udp = new UDP( this, 15800 );
  //udp.log( true );     // <-- printout the connection activity
  udp.listen( true );
}

//process events
void draw() {
  background(255);

  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 23; j++) {

      float x = 35*j + 50;
      float y = 35*i + 50;
      int  index = j + i * 23;


      if (areaPos01.length > 0) {

        int marker = int(areaPos01[index]);
        setColor(marker);

        rect(x, y, 30, 30);

        fill(255);
        text(marker+" ", x + 7, y+10);
        text(index+" ", x + 7, y+24);
      }
    }
  }

  if (areaPos02.length > 0) {
    int marker = int(areaPos02[0]);
    setColor(marker);
    float x =  950;
    float y =  50;

    rect(x, y, 30, 30);
    int index =0;

    fill(255);
    text(marker+" ", x + 7, y+10);
    text(index+" ", x + 7, y+24);
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 5; j++) {

      float x = 35*j + 950;
      float y = 35*i + 150;
      int  index = j + i * 5;

      if (areaPos03.length > 0) {
        int marker = int(areaPos03[index]);
        setColor(marker);

        rect(x, y, 30, 30);

        fill(255);
        text(marker+" ", x + 7, y+10);
        text(index+" ", x + 7, y+24);
      }
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 5; j++) {

      float x = 35*j + 950;
      float y = 35*i + 350;
      int  index = j + i * 5;

      if (areaPos04.length > 0) {
        int marker = int(areaPos04[index]);
        setColor(marker);

        rect(x, y, 30, 30);

        fill(255);
        text(marker+" ", x + 7, y+10);
        text(index+" ", x + 7, y+24);
      }
    }
  }
}

void setColor(int id) {
  if (id == -1) {
    fill(220, 200, 90);
  } else if (id  == 17) {
    fill(150, 50, 50);
  } else if (id  == 38) {
    fill(50, 150, 150);
  } else if (id  == 29) {
    fill(50, 200, 50);
  } else if (id  == 36) {
    fill(50, 0, 50);
  } else {
    fill(0, 150, 250);
  }
}

/**
 * on key pressed event:
 * send the current key value over the network
 */
void keyPressed() {

  String message  = str( key );  // the message to send
  String ip       = "localhost";  // the remote IP address
  int port        = 6100;    // the destination port

  // formats the message for Pd
  message = message+";\n";
  // send the message
  udp.send( message, ip, port );
}

/**
 * To perform any action on datagram reception, you need to implement this
 * handler in your code. This method will be automatically called by the UDP
 * object each time he receive a nonnull message.
 * By default, this method have just one argument (the received message as
 * byte[] array), but in addition, two arguments (representing in order the
 * sender IP address and his port) can be set like below.
 */
// void receive( byte[] data ) {       // <-- default handler
void receive( byte[] data, String ip, int port ) {  // <-- extended handler


  // get the "real" message =
  // forget the ";\n" at the end <-- !!! only for a communication with Pd !!!
  data = subset(data, 0, data.length-1);
  String [] message = new String( data ).split(" ");
  if (message.length>0) {

    if (message[0].equals( "i1")) {
      areaPos01 = Arrays.copyOfRange(message, 1, message.length);
      println( "receive: \""+areaPos01[0]+"\" from "+ip+" on port "+port+" "+areaPos01.length );
    } else if (message[0].equals("i2")) {
      areaPos02 = Arrays.copyOfRange(message, 1, message.length);
      println( "receive: \""+areaPos02[0]+"\" from "+ip+" on port "+port+" "+areaPos02.length );
    } else if (message[0].equals("i3")) {
      areaPos03 = Arrays.copyOfRange(message, 1, message.length);
      println( "receive: \""+areaPos03[0]+"\" from "+ip+" on port "+port+" "+areaPos03.length );
    } else if (message[0].equals("i4")) {
      areaPos04 = Arrays.copyOfRange(message, 1, message.length);
      println( "receive: \""+areaPos04[0]+"\" from "+ip+" on port "+port+" "+areaPos04.length );
    }
  }


 
}
