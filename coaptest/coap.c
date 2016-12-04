#include <string.h> /* strlen, strncat, strtok, memset, memcpy */
#include <stdio.h> /* puts, snprintf */
#include <stdlib.h> /* strtol */
#include <stdint.h> /* uint8_t */
#include <coap.h>
#include <periph/gpio.h>
#include "pHAL.h"

/*! \def RN_UNUSED(arg)
 *  \brief macro to mark a parameter as unused
 *  \param arg the parameter to mark as unused
 *
 *  Used to suppress compiler warnings about unused parameters
**/
#define RN_UNUSED(arg)           ((void) (arg))

/*! \def RN_COUNTOF(array)
 *  \brief macro to calculate the count of elements of an array
 *  \param array the array to calculate the amount of elements for
 *  \warning Only pass arrays into the parameter,
 *           not pointers to the 0th element of an array.
 *           Beware of array to pointer decay.
**/
#define RN_COUNTOF(array)	     ((sizeof((array))) / (sizeof(*(array))))

/*! \def MAX_RESPONSE_LEN
 *  \brief The maximumn length of a response in bytes
 *  \see response
 *  
 *  Used by response.
**/
#define MAX_RESPONSE_LEN         (500)

/*! \def CONTENT_TYPE_PLAIN_TEXT
 *  \brief plain text CoAP content
**/
#define CONTENT_TYPE_PLAIN_TEXT  ((coap_content_type_t) 0)

/*! \def CONTENT_TYPE_LINK_FORMAT
 *  \brief link format CoAP content
**/
#define CONTENT_TYPE_LINK_FORMAT ((coap_content_type_t) 40)

/*! \def CONTENT_TYPE_JSON
 *  \brief JSON CoAP content
**/
#define CONTENT_TYPE_JSON        ((coap_content_type_t) 50)

/*! \def VALUE_HANDLE_SIZE
 *  \brief the size in bytes of the value handles
 *
 *  Used to create arrays large enough to hold the textual representation
 *  of a single digit integer value.
**/
#define VALUE_HANDLE_SIZE        (2)

/*! \def TEST_RESPONSE_SIZE
 *  \brief the size of the test responses in bytes.
**/
#define TEST_RESPONSE_SIZE       (20)

/*! \def BASE_DECIMAL
 *  \brief The decimal base for numbers.
 *
 *  Used as the base argument to strtol.
**/
#define BASE_DECIMAL             (10)

/*! \def TOKENIZATION_DELIMITERS 
 *  \brief Delimiters used to tokenize string data.
**/
#define TOKENIZATION_DELIMITERS  " ,;:"

/*! \def STEPS_MIN
 *  \brief The minimum value for steps; used in stepn functions.
**/
#define STEPS_MIN                (-1500)

/*! \def STEPS_MAX
 *  \brief The maximum value for steps; used in stepn functions.
**/
#define STEPS_MAX                (1500)

