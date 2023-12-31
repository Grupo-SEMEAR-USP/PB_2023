#ifndef PID_H
    #define PID_H
/**
 * @file pid.c
 * @brief Implementation of the PID logic.
 *
 * This file contains the implementation of the PID for two motors. 
 * The functions initializes and configure PID controlls for calculating new output velocity values and use a PID logic to get to a target velocity given by ROS. The PID
 * can control motors by changing their PWM values and increase or decrease the robot velocity.
 *
 * Authors:
 *          
 */

/* Includes */
#include "pid_ctrl.h"
#include "encoder.h"
#include "h_bridge.h"      

/* Definition of PID controlls parameters. */
#define KI_L 1 //Integrativo (Soma o estado anterior para auxliar o crescimento de convergência)
#define KD_L 2 //Derivativo (Caso a velocidade inicie seu declínio, a derivada do gráfico de V se torna negativa - decrescente - e os valores somados por KD auxiliam nessa queda)
#define KP_L 3 //Proporcional (Determina o grau de crescimento do meu parâmetro desejado - Velocidade)

#define KI_R 5
#define KP_R 5
#define KD_R 5

/* It limitates the output value to avoid extremely high or low intructions */
#define Max_Output 1000 
#define Min_Output 1
#define Max_integral 200
#define Min_integral 1

/* Functions */

typedef enum {
        PID_LEFT = 0,
        PID_RIGHT = 1
    } pid_side_t;

#define PID_SIDE_KP(NUM) NUM == (PID_LEFT) ? KP_L : KP_R
#define PID_SIDE_KI(NUM) NUM == (PID_LEFT) ? KI_L : KI_R
#define PID_SIDE_KD(NUM) NUM == (PID_LEFT) ? KD_L : KD_R

/**
 * @brief Initialize PIDs parameters
 * 
 * Initialize PIDs control blocks with KI, KD and KP values, diferently set for each motor.
 * 
 * @return esp_err_t
 */
pid_ctrl_block_handle_t init_pid(pid_side_t side);

/**
* @brief Calculate the PWM values based on the error
* 
* This function calculates an error based on the difference between the current velocity value and the goal one. Then generates the output value for both right and left motors PWMs,
* that alterates their values to get to the target velocity sent by ROS with a frequency defined by the communication part.
* 
* @param upcnt_unit_L Encoder configuration values. 
* @param pid_block_L PID configuration values. 
* @param upcnt_unit_R Encoder configuration values. 
* @param pid_block_R PID configuration values. 
* @return esp_err_t
*/
esp_err_t pid_calculate(pcnt_unit_handle_t upcnt_unit_L, pid_ctrl_block_handle_t pid_block_L, pcnt_unit_handle_t upcnt_unit_R, pid_ctrl_block_handle_t pid_block_R);

/**
* @brief It gets a velocity target for motor
* 
* This function gets a velocity target sent by the control in frequency previously set
* 
* @param void. 
* @return float.
*/
//float pid_get_target(void);


/**
* @brief It sends a flag to stop the PID
* 
* This function sends a boolean flag defined by comunication, representing false when the velocity stays the same and true when a new target
* value is received, stoping the PID execution and passing a new target value. 
* 
* @param void. 
* @return bool.
*/
//bool target_change_flag(void);
#endif