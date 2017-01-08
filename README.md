WS 2016/17 - RIOT im Internet of Things - Bachelor Project of Computer Science and Electrical Engineering

![RIOTNerfLogo](https://github.com/smartuni/RIOTNerf/blob/master/DOC/RiotNerf_LOGO.png)
###A distributed lasergun game for multiple players with lots of fun

# RIOTNerf

# The Idea in detail

The fundamental concept is a distributed multiplayer game, consisting of two 
guns and two associated target stations, each running on an IoT.

Originally the Idea was to use actual nerfguns, currently project is running with laserguns, actually providing great 
results.

# Component overview
* Target application running on RIOT  
* Gun application running on RIOT  
* node.js server running on a Raspberry Pi (raspbian os)  

# Getting started

# Getting started with RIOT-OS
As RIOTNerf is based on RIOT-OS (https://github.com/RIOT-OS/)
you will have to set up RIOT-OS correctly before you will be able to work with RIOTNerf.
To do so it is recommended to follow the steps provided in the RIOT-OS tutorial, which is to be found 
here: https://github.com/RIOT-OS/Tutorials

We experienced the most success in developing with RIOT-OS by running RIOT-OS without a VM (virtual machine)
on a system running a distrbution of GNU/Linux, in particular running Linux Mint.

# Working with this git repository

To clone this git repository use

## git clone --recursive https://github.com/smartuni/RIOTNerf.git

This will download a copy of the entire git repository including the RIOT-OS submodule
to the current working directory. The current working directory can be printed using
pwd


To update your local files of the RIOTNerf project use
## git pull origin master

Pushing your local changes to the remote should be done as follows:

Use 

## git status

to see the files that were modified locally.
If the list shows any files that you don't want to push to the remote, 
such as binary files please edit the .gitignore file so that the files in question will be ignored.

Then add your local changes using

## git add *

Afterwards create the commit using

## git commit -m "YOURMESSAGEHERE"

replace YOURMESSAGEHERE with text that indicates what changes you had made.

Finally push to the remote using 

## git push origin master


# Setting up the hardware and electronics

To set up you will need the following equipment as listed below:

- 4x samr21-xpro
- 1x Raspberry Pi + Transceiver
- 4x Microservo SG90
- 2x Servo Arm for SG90
- 2x 5mW Red Dot Laser Diode Module
- 2x NPN Transistor
- 2x 1kOhm Resistor
- 2x isl29125 RGB Sensor
- 4x USB Powerbank
- 1x Pinboard
- 2x Piece of Wood: LxBxH 3x3x1cm with 6mm Drill Hole
- 2x Piece of Wood: LxBxH 10x10x1cm with Fitting for isl29125
- 2x Triangular Pillar for Targets
- 8x Spacer
- Wiring
- Screws

Assemble the samr21-xpro boards for each gun and each target as follows
![SchematicRIOTNerf](https://github.com/smartuni/RIOTNerf/blob/master/DOC/ScematicRIOTNerf.png)

# Raspberry Pi Setup

Für Raspberry Pi 1B haben wir im Repository (Ordner "RaspPi") alle benötigten Daten.
Zieht den Inhalt des Ordner auf eine SD-Karte und schon müsste der Pi bereit sein.

Für Raspberry Pi >1B muss zur Sicherheit alles neu gemacht werden.
Damit beginnt ihr mit: https://github.com/RIOT-Makers/wpan-raspbian/wiki

Punkt 4 (New Linux Kernels for the Pi) könnt ihr hier bei auslassen.

Zu guter letzt der Hinweis zu den iwpan channels. In unserem Projekt senden und empfangen wir auf iwpan channel 26. Mit "iwpan phy phy0 set channel 0 26" stellt ihr den Channel auf 26, bzw. auf andere Channels. Achtet darauf auch die IoT Geräte anzupassen.

Danach müsst ihr nur noch per Konsole node.js installieren. Hierzu geht ihr auf: https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions und auf http://socket.io/download/ bekommt ihr socket.io.

Nun braucht ihr nur noch den Ordner "WebServer" in das Home Verzeichnis zu kopieren und könnt den Web- und CoapServer starten (node index.js)!

# Building the project

To build the project 
build the application that will run on a laser gun
using the Makefile of the coaptest application by executing the following command

## make all term flash BOARD=samr21-xpro

To build the target application run the following command in the target folder

## make all term flash BOARD=samr21-xpro

In order to remote control the laser guns you will have to build the WebServer application as follows:
TODO.....


# Controlling the laser guns

The laser guns can be remote controlled from the web interface supplied by the Raspberry pi WebServer application.
You can either use the arrow keys on your keyboard or enter numbers into the web interface presented in your browser.
TODO: perhaps add a screenshot of the GUI presented here?

# Contributors
* Andreas - Communication IoT side  
* Jerom   - Server side  
* Michael - Server side  
* Phillip - IoT development  
* Darjush - IoT development  
* Martin  - IoT development, HW Setup  
