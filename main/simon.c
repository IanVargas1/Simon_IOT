
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/touch_pad.h"
#include "esp_log.h"
#include "wifi_sta.h"
#include "touch_sensor.h"
#include "mqtt_simon_client.h"

#define GPIO_LED_PIN 2
void task_mqtt_msg_in(void *args){
  // conf blink
  gpio_reset_pin(GPIO_LED_PIN);
  gpio_set_direction(GPIO_LED_PIN, GPIO_MODE_OUTPUT);
  
  mqtt_msg m;
  while (1)
  {
    if (xQueueReceive(queue_mqtt, &( m ), (TickType_t) 10 ) ) 
    {
      int msg_val = atoi(m.msg);
      while(msg_val-- > 0) {
          gpio_set_level(GPIO_LED_PIN, 1);
          vTaskDelay(500 / portTICK_PERIOD_MS);
          gpio_set_level(GPIO_LED_PIN, 0);
          vTaskDelay(500 / portTICK_PERIOD_MS);
        }
      strcpy(m.topic, "simon/resp");
      sprintf(m.msg, "%d", atoi(m.msg));
      mqtt_send_msg(&m);

    }
    
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
  
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    connect_wifi();
    init_touch_sensor();
    while (wifi_connected == 0)
    {
      vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    
    mqtt_app_start();
    xTaskCreate(&task_mqtt_msg_in, "task_mqtt_msg_in", 4096, NULL, 5, NULL);

}