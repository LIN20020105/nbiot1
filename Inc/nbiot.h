#ifndef __NBIOT_H__
#define __NBIOT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void send_NB_IoT(const char* cmd);
void Clear_Buffer(void);
void NB_IotConnect(void);
void nbiot_reset(void);
void NB_IoT_connect_MQTT(void);
void publishMQTTMessage(void);
void send_ADC_Values(uint32_t adc1, uint32_t adc2, uint32_t adc3);
uint8_t NB_IoT_ack_check(const char* str);
void send_adc_values_as_binary(uint32_t adc1, uint32_t adc2, uint32_t adc3);
size_t string_to_hex(const char *str, uint8_t *hex_data);
uint8_t str_to_hex(char *str, uint8_t *out);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __NBIOT_H__ */
