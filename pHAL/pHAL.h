#ifndef RIOTNERF_INCG_PHAL_H
#define RIOTNERF_INCG_PHAL_H

/*! \def STEP_SIZE
 *  \brief Value that represents a single servo step.
 *
 *  An integer constant that represents the the size of a step.
 *  Used to add or subtract one or multiple steps in the step functions
 *  to or from the current position of the servo that is associated with
 *  the corresponding direction.
 *  The value chosen is the smallest step one of the servos used can
 *  possibly do, given how inaccurate they are. 
**/
#define STEP_SIZE (8)

/*! \def PHAL_INIT_LASER_ERROR
 *  \brief Error code returned by pHAL_init to indicate failure to initialize laser. 
**/
#define PHAL_INIT_LASER_ERROR   (-1)

/*! \def PHAL_INIT_SERVO1_ERROR
 *  \brief Error code returned by pHAL_init to indicate failure to initialize servo 1.
**/
#define PHAL_INIT_SERVO1_ERROR  (-2)

/*! \def PHAL_INIT_SERVO2_ERROR
 *  \brief Error code returned by pHAL_init to indicate failure to initialize servo 2.
**/
#define PHAL_INIT_SERVO2_ERROR  (-3)

/*! \brief pHAL_init The function that initializes the laser and servos to set up the pHAL.
 *  \return returns 0 on success, otherwise
 *          returns PHAL_INIT_LASER_ERROR if initialization of the laser failed.
 *          returns PHAL_INIT_SERVO1_ERROR if initialization of servo 1 failed.
 *          returns PHAL_INIT_SERVO2_ERROR if initialization of servo 2 failed.
 *  \warning This function must be called exactly once 
 *           before any of the other functions exported
 *           by this header file are called.
 *
 * Initializes the pHAL by initializing the laser's GPIO pin and
 * settung up the servos to be used. On error the function will return an error code
 * and print the error message associated to stdout.
**/
int pHAL_init(void);

/*! \brief set_h Sets the horizontal servo to the angle passed to the function.
 *  \param angle the angle to set the horizontal servo to; must be within [-90..90]
 *  \warning Undefined behavior if angle is not in [-90..90].
 *           Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
**/
void set_h(int angle);

/*! \brief set_v Sets the vertical servo to the angle passed to the function.
 *  \param angle the angle to set t he vertical servo to; must be within [-90..90]
 *  \warning Undefined behavior if angle is not in [-90..90].
 *           Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
**/
void set_v(int angle);

/*! \brief sethv Sets both servos to positions given.
 *  \param h The position to set the horizontal servo to;
 *         must be in the range of [500..2000].
 *  \param v The position to set the vertical servo to;
 *         must be in the range of [500..2000].
 *  \warning Undefined behavior if h or v are not within [500..2000].
 *           Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
 *  \note For an interface based on angles of -90 to 90 degrees 
 *        the function set_h and set_v can be used to set the positions
 *        of the horizontal servo and vertical servo, respectively.
**/
void sethv(int h, int v);

/*! \brief step_l Performs a single step to the left.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
 *  
 *  The horizontal servo's position is incremented by STEP_SIZE once,
 *  resulting in the smallest possible leftward movement.
**/
void step_l(void);

/*! \brief step_r Performs a single step to the right.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
 *
 *  The horizontal servo's position is decremented by STEP_SIZE once,
 *  resulting in the smallest possible rightward movement.
**/
void step_r(void);

/*! \brief step_u Performs a single upward step.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
 *
 *  The vertical servo's position is decremented by STEP_SIZE once,
 *  resulting in the smallest possible upward movement.
**/
void step_u(void);

/*! \brief step_d Performs a single downward step.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
 *
 *  The vertical servo's position is incremented by STEP_SIZE once,
 *  resulting in the smallest possible downward movement.
**/
void step_d(void);

/*! \brief stepn_l Performs n leftward steps.
 *  \param n the amount of leftward steps to perform.
 *  \note has no effect if n <= 0.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
**/
void stepn_l(int n);

/*! \brief stepn_r Performs n rightward steps.
 *  \param n the amount of rightward steps to perform.
 *  \note has no effect if n <= 0.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
**/
void stepn_r(int n);

/*! \brief stepn_u Performs n upward steps.
 *  \param n the amount of upward steps to perform.
 *  \note has no effect if n <= 0.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
**/
void stepn_u(int n);

/*! \brief stepn_d Performs n downward steps.
 *  \param n the amount of downward steps to perfom.
 *  \note has no effect if n <= 0.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
**/
void stepn_d(int n);

/*! \brief laser_on Activates the laser.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
**/
void laser_on(void);

/*! \brief laser_off Deactivates the laser.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
**/
void laser_off(void);

/*! \brief laser_toggle Toggles the laser.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
 *
 *  If the laser is already activated calling this function
 *  will turn the laser off.
 *  If the laser is deactivated calling this function will
 *  turn the laser on.
**/
void laser_toggle(void);

/*! \brief align_cntr Centers both servos.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
 *
 *  Sets the horizontal and the vertical servo to the 0 degree position.
**/
void align_cntr(void);

/*! \brief align_h_cntr Centers the horizontal servo.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
 *
 * Sets the horizontal servo to the 0 degree position.
**/
void align_h_cntr(void);

/*! \brief align_v_cntr Centers the vertical servo.
 *  \warning Only call this function after the pHAL has been initialized with a call
 *           to pHAL_init.
 *
 * Sets the vertical servo to the 0 degree position.
**/
void align_v_cntr(void);

#endif /* RIOTNERF_INCG_PHAL_H */
