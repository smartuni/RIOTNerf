![RIOTNerfLogo](https://github.com/smartuni/RIOTNerf/blob/master/DOC/RiotNerf_LOGO.png)
###A distributed lasergun game for multiple players with lots of fun

## The Idea in detail

The fundamental concept is a distributed multiplayer game, consisting of two 
guns and two associated target stations, each running on an IoT.

Originally the Idea was to use actual nerfguns, currently project is running with laserguns, actually providing great 
results.

## Component overview
* Target application running on RIOT  
* Gun application running on RIOT  
* node.js server running on a Raspberry Pi (raspbian os)  

## Getting started

## Working with this git repository

To clone this git repository use

# git clone --recursive https://github.com/smartuni/RIOTNerf.git

This will download a copy of the entire git repository including the RIOT-OS submodule
to the current working directory. The current working directory can be printed using
pwd


To update your local files of the RIOTNerf project use
# git pull origin master

Pushing your local changes to the remote should be done as follows:

Use 

# git status

to see the files that were modified locally.
If the list shows any files that you don't want to push to the remote, 
such as binary files please edit the .gitignore file so that the files in question will be ignored.

Then add your local changes using

# git add *

Afterwards create the commit using

# git commit -m "YOURMESSAGEHERE"

replace YOURMESSAGEHERE with text that indicates what changes you had made.

Finnaly push to the remote using 

# git push origin master


## Setting up the hardware and electronics

To set up you will need the following equipment as listed below:

- 4x samr21-xpro
- 1x Raspberry Pi + Transceiver
- 4x Microservo SG90
- 2x Servo Arm for SG90
- 2x 5mW Red Dot Laser Diode Module
- 2x NPN Transistor
- 2x isl29125 RGB Sensor
- 4x USB Powerbank
- 1x Pinboard
- 2x Piece of Wood: LxBxH 3x3x1cm with 6mm Drill Hole
- 2x Piece of Wood: LxBxH 10x10x1cm with Fitting for isl29125
- 2x Triangular Pillar for Targets
- 8x Spacer
- Wiring
- Screws


TODO: need simple scheme of set up, ie.: which wires go to which pins etc., specific detail on the transistor circuit
TODO: need HW and parts list 

Assemble two samr-x21pro boards for the two laser guns as follows
TODO.....

Assemble two somethingsomething sensors and soonandsoforth for the two targets as follows
TODO.....

Put these wires here and put those over there, ...

TODO:
Raspberry pi setup


## Building the project

To build the project 
build the application that will run on a laser gun
using the Makefile of the coaptest application by executing the following command

# make all term flash BOARD=samr-x21pro

To build the target application run the following command in the target folder

# make all term flash BOARD=samr-x21pro

In order to remote control the laser guns you will have to build the WebServer application as follows:
TODO.....


## Controlling the laser guns

The laser guns can be remote controlled from the web interface supplied by the Raspberry pi WebServer application.
You can either use the arrow keys on your keyboard or enter numbers into the web interface presented in your browser.

## Contributors
* Andreas - Communication IoT side  
* Jerom   - Server side  
* Michael - Server side  
* Phillip - IoT development  
* Darjush - IoT development  
* Martin  - IoT development, HW Setup  
