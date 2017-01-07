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

## Get started

Before getting started, make sure to have all the following equipment ready and wired up:  
TODO: need simple scheme of set up, ie.: which wires go to which pins etc., specific detail on the transistor circuit  
TODO: need HW and parts list  

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
