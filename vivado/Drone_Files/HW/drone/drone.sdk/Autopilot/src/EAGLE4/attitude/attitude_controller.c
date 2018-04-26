#include "../EAGLE4.h"
#include <math.h>

/** Defining static variables */
static real_t attitude_state_estimate[NX];
static real_t attitude_state_estimate_reduced[NX_REDUCED];
static real_t attitude_state_estimate_prev[NX_REDUCED];
static real_t attitude_control_actions[NU];
static real_t relative_attitude[NX_REDUCED];

/**
* Call this before taking off.
*/
void attitude_controller_init(void) {
	/* Initializing the attitude_state_estimate_reduced which we use for our calculations of the new state estimates */
	attitude_state_estimate_reduced[0] = 0.0;
	attitude_state_estimate_reduced[1] = 0.0;
	attitude_state_estimate_reduced[2] = 0.0;
	attitude_state_estimate_reduced[3] = 0.0;
	attitude_state_estimate_reduced[4] = 0.0;
	attitude_state_estimate_reduced[5] = 0.0;
	attitude_state_estimate_reduced[6] = 0.0;
	attitude_state_estimate_reduced[7] = 0.0;
	attitude_state_estimate_reduced[8] = 0.0;
	/* Initializing the attitude_control_actions */
	attitude_control_actions[0] = 0.0;
	attitude_control_actions[1] = 0.0;
	attitude_control_actions[2] = 0.0;
	/* Initializing our attitude_state_estimates which we use to compute our control actions*/
	attitude_state_estimate[0] = 1.0;
	attitude_state_estimate[1] = 0.0;
	attitude_state_estimate[2] = 0.0;
	attitude_state_estimate[3] = 0.0;
	attitude_state_estimate[4] = 0.0;
	attitude_state_estimate[5] = 0.0;
	attitude_state_estimate[6] = 0.0;
	attitude_state_estimate[7] = 0.0;
	attitude_state_estimate[8] = 0.0;
	attitude_state_estimate[9] = 0.0;
}

/**
* Getter for attitude_control_actions
*/
real_t * get_attitude_control_action(void) {
	return attitude_control_actions;
}

/**
* Getter for attitude_state_estimate_reduced
*/
real_t * get_attitude_state_estimate_reduced(void) {
	return attitude_state_estimate_reduced;
}

/**
 * STATE_ESTIMATOR estimates the next state based on the previous state.
 * this function uses the measurements of the IMU and
 * the L matrix previously calculated in MATLAB
 */
