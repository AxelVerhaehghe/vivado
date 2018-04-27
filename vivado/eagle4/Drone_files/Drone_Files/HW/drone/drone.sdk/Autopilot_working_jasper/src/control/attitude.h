/***********************************************************************
*   Attitude controller header file
*   this script contains functions used to generate inputs to the drone
*   that allow it to track the attitude reference from the RC or from the 
*   navigation controller
*
*   author: p. coppens
************************************************************************/
#ifndef ATTITUDE_H
#define ATTITUDE_H

#include "../main.h"
#include "../EAGLE4/EAGLE4.h"
/* --------- Constants --------- */

#define CTR_OUT_LOW 40.0
#define CTR_OUT_HIGH 90.0



/* --------- Method prototypes --------- */


/**
 *  this method is used during flying to get input based on a certain 
 *  reference signal generated by the RC input or by the navigator
 *  the input is processed and turned into a control signal that 
 *  stabilizes the drone around the attitude provided in the reference signal.
 *
 *  @param thrust the thrust provided by the rc or navigator (0..1)
 *  @param rot_x the requested roll provided by the rc or navigator (0..1 = [0..45DEG])
 *  @param rot_x_y the requested pitch provided by the rc or navigator (0..1 = [0..45DEG])
 *  @param rot_z the requested yaw provided by the rc or navigator (0..1 = [0..360DEG])
 */
void controller_flying(
	float thrust, 
	float rot_x, 
	float rot_y, 
	float rot_z);

/**
 *  this method is used to initiate the controller
 *  it does this by loading some initial values into 
 *  the controller's and observer's states
 */
void controller_init();

/**
 *  used instead controller_flying when the controller is disarmed
 *  it deactivates all engines
 */
void controller_idle();

/**
 *  this method is used to read in the measurements from the IMU and
 *  store them in variables that are used in the control calculations
 */
void load_measurements();

/**
 *  this method reads the input from the rc or navigator and 
 *  transforms the parameters to values that can be used by the controller
 *
 *  @param thrust the thrust provided by the rc or navigator (0..1)
 *  @param rot_x the requested roll provided by the rc or navigator (0..1 = [0..45DEG])
 *  @param rot_y the requested pitch provided by the rc or navigator (0..1 = [0..45DEG])
 *  @param rot_z the requested yaw provided by the rc or navigator (0..1 = [0..360DEG])
 */
void load_input(
	float thrust, 
	float rot_x, 
	float rot_y, 
	float rot_z);



/* --------- Macros --------- */

/**
 *  CLAMP Method used to clamp the voltages
 *
 *  parameters:
 *  x:      the value to clamp
 *  lo:     the lower boundary
 *  hi:     the higher boundary
 */
#define CLAMP_INPLACE(x, lo, hi) { \
	if((x) < ((lo))) \
		(x) = (lo); \
	if((x) > ((hi))) \
		(x) = (hi); \
}



/* ------- Global Variables ------- */

float rel_yaw; /* relative yaw */
float r[4]; /* reference signal */
float v[4]; /* voltage control signals */
float q[4]; /* orientation measurement */
float w[3]; /* angular velocity measurement */

#endif /* ATTITUDE_H */