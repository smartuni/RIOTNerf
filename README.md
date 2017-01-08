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

Before getting started, make sure to have all the following equipment ready and wired up:  
Hardware list:
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

Once all is set up, simply flash the programm, using the makefile in the coap folder with the following command:  

TODO: need command here  

if you simply wish to test the individual IOT components, have a check on the according wiki page

TODO: raspberry pi set up  

## Contributors
* Andreas - Communication IoT side  
* Jerom   - Server side  
* Michael - Server side  
* Phillip - IoT development  
* Darjush - IoT development  
* Martin  - IoT development, HW Setup  