void state_estimator(void){
/* here we only use the last 6 values of our inputted imu_measurements so we work with the reduced version */

	real_t * imu_measurements_reduced = get_imu_measurements_reduced();
	/* x_est = Ad*x_est + L*(Cd*x_est - y) + Bd*u */

	 attitude_state_estimate_prev[0] = attitude_state_estimate_reduced[0];
	 attitude_state_estimate_prev[1] = attitude_state_estimate_reduced[1];
	 attitude_state_estimate_prev[2] = attitude_state_estimate_reduced[2];
	 attitude_state_estimate_prev[3] = attitude_state_estimate_reduced[3];
	 attitude_state_estimate_prev[4] = attitude_state_estimate_reduced[4];
	 attitude_state_estimate_prev[5] = attitude_state_estimate_reduced[5];
	 attitude_state_estimate_prev[6] = attitude_state_estimate_reduced[6];
	 attitude_state_estimate_prev[7] = attitude_state_estimate_reduced[7];
	 attitude_state_estimate_prev[8] = attitude_state_estimate_reduced[8];
	/* attitude_state_estimate_reduced <-- (Ad + L*Cd) * attitude_state_estimate_prev */
	attitude_state_estimate_reduced[0] = (9.978997e-01) * attitude_state_estimate_prev[0] + (-5.014775e-04) * attitude_state_estimate_prev[3] + (1.397984e-05) * attitude_state_estimate_prev[6];
	attitude_state_estimate_reduced[1] = (9.978997e-01) * attitude_state_estimate_prev[1] + (-4.855121e-04) * attitude_state_estimate_prev[4] + (1.319862e-05) * attitude_state_estimate_prev[7];
	attitude_state_estimate_reduced[2] = (9.979002e-01) * attitude_state_estimate_prev[2] + (-5.395079e-04) * attitude_state_estimate_prev[5] + (-8.212841e-06) * attitude_state_estimate_prev[8];
	attitude_state_estimate_reduced[3] = (-1.548006e-03) * attitude_state_estimate_prev[0] + (5.035498e-01) * attitude_state_estimate_prev[3] + (1.304781e-02) * attitude_state_estimate_prev[6];
	attitude_state_estimate_reduced[4] = (-1.566676e-03) * attitude_state_estimate_prev[1] + (5.194812e-01) * attitude_state_estimate_prev[4] + (1.231867e-02) * attitude_state_estimate_prev[7];
	attitude_state_estimate_reduced[5] = (-1.491342e-03) * attitude_state_estimate_prev[2] + (4.689172e-01) * attitude_state_estimate_prev[5] + (-7.665294e-03) * attitude_state_estimate_prev[8];
	attitude_state_estimate_reduced[6] = (7.493729e-03) * attitude_state_estimate_prev[0] + (-6.530278e+00) * attitude_state_estimate_prev[3] + (8.868778e-01) * attitude_state_estimate_prev[6];
	attitude_state_estimate_reduced[7] = (7.439628e-03) * attitude_state_estimate_prev[1] + (-6.514910e+00) * attitude_state_estimate_prev[4] + (8.868778e-01) * attitude_state_estimate_prev[7];
	attitude_state_estimate_reduced[8] = (3.327782e-03) * attitude_state_estimate_prev[2] + (-6.973919e+00) * attitude_state_estimate_prev[5] + (8.868778e-01) * attitude_state_estimate_prev[8];
	/* attitude_state_estimate_reduced <-- 1.00 * attitude_state_estimate_reduced - (L) * imu_measurements_reduced */
	attitude_state_estimate_reduced[0] = attitude_state_estimate_reduced[0] - (-2.100281e-03) * imu_measurements_reduced[0] - (-2.602318e-03) * imu_measurements_reduced[3] ;
	attitude_state_estimate_reduced[1] = attitude_state_estimate_reduced[1] - (-2.100299e-03) * imu_measurements_reduced[1] - (-2.586352e-03) * imu_measurements_reduced[4] ;
	attitude_state_estimate_reduced[2] = attitude_state_estimate_reduced[2] - (-2.099829e-03) * imu_measurements_reduced[2] - (-2.640348e-03) * imu_measurements_reduced[5] ;
	attitude_state_estimate_reduced[3] = attitude_state_estimate_reduced[3] - (-1.548006e-03) * imu_measurements_reduced[0] - (-4.964502e-01) * imu_measurements_reduced[3] ;
	attitude_state_estimate_reduced[4] = attitude_state_estimate_reduced[4] - (-1.566676e-03) * imu_measurements_reduced[1] - (-4.805188e-01) * imu_measurements_reduced[4] ;
	attitude_state_estimate_reduced[5] = attitude_state_estimate_reduced[5] - (-1.491342e-03) * imu_measurements_reduced[2] - (-5.310828e-01) * imu_measurements_reduced[5] ;
	attitude_state_estimate_reduced[6] = attitude_state_estimate_reduced[6] - (7.493729e-03) * imu_measurements_reduced[0] - (-6.530278e+00) * imu_measurements_reduced[3] ;
	attitude_state_estimate_reduced[7] = attitude_state_estimate_reduced[7] - (7.439628e-03) * imu_measurements_reduced[1] - (-6.514910e+00) * imu_measurements_reduced[4] ;
	attitude_state_estimate_reduced[8] = attitude_state_estimate_reduced[8] - (3.327782e-03) * imu_measurements_reduced[2] - (-6.973919e+00) * imu_measurements_reduced[5] ;
	/* attitude_state_estimate_reduced <-- 1.00 * attitude_state_estimate_reduced + (Bd) * attitude_control_actions */
	attitude_state_estimate_reduced[0] = attitude_state_estimate_reduced[0] + (6.584711e-05) * attitude_control_actions[0] ;
	attitude_state_estimate_reduced[1] = attitude_state_estimate_reduced[1] + (6.216742e-05) * attitude_control_actions[1] ;
	attitude_state_estimate_reduced[2] = attitude_state_estimate_reduced[2] + (1.053391e-03) * attitude_control_actions[2] ;
	attitude_state_estimate_reduced[3] = attitude_state_estimate_reduced[3] + (9.310577e-02) * attitude_control_actions[0] ;
	attitude_state_estimate_reduced[4] = attitude_state_estimate_reduced[4] + (8.790280e-02) * attitude_control_actions[1] ;
	attitude_state_estimate_reduced[5] = attitude_state_estimate_reduced[5] + (9.849575e-01) * attitude_control_actions[2] ;
	attitude_state_estimate_reduced[6] = attitude_state_estimate_reduced[6] + (1.318439e+01) * attitude_control_actions[0] ;
	attitude_state_estimate_reduced[7] = attitude_state_estimate_reduced[7] + (1.318439e+01) * attitude_control_actions[1] ;
	attitude_state_estimate_reduced[8] = attitude_state_estimate_reduced[8] + (1.318439e+01) * attitude_control_actions[2] ;
}

