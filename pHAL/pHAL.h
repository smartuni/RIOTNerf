#ifndef PHAL_H
#define PHAL_H

/* Stepsize in degree */
#define STEP_SIZE 8

int pHAL_init(void);

void set_h(int angle);
void set_v(int angle);

/* hv range between 500...2000 */
void sethv(int h, int v);

/* 2 axis stepwise movement */
void step_l(void);
void step_r(void);
void step_u(void);
void step_d(void);

void stepn_l(int n);
void stepn_r(int n);
void stepn_u(int n);
void stepn_d(int n);

void laser_on(void);
void laser_off(void);
void laser_toggle(void);

void allign_cntr(void);
void allign_h_cntr(void);
void allign_v_cntr(void);

#endif /* PHAL_H */
