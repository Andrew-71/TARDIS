#include <stdio.h>
#include <pico/stdlib.h>

#include "pico/time.h"
#include "hardware/pwm.h"

#include "lib/GUI/GUI_Paint.h"
#include "lib/EPD_1in54_V2/EPD_1in54_V2.h"

#define PIN_LED 25

#define R_PIN 20
#define G_PIN 19
#define B_PIN 18


void blink(UBYTE amount, UDOUBLE time)
{
    UBYTE i = 0;
    while (i < amount)
    {
        sleep_ms(time);
        gpio_put(PIN_LED, 1);
        sleep_ms(time);
        gpio_put(PIN_LED, 0);
        i++;
    }
}

int main()
{
    stdio_init_all();
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);

    gpio_set_function(R_PIN, GPIO_FUNC_PWM);
    gpio_set_function(G_PIN, GPIO_FUNC_PWM);
    gpio_set_function(B_PIN, GPIO_FUNC_PWM);

    blink(2, 150);

    // Init EPD //
    Init_Device();
    EPD_1IN54_V2_Init();
    EPD_1IN54_V2_Clear();

    // Create new image cache
    UBYTE *ImageBuffer;
    UWORD Imagesize = ((EPD_1IN54_V2_WIDTH % 8 == 0) ? (EPD_1IN54_V2_WIDTH / 8) : (EPD_1IN54_V2_WIDTH / 8 + 1)) * EPD_1IN54_V2_HEIGHT;
    if ((ImageBuffer = (UBYTE *)malloc(Imagesize)) == NULL)
    {
        printf("Failed to apply for memory...\n");
    }
    Paint_NewImage(ImageBuffer, EPD_1IN54_V2_WIDTH, EPD_1IN54_V2_HEIGHT, 270, WHITE);
    Paint_SelectImage(ImageBuffer);

    blink(1, 250);

    // Edit image and display it

    Paint_Clear(WHITE);

    Paint_DrawString_EN(12, 5, "POLICE TELEPHONE", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(66, 17, "FREE", &Font24, BLACK, WHITE);
    Paint_DrawString_EN(30, 34, "FOR USE OF", &Font20, BLACK, WHITE);
    Paint_DrawString_EN(49, 51, "PUBLIC", &Font24, BLACK, WHITE);

    Paint_DrawString_EN(53, 75, "ADVICE & ASSISTANCE", &Font8, BLACK, WHITE);
    Paint_DrawString_EN(53, 85, "OBTAINABLE IMMEDIATELY", &Font8, BLACK, WHITE);



    Paint_DrawString_EN(17, (200 - 20 * 3), "OFFICERS & CARS", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(30, (200 - 20 * 2), "RESPOND TO ALL CALLS", &Font12, BLACK, WHITE);
    Paint_DrawString_EN(16, (200 - 20 * 1), "PULL TO OPEN", &Font20, BLACK, WHITE);

    //Paint_DrawNum(10, 25, 1337, &Font20, BLACK, WHITE);

    EPD_1IN54_V2_Display(ImageBuffer);

    sleep_ms(5000);


    // Exit //
    EPD_1IN54_V2_Init();
    EPD_1IN54_V2_Clear();
    EPD_1IN54_V2_Sleep();

    free(ImageBuffer);

    //sleep_ms(2000);
    //blink(2, 150);

    const uint LED_PIN = 18;
    const uint MAX_PWM_LEVEL = 65535;

    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);

    uint sliceNum = pwm_gpio_to_slice_num(LED_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_init(sliceNum, &config, true);

    int level = 0;
    bool up = true;
    while (true) {
        pwm_set_gpio_level(LED_PIN, level);

        if (level == 0) {
            up = true;
        } else if (level == MAX_PWM_LEVEL) {
            up = false;
        }
        level = up ? level + 1 : level - 1;

        sleep_us(50);
    }
}