/**
* This function expands the state_estimates to the full form
*/
void attitude_state_reduced_to_full(void){
	real_t * imu_measurements = get_imu_measurements();
	attitude_state_estimate[0] = copysign(sqrt(1 - attitude_state_estimate_reduced[0] * attitude_state_estimate_reduced[0] - attitude_state_estimate_reduced[1] * attitude_state_estimate_reduced[1] - attitude_state_estimate_reduced[2] * attitude_state_estimate_reduced[2]),imu_measurements[0]);
	attitude_state_estimate[1] = attitude_state_estimate_reduced[0];
	attitude_state_estimate[2] = attitude_state_estimate_reduced[1];
	attitude_state_estimate[3] = attitude_state_estimate_reduced[2];
	attitude_state_estimate[4] = attitude_state_estimate_reduced[3];
	attitude_state_estimate[5] = attitude_state_estimate_reduced[4];
	attitude_state_estimate[6] = attitude_state_estimate_reduced[5];
	attitude_state_estimate[7] = attitude_state_estimate_reduced[6];
	attitude_state_estimate[8] = attitude_state_estimate_reduced[7];
	attitude_state_estimate[9] = attitude_state_estimate_reduced[8];
}

/**
 * COMPUTE_CONTROL_ACTION calculates the output of the controller
 * based on the K matrix calculated in matlab and the estimated state.
 * But first we calculate the relative_attitude
 * The first 3 elements of the array are created by the Hamilton product of the measured quaternion and the conjugate of the estimated quaternion
 * The rest is equal to the attitude_stat_estimate because the reference angular velocity and the reference deviation from the hovering spin of rotation are 0).
 */
void compute_ctrl_action(void){
	real_t * attitude_reference = get_attitude_reference();
	attitude_state_reduced_to_full();
	relative_attitude[0] = attitude_state_estimate[1] * attitude_reference[0] - attitude_reference[1] * attitude_state_estimate[0] + attitude_state_estimate[3] * attitude_reference[2] - attitude_reference[3] * attitude_state_estimate[2] ;
	relative_attitude[1] = attitude_state_estimate[2] * attitude_reference[0] - attitude_reference[1] * attitude_state_estimate[3] - attitude_state_estimate[0] * attitude_reference[2] + attitude_reference[3] * attitude_state_estimate[1] ;
	relative_attitude[2] = attitude_state_estimate[3] * attitude_reference[0] + attitude_reference[1] * attitude_state_estimate[2] - attitude_state_estimate[1] * attitude_reference[2] - attitude_reference[3] * attitude_state_estimate[0] ;
	relative_attitude[3] = attitude_state_estimate[4] ;
	relative_attitude[4] = attitude_state_estimate[5] ;
	relative_attitude[5] = attitude_state_estimate[6] ;
	relative_attitude[6] = attitude_state_estimate[7] ;
	relative_attitude[7] = attitude_state_estimate[8] ;
	relative_attitude[8] = attitude_state_estimate[9] ;
	/* attitude_control_actions <-- (K) * relative_attitude */
	attitude_control_actions[0] = (-1.586674e+01) * relative_attitude[0] + (-7.169046e-01) * relative_attitude[3] + (-7.202287e-02) * relative_attitude[6];
	attitude_control_actions[1] = (-1.590354e+01) * relative_attitude[1] + (-7.332292e-01) * relative_attitude[4] + (-7.185947e-02) * relative_attitude[7];
	attitude_control_actions[2] = (-1.147848e+01) * relative_attitude[2] + (-8.788310e-01) * relative_attitude[5] + (-3.142960e-03) * relative_attitude[8];
}

void set_control_action(float* u) {
	attitude_control_actions[0] = u[0];
	attitude_control_actions[1] = u[1];
	attitude_control_actions[2] = u[2];
}