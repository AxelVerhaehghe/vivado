/******************************************************************************
*   Main script
*   this is the first script executed when the BareMetal starts. It set's up
*   all of the components on the Zybo, starts the interrupts and runs a loop
*   that logs data.
******************************************************************************/
#include "main.h"


/**
 * main entry
 *
 * When booting in AMP mode with both Linux and BareMetal present
 * the BareMetal starts after Linux runs the command:
 * ./rwmem 0xfffffff0 0x18000000 &
 */
int main(void) {
	// Initialize Platform
	// ======================================================================
	// configure inter-processor communication
	eagle_setup_ipc();

	// enable caches etc
	init_platform();

	// setup clock control
	eagle_setup_clk();

	// Initialize Interrupt system
	// ======================================================================
	// Makes sure that the interrupt driver is ready to be configured
	// for taking interrupts from both the IMU and the IIC
	// ======================================================================
	// enable interrupt system
	Status = SetupInterruptSystem();
	if (Status != XST_SUCCESS) {
		xil_printf("setup interrupt system failed /r/n");
		return XST_FAILURE;
	}

	// Initialize Integrated Integrated Circuits (IIC)
	// ======================================================================
	// This connects one interrupt to the IIC so that it can handle
	// the communication with the IMU. The interrupt is used to tell
	// the system that the data transfer has been handled
	// ======================================================================
	// configure the IIC data structure
	IicConfig(XPAR_XIICPS_0_DEVICE_ID, &Iic0);

	// Initialize the attitude controller
	// ======================================================================
	controller_init();

	// Initialize the sonar filters
	// ======================================================================
	init_sonar();

	// Initialize the state of the OCM
	// ======================================================================
	// The OCM is used to communicate with the other CPU running python.
	// We need this communication for receiving position data and
	// for transmitting the log data.
	// ======================================================================
	init_comm();

	// Initialize all of the biases
	// ======================================================================
	// The altitude and navigation both have to deal with a bias.
	// Altitude must know the hover throttle that is required to keep the
	// drone stably in the air, while the navigation has to know what
	// pitch and roll keep the drone stable in the other directions.
	// ======================================================================
	init_bias();

	// Initialize the PWM
	// ======================================================================
	// initialize PWM + visual check with LEDs blinking
	PWM_init();

	// Initialize the IMU
	// ======================================================================
	// The IMU controls the frequency of our controller. It sends a pulse
	// through a cable to block JB on the Zybo. (JB4 when in AMP mode and
	// JB10 when just executing the BareMetal framework. You have to make
	// sure that the cable is connected to the correct pin)
	// The interrupt system detects this pulse and executes the code in the
	// Int_gyr method in int.c. A pulse is send at a frequency of 238.0Hz.
	// These interrupts provide the updates to the fsm described in fsm.c
	// ======================================================================
	// initiate IMU registers
	initIMU();

	//-------------------- MAIN EXECUTION -------------------
	while (1) {
		// If the main loop can set this value to 0
		// before the led is printed then the interrupt is processed to slowly
		led_check = 0;

		// Log the data to CPU0
		// =========================================================================
		// We don't do this in the interrupt as it takes too long.
		// When we call it over here and it takes too long to write all of the data,
		// the code will just be interrupted and continued later. This ensures that
		// the controller continues running at 238Hz
		log_data();
	}

	cleanup_platform();
	return 1;
}