/*! \brief handle_get_well_known_core TODO: add documentation.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  TODO: add detailed documentation.
**/
static int handle_get_well_known_core(coap_rw_buffer_t* scratch,
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_get_servos TODO: add documentation.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  TODO: add detailed documentation.
**/  
static int handle_get_servos(coap_rw_buffer_t* scratch,
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_put_servoh_step Moves the horizontal servo by one step.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  If the value passed is '1' the horizontal servo moves one step to the right.
 *  If the value passed is '0' the horizontal servo moves one step to the left.
**/
static int handle_put_servoh_step(coap_rw_buffer_t* scratch,
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_put_servov_step Moves the vertical servo by one step.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  If the value passed is '1' the vertical servo moves one step upward.
 *  If the value passed is '0' the vertical servo moves one step downward.
**/
static int handle_put_servov_step(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_put_servoh_nstep Moves the horizontal servo by n steps.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *  \warning the steps passed in must be within STEPS_MIN and STEPS_MAX.
 *
 *  For negative values the horizontal servo moves n steps to the left.
 *  For positive values the horizontal servo moves n steps to the right.
**/
static int handle_put_servoh_nstep(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_put_servov_nstep Moves the vertical servo by n steps.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *  \warning the steps passed in must be within STEPS_MIN and STEPS_MAX
 *
 *  For negative values the vertical servo moves n steps downward.
 *  For positive values the vertical servo moves n steps upward.
**/
static int handle_put_servov_nstep(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_put_servoh_angle TODO: add documentation.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  TODO: add detailed documentation.
**/
static int handle_put_servoh_angle(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_put_servov_angle TODO: add documentation.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  TODO: add detailed documentation.
**/
static int handle_put_servov_angle(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_put_servos_nstep Moves both servos by n steps.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  TODO: add detailed documentation.
**/
static int handle_put_servos_nstep(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_put_servos_angle TODO: add documentation.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  TODO: add detailed documentation.
**/
static int handle_put_servos_angle(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*! \brief handle_put_laser Turns the laser on or off depending on the value passed.
 *  \param scratch TODO: add documentation.
 *  \param inpkt TODO: add documentation.
 *  \param outpkt TODO: add documentation.
 *  \param id_hi TODO: add documentation.
 *  \param id_lo TODO: add documentation.
 *  \return TODO: add documentation.
 *
 *  If a value of '1' is passed the laser is turned on.
 *  If a value of '0' is passed the laser is turned off.
**/
static int handle_put_laser(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

/*!< buffer to hold a response received */
static uint8_t response[MAX_RESPONSE_LEN];

// CoAP PUT [
static const coap_endpoint_path_t path_servo_h_step =  { 2, { "periph", "servohstep" } };
static const coap_endpoint_path_t path_servo_v_step =  { 2, { "periph", "servovstep" } };
static const coap_endpoint_path_t path_servo_h_nstep = { 2, { "periph", "servohnstep" } };
static const coap_endpoint_path_t path_servo_v_nstep = { 2, { "periph", "servovnstep" } };
static const coap_endpoint_path_t path_servo_h_angle = { 2, { "periph", "servohangle" } };
static const coap_endpoint_path_t path_servo_v_angle = { 2, { "periph", "servovangle" } };
static const coap_endpoint_path_t path_servos_nstep =  { 2, { "periph", "servosnstep" } };
static const coap_endpoint_path_t path_servos_angle =  { 2, { "periph", "servosangle" } };
static const coap_endpoint_path_t path_laser =         { 2, { "periph", "laser" } };
// ] CoAP PUT

// CoAP GET [
static const coap_endpoint_path_t path_well_known_core = { 2, { ".wellknown", "core" } };
static const coap_endpoint_path_t path_servos =          { 2, { "periph", "servos" } };
// ] CoAP GET

// https://tools.ietf.org/html/rfc7252 12.3.
// 0  = plain/text
// 40 = link-format
// 50 = json
const coap_endpoint_t endpoints[] = {
    /* CoAP GET*/
    { COAP_METHOD_GET, handle_get_well_known_core, &path_well_known_core, "ct=40" },
    { COAP_METHOD_GET, handle_get_servos,          &path_servos,          "ct=50" },

    /* CoAP PUT */
    { COAP_METHOD_PUT, handle_put_servoh_step,     &path_servo_h_step,    "ct=0" },
    { COAP_METHOD_PUT, handle_put_servov_step,     &path_servo_v_step,    "ct=0" },
    { COAP_METHOD_PUT, handle_put_servoh_nstep,    &path_servo_h_nstep,   "ct=0" },
    { COAP_METHOD_PUT, handle_put_servov_nstep,    &path_servo_v_nstep,   "ct=0" },
    { COAP_METHOD_PUT, handle_put_servoh_angle,    &path_servo_h_angle,   "ct=0" },
    { COAP_METHOD_PUT, handle_put_servov_angle,    &path_servo_v_angle,   "ct=0" },
    { COAP_METHOD_PUT, handle_put_servos_nstep,    &path_servos_nstep,    "ct=0" },
    { COAP_METHOD_PUT, handle_put_servos_angle,    &path_servos_angle,    "ct=0" },
    { COAP_METHOD_PUT, handle_put_laser,           &path_laser,           "ct=0" },

    { (coap_method_t) 0, NULL, NULL, NULL }
};

// Taken from RIOT example
// Edited by "CppPhil"
static int handle_get_well_known_core(coap_rw_buffer_t* scratch,
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    char *rsp = (char *) response;
    /* resetting the content of response message */
    memset(response, 0, RN_COUNTOF(response));
    int len = RN_COUNTOF(response);
    const coap_endpoint_t *ep = endpoints;
    int i;

    len--; // Null-terminated string

    while (NULL != ep->handler) {
        if (NULL == ep->core_attr) {
            ep++;
            continue;
        }

        if (0 < strlen(rsp)) {
            strncat(rsp, ",", len);
            len--;
        }

        strncat(rsp, "<", len);
        len--;

        for (i = 0; i < ep->path->count; i++) {
            strncat(rsp, "/", len);
            len--;

            strncat(rsp, ep->path->elems[i], len);
            len -= strlen(ep->path->elems[i]);
        }

        strncat(rsp, ">;", len);
        len -= 2;

        strncat(rsp, ep->core_attr, len);
        len -= strlen(ep->core_attr);

        ep++;
    } /* END while */

    return coap_make_response(scratch, outpkt, (const uint8_t *) rsp,
                              strlen(rsp), id_hi, id_lo, &inpkt->tok,
                              COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_APPLICATION_LINKFORMAT);
}

static int handle_get_servos(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    static const char handle_get_servos_response[] =
        "[ {\"name\":\"servoH\", \"unis\":2130}, {\"name\":\"servoL\", \"units\":70} ]";

    memcpy(response, handle_get_servos_response,
        sizeof(handle_get_servos_response) - 1);

    return coap_make_response(scratch, outpkt, (const uint8_t *) response,
                              sizeof(handle_get_servos_response) - 1, id_hi, id_lo,
                              &inpkt->tok, COAP_RSPCODE_CONTENT,
                              CONTENT_TYPE_JSON);
}

static int handle_put_servoh_step(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    uint8_t value = inpkt->payload.p[0];

    if (inpkt->payload.len != 1 || value > '1' || value < '0') {
        puts("Error, inpkt->len != 1");
    } else {
        if (value == '1') {
            puts("Servo H step right");
            step_r();
        } else if (value == '0') {
            puts("Servo H step left");
            step_l();
        } else {
            puts("False value for Servo H step");
        }

        char valuehandle[VALUE_HANDLE_SIZE];
        snprintf(valuehandle, VALUE_HANDLE_SIZE, "%u", value);

        char testresponse[TEST_RESPONSE_SIZE];
        strcpy(testresponse, "{ \"ledstatus\":");
        strncat(testresponse, valuehandle, strlen(testresponse));
        strncat(testresponse, " }", strlen(testresponse));

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, 
            len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT,
            CONTENT_TYPE_JSON);
    }

    return -1;
}

static int handle_put_servov_step(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    uint8_t value = inpkt->payload.p[0];

    if (inpkt->payload.len != 1 || value > '1' || value < '0') {
        puts("Error, inpkt->len != 1");
    } else {
        if (value == '1') {
            puts("Servo V step up");
            step_u();
        } else if (value == '0') {
            puts("Servo V step down");
            step_d();
        } else {
            puts("False value for Servo H step");
        }

        char valuehandle[VALUE_HANDLE_SIZE];
        snprintf(valuehandle, VALUE_HANDLE_SIZE, "%u", value);

        char testresponse[TEST_RESPONSE_SIZE];
        strcpy(testresponse, "{ \"ledstatus\":");
        strncat(testresponse, valuehandle, strlen(testresponse));
        strncat(testresponse, " }", strlen(testresponse));

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, 
            len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, 
            CONTENT_TYPE_JSON);
    }

    return -1;
}

static int handle_put_servoh_nstep(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    static const char delimiters[] = TOKENIZATION_DELIMITERS;
    char msg_total[inpkt->payload.len + 1];

    memcpy(msg_total, inpkt->payload.p, inpkt->payload.len);

    char* msg_h = strtok(msg_total, delimiters);

    int h = (int) strtol(msg_h, NULL, BASE_DECIMAL);

    if (h < STEPS_MIN || h > STEPS_MAX) {
        puts("ERROR: Received values out of bounds [put_servoh_nstep].");
    } else {
        if (h < 0) {
            stepn_l(-h);
        } else if (h > 0) {
            stepn_r(h);
        }

        char testresponse[TEST_RESPONSE_SIZE];
        strcpy(testresponse, "{ \"status\":ok");

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, 
            len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, 
            CONTENT_TYPE_JSON);
    }

    return 0;
}

static int handle_put_servov_nstep(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    static const char delimiters[] = TOKENIZATION_DELIMITERS;
    char msg_total[inpkt->payload.len + 1];

    memcpy(msg_total, inpkt->payload.p, inpkt->payload.len);

    char* msg_v = strtok(msg_total, delimiters);

    int v = (int) strtol(msg_v, NULL, BASE_DECIMAL);

    if (v < STEPS_MIN || v > STEPS_MAX) {
        puts("ERROR: Received values out of bounds [put_servoh_nstep].");
    } else {
        if (v < 0) {
            stepn_d(-v);
        } else if(v > 0) {
            stepn_u(v);
        }

        char testresponse[TEST_RESPONSE_SIZE];
        strcpy(testresponse, "{ \"status\":ok");

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, 
            len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, 
            CONTENT_TYPE_JSON);
    }

    return 0;
}

static int handle_put_servoh_angle(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    RN_UNUSED(scratch);
    RN_UNUSED(inpkt);
    RN_UNUSED(outpkt);
    RN_UNUSED(id_hi);
    RN_UNUSED(id_lo);

    return 0;
}

static int handle_put_servov_angle(coap_rw_buffer_t* scratch, 
const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {

    RN_UNUSED(scratch);
    RN_UNUSED(inpkt);
    RN_UNUSED(outpkt);
    RN_UNUSED(id_hi);
    RN_UNUSED(id_lo);

    return 0;
}

static int handle_put_servos_nstep(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    static const char delimiters[] = TOKENIZATION_DELIMITERS;
    char msg_total[inpkt->payload.len + 1];

    memcpy(msg_total, inpkt->payload.p, inpkt->payload.len);

    char* msg_h = strtok(msg_total, delimiters);
    char* msg_v = strtok(NULL, delimiters);

    int h = (int) strtol(msg_h, NULL, BASE_DECIMAL);
    int v = (int) strtol(msg_v, NULL, BASE_DECIMAL);

    if (h < STEPS_MIN || h > STEPS_MAX || v < STEPS_MIN || v > STEPS_MAX) {
        puts("ERROR: Received values out of bounds [put_servoh_nstep].");
    } else {
        if (h < 0) {
            stepn_l(-h);
        } else if (h > 0) {
            stepn_r(h);
        }

        if (v < 0) {
            stepn_d(-v);
        } else if(v > 0) {
            stepn_u(v);
        }

        char testresponse[TEST_RESPONSE_SIZE];
        strcpy(testresponse, "{ \"status\":ok");

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, 
            len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, 
            CONTENT_TYPE_JSON);
    }

    return 0;
}

static int handle_put_servos_angle(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    RN_UNUSED(scratch);
    RN_UNUSED(inpkt);
    RN_UNUSED(outpkt);
    RN_UNUSED(id_hi);
    RN_UNUSED(id_lo);

    return 0;
}

static int handle_put_laser(coap_rw_buffer_t* scratch, 
    const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo) {
    
    puts("Handle Put Laser");
    uint8_t value = inpkt->payload.p[0];

    if (inpkt->payload.len != 1 || value > '1' || value < '0') {
        puts("Error, inpkt->len != 1");
    } else {
        if (value == '1') {
            puts("Turned the laser on");
            laser_on();
        } else if (value == '0') {
            puts("Turned the laser off");
            laser_off();
        } else {
            puts("False value for LED");
        }

        char valuehandle[VALUE_HANDLE_SIZE];
        snprintf(valuehandle, VALUE_HANDLE_SIZE, "%u", value);

        char testresponse[TEST_RESPONSE_SIZE];
        strcpy(testresponse, "{ \"ledstatus\":");
        strncat(testresponse, valuehandle, strlen(testresponse));
        strncat(testresponse, " }", strlen(testresponse));

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, 
            len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, 
            CONTENT_TYPE_JSON);
    }

    return -1;
}
