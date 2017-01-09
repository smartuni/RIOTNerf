///////////////////////////////////////////////////////////////////
//////////////////////////////TODO/////////////////////////////////
//TODO: IP selber finden? Broadcastadresse?
// 		Chat auf Website
//		Slider für Eingabe -> -90 bis +90
//      Feste IPs für Laser 1 und Laser 2
//		Zielscheibe 1, Zielscheibe 2


//Mainfunktion, die sich um das periodische abfeuern der Laser kuemmert. Bald deprecated
/*
var main = function() {
	run();
}

if (require.main == module) {
	main();
}
*/



///////////////////////////////////////////////////////////////////
///////////////////////////IMPORTS/////////////////////////////////

//HTTP-Server
var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
app.use(express.static(__dirname + '/public'));

//COAP-Server
var coap = require('coap');
var coapServer = coap.createServer({type: 'udp6'});








///////////////////////////////////////////////////////////////////
////////////////////////////GLOBAL-FIELDS//////////////////////////

//Die IPs fuer die IOT-Geraete bleiben auch bei Neustart gleich!!
var coapLaser1 = 'fd1d:8d5c:12a5:0:585a:6a65:70bd:247e%lowpan0';
var coapLaser2 = 'fd1d:8d5c:12a5:0:5841:1644:2407:f2c2%lowpan0';
		

//die Ips der Zielscheiben
var target1 = 'fe80::5855:5f70:108b:c07e'
var target2 = 'fe80::5841:174a:589e:f29e'	
var target1hit = false;
var target2hit = false;								

//Multicast-Adresse
var coapPath = 'ff02::1%lowpan0';
//direkte Ansprache der Laser momentan (noch) nicht moeglich
/*
coapLaser1 = coapPath;
coapLaser2 = coapPath;
*/

//Globale Variablen fuer Game-Logic...
var userCount = 0;

var input1Count = 0;
var input2Count = 0;
var collectiveHorizontal1 = 0;
var collectiveVertical1 = 0;
var collectiveHorizontal2 = 0;
var collectiveVertical2 = 0;


var laserState = [0,0];







////////////////////////////////////////////////////////////////
//////////////////////////HTTP-SERVER///////////////////////////


//hier 8888?
coapServer.listen(function(){
  console.log('COAP listening ');
});

//provide Webpage
app.get('/', function(req, res){
  //res.sendFile(__dirname + '/index.html');
  res.sendFile('index.html');
});

//Web-Functionality
io.on('connection', function(socket){

	//Bei Verbindungsende wird User-Count dekrementiert
	socket.on('disconnect', function(){
		//nicht kleiner als 0 werden!
		userCount--;
	});

	//Usercount erhoeht sich bei jeder neuen Verbindung
	userCount++;
	console.log(userCount);


  //When User clicks Button on Webpage, 'chat message' is emitted, message is reversed and sent back
 	socket.on('chat message', function(msg){
	    console.log('message: '+ msg);
	    var answer = 'links';
	    var payload = '1';

	    if( msg == 'links'){ payload='0';}

	    coapPut('/periph/servohstep', payload);
		
	});

 	//TODO: what happens, when user puts nothin in textfield
 	//TODO: function refaktorisieren, sodass SPOC gewaehrleistet
	socket.on('servosnsteps1', function(msg){
		input1Count++;
		console.log(msg);
		servosNSteps(coapLaser1, msg);
		var array = msg.split(' ');
		collectiveHorizontal1 += parseInt(array[0]);
		collectiveVertical1 += parseInt(array[1]);
	});

	socket.on('servosnsteps2', function(msg){
		input2Count++;
		console.log(msg);
		servosNSteps(coapLaser2, msg);
		var array = msg.split(' ');
		collectiveHorizontal2 += parseInt(array[0]);
		collectiveVertical2 += parseInt(array[1]);
	});


	//TODO: hier servosNSteps von Player 2 - copy/paste

	socket.on('laser', function(msg){
		console.log(msg);
		setLaser(msg);
	});

	//Steuerung ueber Pfeiltasten
	socket.on('keyPressed1', function(msg){
		//console.log('KeyPressed');
		if(msg == 'left'){
			console.log('left pressed');
			servosNSteps(coapLaser1, '-1 0');
		} else if(msg == 'right'){
			console.log('right pressed');
			servosNSteps(coapLaser1, '1 0');
		} else if(msg == 'up'){
			console.log('up pressed');
			servosNSteps(coapLaser1, '0 1');
		} else if(msg == 'down'){
			console.log('down pressed');
			servosNSteps(coapLaser1, '0 -1');
		} else if(msg == 'fire'){
			console.log('fire2 pressed');
			fireLaser(1);
		}
	});

	//TODO: WASD
	socket.on('keyPressed2', function(msg){
		//console.log('KeyPressed2');
		if(msg == 'left'){
			console.log('left2 pressed');
			servosNSteps(coapLaser2, '-1 0');
		} else if(msg == 'right'){
			console.log('right2 pressed');
			servosNSteps(coapLaser2, '1 0');
		} else if(msg == 'up'){
			console.log('up2 pressed');
			servosNSteps(coapLaser2, '0 1');
		} else if(msg == 'down'){
			console.log('down2 pressed');
			servosNSteps(coapLaser2, '0 -1');
		} else if(msg == 'fire'){
			console.log('fire2 pressed');
			fireLaser(2);
		}
	});
	

	coapServer.on('request', function(req, res){
	  console.log('COAP: Request came in');


	  // message.rsinfo, könnte Infos zur richtigen IP geben

	  var payload = req.payload;
	  var payloadString = payload.toString();
	  console.log(payloadString)

	  var info = req.rsinfo;
	  var address = info.address;
	  console.log(address.toString());
	  if((address.toString() === target1) && !target1hit){
	  	console.log("Zielscheibe1 wurde getroffen!");
	  	if(!target1hit){
	  		io.emit('LaserTrigger','Zielscheibe1 wurde getroffen!');
	  		target1hit = true;
	  	}
	  }

	  if((address.toString() === target2) && !target2hit){
	  	console.log("Zielscheibe2 wurde getroffen!");
	  	if(!target2hit){
	  		io.emit('LaserTrigger','Zielscheibe2 wurde getroffen!');
	  		target2hit = true;
	  	}
	  }

	  res.end();
	});

});

