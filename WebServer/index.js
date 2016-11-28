///////////////////////////////////////////////////////////////////
//////////////////////////////TODO/////////////////////////////////
//TODO: IP selber finden? Broadcastadresse?
// 		Chat auf Website
//		Slider für Eingabe -> -90 bis +90
//      Feste IPs für Laser 1 und Laser 2
//		Zielscheibe 1
//      



var main = function() {
	run();
}

if (require.main == module) {
	main();
}



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

//Die IPs bleiben jeweils gleich!!
var coapLaser1 = 'fd1d:8d5c:12a5:0:585a:6a65:70bd:247e'
var coapLaser2 = 'fd1d:8d5c:12a5:0:5841:1644:2407:f2c2'
var coapPath = 'ff02::1%lowpan0';

var userCount = 0;
var collectiveHorizontal = 0;
var collectiveVertical = 0;







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

	socket.on('disconnect', function(){
		//nicht kleiner als 0 werden!
		userCount--;
	});

	userCount++;
	console.log(userCount);


  //When User clicks Button on Webpage, 'chat message' is emitted, message is reversed and sent back
 	socket.on('chat message', function(msg){
	    console.log('message: '+ msg);
	    var answer = 'links';
	    var payload = '1';

	    if( msg == 'links'){ payload='0';}

	    coapPut('/periph/servohstep', payload);
	    
	    //hier URL herausziehen
        //console.log('request ' + JSON.stringify(request.url));

        //console.log('payload ' + payload);

	    //io.emit('chat message', 'definitely not '+answer);


		
	});

 	//TODO: what happens, when user puts nothin in textfield
 	//TODO: function nicht declarieren, sondern ausführen
	socket.on('servosnsteps', function(msg){
		console.log(msg);
		//servosNSteps(msg);
		var array = msg.split(' ');
		collectiveHorizontal += parseInt(array[0]);
		collectiveVertical += parseInt(array[1]);
	});

	socket.on('laser', function(msg){
		console.log(msg);
		setLaser(msg);
	});

	socket.on('keyPressed', function(msg){
		console.log('KeyPressed');
		if(msg == 'left'){
			servosNSteps('-1 0');
		} else if(msg == 'right'){
			servosNSteps('1 0');
		} else if(msg == 'up'){
			servosNSteps('0 1');
		} else if(msg == 'down'){
			servosNSteps('0 -1');
		} else if(msg == 'space'){

		}
	});
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});








////////////////////////////////////////////////////////////////
//////////////////////////COAP-SERVER///////////////////////////

coapServer.on('request', function(req, res){
  console.log('COAP: Request came in');
  req.pipe(process.stdout);
  res.end();
  /*var method = req.method;
  console.log(method);
  var url = req.url;
  console.log(url);
  //get PUT value and print on console
  if(method == 'PUT'){
    var value = url.split('=');
    console.log(value[1]);
  }
  res.end('Hello ' + req.url.split('/')[1] + '\n');
  */
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
	//console.log(coapPath+path);
	var request = coap.request({
		method: 'PUT',
		host: hostName,
		pathname: path,
		confirmable: 'false'
	});
	request.write(payload);
	console.log('request ' + JSON.stringify(request.url));
	console.log('payload ' + payload);
	request.end();
}

function servosNSteps(stepsHV){
	coapPut(coapPath, '/periph/servosnstep', stepsHV);
}

//Mehrmals schicken, wegen Packetloss
function setLaser(laser){
	coapPut(coapPath, '/periph/laser', laser);
}









////////////////////////////////////////////////////////////////
//////////////////////////GAME-LOGIC////////////////////////////


function run(){
	setInterval(function(){
		calculateNewServosNSteps();
		round();
	}, 10000);
}

function calculateNewServosNSteps(){
	var count = userCount;
	var horizontal = collectiveHorizontal;
	var vertical = collectiveVertical;
	if(count > 0){
		resetCollectives();

		var servosNStepsString = (horizontal/count) + ' ' + (vertical/count);
		servosNSteps(servosNStepsString);
	}
}

function round(){
	fireLaser(1);
	fireLaser(2);

}

function fireLaser(num){
	var laserPath = '/periph/laser';
	if(num == 1){
		coapPut(coapLaser1, laserPath, '1');
	} else {
		coapPut(coapLaser2, laserPath, '1');
	}
}

function resetCollectives(){
		collectiveHorizontal = 0;
		collectiveVertical = 0;
	}
