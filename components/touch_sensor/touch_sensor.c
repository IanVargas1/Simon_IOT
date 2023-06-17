#include "touch_sensor.h"

/*
  Read values sensed at all available touch pads.
 Print out values in a loop on a serial monitor.
 */
static void tp_read_task(void *pvParameter)
{
    uint16_t touch_value;
    uint16_t touch_filter_value;

    printf("Touch Sensor filter mode read, the output format is: \nTouchpad num:[raw data, filtered data]\n\n");

    while (1) {
        touch_pad_read_raw_data(0, &touch_value);
        touch_pad_read_filtered(0, &touch_filter_value);
        printf("T0:[%4"PRIu16",%4"PRIu16"] ", touch_value, touch_filter_value);

        // touch_pad_read_raw_data(8, &touch_value);
        // touch_pad_read_filtered(8, &touch_filter_value);
        // printf("T8:[%4"PRIu16",%4"PRIu16"] ", touch_value, touch_filter_value);

        // touch_pad_read_raw_data(3, &touch_value);
        // touch_pad_read_filtered(3, &touch_filter_value);
        // printf("T3:[%4"PRIu16",%4"PRIu16"] ", touch_value, touch_filter_value);

        // touch_pad_read_raw_data(4, &touch_value);
        // touch_pad_read_filtered(4, &touch_filter_value);
        // printf("T4:[%4"PRIu16",%4"PRIu16"] ", touch_value, touch_filter_value);

        printf("\n");
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

static void tp_touch_pad_init(void)
{
   
    touch_pad_config(0, TOUCH_THRESH_NO_USE);
    // touch_pad_config(8, TOUCH_THRESH_NO_USE);
    // touch_pad_config(3, TOUCH_THRESH_NO_USE);
    // touch_pad_config(4, TOUCH_THRESH_NO_USE);
}

void init_touch_sensor(void)
{

    ESP_ERROR_CHECK(touch_pad_init());

    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    tp_touch_pad_init();

    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);

    // Start task to read values sensed by pads
    xTaskCreate(&tp_read_task, "touch_pad_read_task", 4096, NULL, 5, NULL);
}