#include <coap.h>
#include <string.h>
#include <stdio.h>              /* puts, snprintf */
#include <stdlib.h>
#include <periph/gpio.h>
#include "pHAL.h"

#define RN_UNUSED(arg)  ((void) (arg))
#define RN_COUNTOF(array)	((sizeof((array))) / (sizeof(*(array))))

#define MAX_RESPONSE_LEN 500
static uint8_t response[MAX_RESPONSE_LEN] = { 0U };


static int handle_get_well_known_core(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_get_servos(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);

static int handle_put_servoh_step(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_servov_step(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_servoh_nstep(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt,  coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_servov_nstep(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_servoh_angle(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_servov_angle(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_servos_nstep(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_servos_angle(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);
static int handle_put_laser(coap_rw_buffer_t* scratch,  const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo);


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
static int handle_get_well_known_core(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
    char *rsp = (char *) response;
    /* resetting the content of response message */
    memset(response, 0, RN_COUNTOF(response));
    int len = RN_COUNTOF(response);
    const coap_endpoint_t *ep = endpoints;
    int i;

    len--;                      // Null-terminated string

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
    }                           /* END while */

    return coap_make_response(scratch, outpkt, (const uint8_t *) rsp,
                              strlen(rsp), id_hi, id_lo, &inpkt->tok,
                              COAP_RSPCODE_CONTENT,
                              COAP_CONTENTTYPE_APPLICATION_LINKFORMAT);
}

static int handle_get_servos(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
    static const char testresponse[] =
        "[ {\"name\":\"servoH\", \"unis\":2130}, {\"name\":\"servoL\", \"units\":70} ]";

    memcpy(response, testresponse, sizeof(testresponse) - 1);

    return coap_make_response(scratch, outpkt, (const uint8_t *) response,
                              sizeof(testresponse) - 1, id_hi, id_lo,
                              &inpkt->tok, COAP_RSPCODE_CONTENT,
                              (coap_content_type_t) 50);
}

static int handle_put_servoh_step(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
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
        } else
            puts("False value for Servo H step");

        char valuehandle[2] = { '\0' };
        snprintf(valuehandle, 2, "%u", value);

        char testresponse[20] = { '\0' };
        strcpy(testresponse, "{ \"ledstatus\":");
        strncat(testresponse, valuehandle, strlen(testresponse));
        strncat(testresponse, " }", strlen(testresponse));

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, (coap_content_type_t) 50);
    }

    return -1;
}

static int handle_put_servov_step(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
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
        } else
            puts("False value for Servo H step");

        char valuehandle[2];
        snprintf(valuehandle, 2, "%u", value);

        char testresponse[20];
        strcpy(testresponse, "{ \"ledstatus\":");
        strncat(testresponse, valuehandle, strlen(testresponse));
        strncat(testresponse, " }", strlen(testresponse));

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, (coap_content_type_t) 50);
    }

    return -1;
}

static int handle_put_servoh_nstep(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt,  coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
    static const char delimiters[] = " ,;:";
    char msg_total[inpkt->payload.len + 1];

    memcpy(msg_total, inpkt->payload.p, inpkt->payload.len);

    char* msg_h = strtok(msg_total, delimiters);

    int h = (int) strtol(msg_h, NULL, 10);

    if(h < -1500 || h > 1500) {
        puts("ERROR: Received values out of bounds [put_servoh_nstep].");
    }
    else {
        if(h < 0) {
            stepn_l(-h);
        }
        else if(h > 0){
            stepn_r(h);
        }

        char testresponse[20];
        strcpy(testresponse, "{ \"status\":ok");

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, (coap_content_type_t) 50);
    }

    return 0;
}

static int handle_put_servov_nstep(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
    static const char delimiters[] = " ,;:";
    char msg_total[inpkt->payload.len + 1];

    memcpy(msg_total, inpkt->payload.p, inpkt->payload.len);

    char* msg_v = strtok(msg_total, delimiters);

    int v = (int) strtol(msg_v, NULL, 10);

    if(v < -1500 || v > 1500) {
        puts("ERROR: Received values out of bounds [put_servoh_nstep].");
    }
    else {
        if(v < 0) {
            stepn_d(-v);
        }
        else if(v > 0){
            stepn_u(v);
        }

        char testresponse[20];
        strcpy(testresponse, "{ \"status\":ok");

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, (coap_content_type_t) 50);
    }

    return 0;
}

static int handle_put_servoh_angle(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
    RN_UNUSED(scratch);
    RN_UNUSED(inpkt);
    RN_UNUSED(outpkt);
    RN_UNUSED(id_hi);
    RN_UNUSED(id_lo);

    return 0;
}

static int handle_put_servov_angle(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
    RN_UNUSED(scratch);
    RN_UNUSED(inpkt);
    RN_UNUSED(outpkt);
    RN_UNUSED(id_hi);
    RN_UNUSED(id_lo);

    return 0;
}

static int handle_put_servos_nstep(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
    static const char delimiters[] = " ,;:";
    char msg_total[inpkt->payload.len + 1];

    memcpy(msg_total, inpkt->payload.p, inpkt->payload.len);

    char* msg_h = strtok(msg_total, delimiters);
    char* msg_v = strtok(NULL, delimiters);

    int h = (int) strtol(msg_h, NULL, 10);
    int v = (int) strtol(msg_v, NULL, 10);

    if(h < -1500 || h > 1500 || v < -1500 || v > 1500) {
        puts("ERROR: Received values out of bounds [put_servoh_nstep].");
    }
    else {
        if(h < 0) {
            stepn_l(-h);
        }
        else if(h > 0){
            stepn_r(h);
        }

        if(v < 0) {
            stepn_d(-v);
        }
        else if(v > 0){
            stepn_u(v);
        }

        char testresponse[20];
        strcpy(testresponse, "{ \"status\":ok");

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, (coap_content_type_t) 50);
    }

    return 0;
}

static int handle_put_servos_angle(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
    RN_UNUSED(scratch);
    RN_UNUSED(inpkt);
    RN_UNUSED(outpkt);
    RN_UNUSED(id_hi);
    RN_UNUSED(id_lo);

    return 0;
}

static int handle_put_laser(coap_rw_buffer_t* scratch, const coap_packet_t* inpkt, coap_packet_t* outpkt, uint8_t id_hi, uint8_t id_lo)
{
    puts("Handle Put Testled");
    uint8_t value = inpkt->payload.p[0];


    if (inpkt->payload.len != 1 || value > '1' || value < '0') {
        puts("Error, inpkt->len != 1");
    } else {
        if (value == '0')
            puts("Reset Laser");
            laser_on();
        else if (value == '1')
            puts("Set Laser");
            laser_off();
        else
            puts("False value for LED");

        char valuehandle[2];
        snprintf(valuehandle, 2, "%u", value);

        char testresponse[20];
        strcpy(testresponse, "{ \"ledstatus\":");
        strncat(testresponse, valuehandle, strlen(testresponse));
        strncat(testresponse, " }", strlen(testresponse));

        int len = strlen(testresponse);

        memcpy(response, testresponse, len);

        return coap_make_response(scratch, outpkt, (const uint8_t *) response, len, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, (coap_content_type_t) 50);
    }

    return -1;
}