//Starten des Servers
http.listen(3000, function(){
  console.log('listening on *:3000');
});










////////////////////////////////////////////////////////////////
//////////////////////////REQUEST-HELPER////////////////////////

//path: suffix for coap://[fd1d:8d5c:12a5:0:585a:6a65:70bd:247e]/ 
//returns: request
/*function coapGet(path){
	return coap.request(coapPath+''+path);
}*/

function coapPut(hostName, path, payload){
	//var request = coapGet(path);
	console.log(hostName+path);
	var request = coap.request({
		method: 'PUT',
		//bis wir ip direkt ansprechen koennen, hier an Multicast-Adresse schicken
		host: hostName,
		pathname: path,
		confirmable: 'false'
	});
	request.write(payload);
	console.log('request ' + JSON.stringify(request.url));
	console.log('payload ' + payload);
	request.end();
}

function servosNSteps(host, stepsHV){
	coapPut(host, '/periph/servosnstep', stepsHV);
}

/*
//Mehrmals schicken, wegen Packetloss
function setLaser(laser){
	coapPut(coapPath, '/periph/laser', laser);
}
*/









////////////////////////////////////////////////////////////////
//////////////////////////GAME-LOGIC////////////////////////////


function run(){

	//Alle n Sekunden werden erst Servos bewegt (beide entsprechend der für sie eingegebenen Werte) 
	//und dann beide Laser nacheinander abgefeuert, und auf Treffer "gehorcht"
	/*
	setInterval(function(){
		calculateNewServosNSteps();
		round();
	}, 10000);
	*/
}

function calculateNewServosNSteps(){
	var count1 = input1Count;
	var horizontal1 = collectiveHorizontal1;
	var vertical1 = collectiveVertical1;

	var count2 = input2Count;
	var horizontal2 = collectiveHorizontal2;
	var vertical2 = collectiveVertical2;

	resetCollectives();
	if(count1 > 0){
		

		var servosNStepsString1 = (Math.round(horizontal1/count1)) + ' ' + (Math.round(vertical1/count1));
		servosNSteps(coapLaser1, servosNStepsString1);
	}
	if(count2 > 0){
		var servosNStepsString2 = (Math.round(horizontal2/count2)) + ' ' + (Math.round(vertical2/count2));
		servosNSteps(coapLaser2, servosNStepsString2);
	}
}

function round(){
	fireLaser(1);
	fireLaser(2);

}

function fireLaser(num){
	var laserPath = '/periph/laser';
	if(num == 1){
		switchLaserState(0);
		coapPut(coapLaser1, laserPath, ''+ laserState[0]);
	} else {
		switchLaserState(1);
		coapPut(coapLaser2, laserPath, ''+ laserState[1]);
	}
}

function switchLaserState(laser){
	var currLaserState = laserState[laser];
	if(currLaserState == 0){
		laserState[laser] = 1;
	} else{
		laserState[laser] = 0;
	}
}

function resetCollectives(){
		collectiveHorizontal1 = 0;
		collectiveVertical1 = 0;
		collectiveHorizontal2 = 0;
		collectiveVertical2 = 0;
	}
