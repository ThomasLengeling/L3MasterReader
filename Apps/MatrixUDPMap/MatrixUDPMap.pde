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
 
import spout.*;
Spout spout;

// import UDP library
import hypermedia.net.*;
import java.util.Arrays;

PImage map;

UDP udp;  // define the UDP object

String[] areaPos01 = {};
String[] areaPos02 = {};
String[] areaPos03 = {};
String[] areaPos04 = {};

PGraphics offScreen;

void setup() {

  size(1280, 720, P3D); //3508,1080 img size

  offScreen = createGraphics(3508, 1080, P3D);
  offScreen.smooth(8);
  smooth(8);
  surface.setLocation(0, 0);
  surface.setResizable(false);

  map = loadImage("map_interaction.png");

  // create a new datagram connection on port 6000
  // and wait for incomming message
  udp = new UDP( this, 15800 );
  //udp.log( true );     // <-- printout the connection activity
  udp.listen( true );
  
  spout = new Spout(this);
   spout.setSenderName("SpoutP");
   
}

//process events
void draw() {



  offScreen.beginDraw();
  offScreen.background(0);
  offScreen.image(map, 0,  0, offScreen.width, offScreen.height);

  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 23; j++) {

      float x = 51.6*j + 621;
      float y = 51.6*i + 154;
      int  index = j + i * 23;


      if (areaPos01.length > 0) {

        int marker = int(areaPos01[index]);
        setColor(marker);

        offScreen.rect(x, y, 44.5, 44.5);

        offScreen.fill(255);
       // offScreen.text(marker+" ", x + 7, y+10);
       // offScreen.text(index+" ", x + 7, y+24);
      }
    }
  }

  if (areaPos02.length > 0) {
    int marker = int(areaPos02[0]);
    setColor(marker);
    float x =  2118;
    float y =  310;

    offScreen.rect(x, y, 43.5, 43.5);
    int index =0;

    offScreen.fill(255);
   // offScreen.text(marker+" ", x + 7, y+10);
   // offScreen.text(index+" ", x + 7, y+24);
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 5; j++) {

      float x = 51.5*j + 1913;
      float y = 51.5*i + 412;
      int  index = j + i * 5;

      if (areaPos03.length > 0) {
        int marker = int(areaPos03[index]);
        setColor(marker);

        offScreen.rect(x, y, 43.5, 43.5);

        offScreen.fill(255);
       // offScreen.text(marker+" ", x + 7, y+10);
       // offScreen.text(index+" ", x + 7, y+24);
      }
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 5; j++) {

      float x = 51.5*j +  2790  ;
      float y = 51.5*i + 104;
      int  index = j + i * 5;

      if (areaPos04.length > 0) {
        int marker = int(areaPos04[index]);
        setColor(marker);

        offScreen.rect(x, y, 43.5, 43.5);

        offScreen.fill(255);
       // offScreen.text(marker+" ", x + 7, y+10);
       // offScreen.text(index+" ", x + 7, y+24);
      }
    }
  }

  offScreen.endDraw();

  image(offScreen, 0, 0, width, height);
  spout.sendTexture(offScreen);
  
}

void setColor(int id) {
  if (id == -1) {
    offScreen.fill(0);
  } else if (id  == 17) {//RS
    offScreen.fill(#ff00ff);
  } else if (id  == 34) {//RM
    offScreen.fill(#b802ff);
  } else if (id  ==13) {//RL
    offScreen.fill(#a200ff);
  } else if (id  == 37) {//OS
    offScreen.fill(#00ffff);
  } else if (id == 33) {//OM
    offScreen.fill(#0099ff);
  } else if (id == 29 ) {//OL
    offScreen.fill(#00ffd5);
  } else if (id == 38) { //parks
    offScreen.fill(0, 80, 80, 200);
  } else {
    offScreen.fill(255, 240, 140, 200);
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
