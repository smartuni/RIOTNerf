# RIOTNerf
WS 2016/17 - RIOT im Internet of Things - Bachelor Project of Computer Science and Electrical Engineering

## CoAP
### PUT
- Servo Horizontal single step "/periph/servohstep" values: 0 = left step; 1 = right step; ex: "1"
- Servo Vertical single step "/periph/servovstep" values: 0 = left step; 1 = right step; ex "0"
- Servo Horizontal n steps "/periph/servohnstep" values: -1500 to 0 = left; 0 to 1500 = right; ex: "1337"
- Servo Vertical n steps "/periph/servovnstep" values: -1500 to 0 = left; 0 to 1500 = right; ex: "1337"
- Servo Horizontal angle "/periph/servohangle" values: -90.00 to 90.00 = angle; ex: "13.23"
- Servo Vertical angle "/periph/servovangle" values: -90.00 to 90.00 = angle; ex: "13.23"
- Servos n steps "/periph/servosnstep" values: -1500 to 0 = left; 0 to 1500 = right; ex: "-1230 456"
- Servos angle "/periph/servosangle" values: -90.00 to 90.00 = angle; ex: "23.45 -12.60"
- Laser set "/periph/laser" values: 0 = unset; 1 = set; ex: "1"
### GET
- Wellknown Core "/.wellknown/core" return: structure data
- Servos "/periph/servos" return: servo states in json
