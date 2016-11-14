//HTTP-Server
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

//TODO: IP selber finden? Broadcastadresse?
// 		Chat auf Website
//		Slider für Eingabe -> -90 bis +90

//COAP-Server
var coap = require('coap');
var coapServer = coap.createServer({type: 'udp6'});

//var coapPath = 'fd1d:8d5c:12a5:0:585a:6a65:70bd:247e'
var coapPath = 'ff02::1%lowpan0';

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

//hier 8888?
coapServer.listen(function(){
  console.log('COAP listening ');
});

//provide Webpage
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});


//Web-Functionality
io.on('connection', function(socket){
  //When User clicks Button on Webpage, 'chat message' is emitted, message is reversed and sent back
 	socket.on('chat message', function(msg){
	    console.log('message: '+ msg);
	    var answer = 'links';
	    var payload = '1';

	    if( msg == 'links'){ payload='0';}

	    var request = coapPut('/periph/servohstep', payload);
	    //hier URL herausziehen
        //console.log('request ' + JSON.stringify(request.url));

        //console.log('payload ' + payload);

	    //io.emit('chat message', 'definitely not '+answer);
		request.end();
	});


 	//TODO: what happens, when user puts nothin in textfield
 	//TODO: function nicht declarieren, sondern ausführen
	socket.on('servosnsteps', function(msg){
		console.log(msg);
		var request = servosNSteps(msg);
		request.end();
	});

	socket.on('laser', function(msg){
		console.log(msg);
		var request = setLaser(msg);
		request.end();
	});

	socket.on('keyPressed', function(msg){
		console.log('KeyPressed');
		if(msg == 'left'){
			var request = servosNSteps('-1 0');
			request.end();
		} else if(msg == 'right'){
			var request = servosNSteps('1 0');
			request.end();
		} else if(msg == 'up'){
			var request = servosNSteps('0 1');
			request.end();
		} else if(msg == 'down'){
			var request = servosNSteps('0 -1');
			request.end();
		}
	});
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});


//path: suffix for coap://[fd1d:8d5c:12a5:0:585a:6a65:70bd:247e]/ 
//returns: request
/*function coapGet(path){
	return coap.request(coapPath+''+path);
}*/

function coapPut(path, payload){
	//var request = coapGet(path);
	console.log(coapPath+path);
	var request = coap.request({
		method: 'PUT',
		host: coapPath,
		pathname: path,
		confirmable: 'false'
	});
	request.write(payload);
	console.log('request ' + JSON.stringify(request.url));
	console.log('payload ' + payload);
	return request;
}

function servosNSteps(stepsHV){
	return coapPut('/periph/servosnstep', stepsHV);
}

//Mehrmals schicken, wegen Packetloss
function setLaser(laser){
	return coapPut('/periph/laser', laser);
}