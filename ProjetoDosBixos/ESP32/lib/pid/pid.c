#include "pid.h"

float valor_lido = 0;

const char *TAG_PID = "PID";

pid_ctrl_block_handle_t init_pid(pid_side_t side){

    pid_ctrl_config_t config_pid; 
    pid_ctrl_block_handle_t pid_block; 

    pid_ctrl_parameter_t values_pid = {
      .kd = PID_SIDE_KD(side),
      .kp = PID_SIDE_KP(side),
      .ki = PID_SIDE_KI(side),
      .min_integral = Min_integral,
      .max_integral = Max_integral,
      .min_output = Min_Output,
      .max_output = Max_Output,
      .cal_type = PID_CAL_TYPE_INCREMENTAL,
    };
    config_pid.init_param = values_pid;

    ESP_ERROR_CHECK(pid_new_control_block(&config_pid, &pid_block));
    ESP_ERROR_CHECK(pid_update_parameters(pid_block, &values_pid));
    return pid_block;
}

esp_err_t pid_calculate(pcnt_unit_handle_t upcnt_unit_L, pid_ctrl_block_handle_t pid_block_L, pcnt_unit_handle_t upcnt_unit_R, pid_ctrl_block_handle_t pid_block_R){
  float controll_pid_LEFT, controll_pid_RIGHT;

  float target_LEFT = TARGET_VALUE_L;
  float target_RIGHT = TARGET_VALUE_R;

  // while(!FLAG_TARGET){

  //   float ENCODER_READ_L = pulse_count(upcnt_unit_L) * 0.02;
  //   float ENCODER_READ_R = pulse_count(upcnt_unit_R) * 0.02;

  //   //Recalculate an error
  //   ESP_LOGI(TAG_PID, "Velocidade inicial ESQUERDA: %f", ENCODER_READ_L);
  //   ESP_LOGI(TAG_PID, "Velocidade inicial DIREITA: %f", ENCODER_READ_R);

  //   float error_motor_LEFT = (target_LEFT - ENCODER_READ_L);
  //   float error_motor_RIGHT = (target_RIGHT - ENCODER_READ_R);
    
  //   ESP_LOGI(TAG_PID, "Erro ESQUERDA: %f", error_motor_LEFT);
  //   ESP_LOGI(TAG_PID, "Erro DIREITA: %f", error_motor_RIGHT);
      
  //   vTaskDelay(100 / portTICK_PERIOD_MS);

  //   //Calculate a new PWM Value
  //   pid_compute(pid_block_L, error_motor_LEFT, &controll_pid_LEFT);
  //   pid_compute(pid_block_R, error_motor_RIGHT, &controll_pid_RIGHT);

  //   ESP_LOGI(TAG_PID, "Velocidade final ESQUERDA: %f", controll_pid_LEFT);
  //   ESP_LOGI(TAG_PID, "Velocidade final DIREITA: %f", controll_pid_RIGHT);

  //   update_motor(LEFT, 20 * controll_pid_LEFT);
  //   update_motor(RIGHT, 20 * controll_pid_RIGHT);

  //   vTaskDelay(300 / portTICK_PERIOD_MS);

  // }

  for(int i = 0; i < 10; i++){

    ENCODER_READ_L = pulse_count(upcnt_unit_L) * 0.02;
    ENCODER_READ_R = pulse_count(upcnt_unit_R) * 0.02;

    //Recalculate an error
    //ESP_LOGI(TAG_PID, "Velocidade inicial ESQUERDA: %f", ENCODER_READ_L);
    //ESP_LOGI(TAG_PID, "Velocidade inicial DIREITA: %f", ENCODER_READ_R);

    float error_motor_LEFT = (target_LEFT - ENCODER_READ_L);
    float error_motor_RIGHT = (target_RIGHT - ENCODER_READ_R);

    //ESP_LOGI(TAG_PID, "Erro ESQUERDA: %f", error_motor_LEFT);
    //ESP_LOGI(TAG_PID, "Erro DIREITA: %f", error_motor_RIGHT);
      
    vTaskDelay(100 / portTICK_PERIOD_MS);

    //Calculate a new PWM Value
    pid_compute(pid_block_L, error_motor_LEFT, &controll_pid_LEFT);
    pid_compute(pid_block_R, error_motor_RIGHT, &controll_pid_RIGHT);

    //ESP_LOGI(TAG_PID, "Velocidade final ESQUERDA: %f", controll_pid_LEFT);
    //ESP_LOGI(TAG_PID, "Velocidade final DIREITA: %f", controll_pid_RIGHT);

    update_motor(LEFT, 20 * controll_pid_LEFT);
    update_motor(RIGHT, 20 * controll_pid_RIGHT);

    vTaskDelay(300 / portTICK_PERIOD_MS);
  }

  return ESP_OK;
}
  