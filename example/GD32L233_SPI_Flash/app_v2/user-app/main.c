/**
 * \file            main.c
 * \brief           main application
 */

/*
 * Copyright (c) 2024
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of mOTA - The Over-The-Air technology component for MCU.
 *
 * Author:          wade任
 * Change Logs:
 * Version  Date           Author       Notes
 * v1.0     2024-01-11     wade任       the first version
 */

/* Includes ------------------------------------------------------------------*/
#include "gd32l23x.h"
#include "gd32l233r_eval.h"
#include "systick.h"
#include <stdio.h>
#include "bootloader_config.h"
#include "common.h"

#define APP_VERSION_MAIN     0x02
#define APP_VERSION_SUB      0x00

/* USER CODE BEGIN PFP */
#define USING_BOOTLOADER

static void test_status_led_init(void);
static void led_flash(uint8_t times);
static void usart_config(void);

/* 固件更新的标志位，该标志位不能被清零 */
#if defined(__IS_COMPILER_ARM_COMPILER_5__)
    volatile uint64_t update_flag __attribute__((at(FIRMWARE_UPDATE_VAR_ADDR), zero_init));

#elif defined(__IS_COMPILER_ARM_COMPILER_6__)
    #define __INT_TO_STR(x)     #x
    #define INT_TO_STR(x)       __INT_TO_STR(x)
    volatile uint64_t update_flag __attribute__((section(".bss.ARM.__at_" INT_TO_STR(FIRMWARE_UPDATE_VAR_ADDR))));

#else
    #error "variable placement not supported for this compiler."
#endif

static inline void _APP_Reset(void)
{
    NVIC_SystemReset(); /* 复位重启 */
}

static void system_init()
{
#ifdef USING_BOOTLOADER
    extern int Image$$ER_IROM1$$Base;
    __disable_irq();
    SCB->VTOR = (uint32_t)&Image$$ER_IROM1$$Base;
    __enable_irq();
#endif /* USING_BOOTLOADER */
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t count = 0;

    /* initialize the LEDs */
    test_status_led_init();

    /* configure systick */
    systick_config();

    system_init();

    /* flash the LEDs for 2 time */
    led_flash(2);

    /* configure USART0 */
    usart_config();

    /* configure TAMPER key */
    gd_eval_key_init(KEY_TAMPER, KEY_MODE_GPIO);
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);

    /* wait for completion of USART transmission */
    while (RESET == usart_flag_get(USART0, USART_FLAG_TC)) {}

    while (1)
    {
        gpio_bit_set(GPIOC, GPIO_PIN_7);
        gpio_bit_set(GPIOC, GPIO_PIN_9);
        gpio_bit_set(GPIOC, GPIO_PIN_11);
        delay_1ms(500);
        if (count == 500)
        {
            count = 0;
        }
        printf("[%d] app v%d.%d\r\n", count++, APP_VERSION_MAIN, APP_VERSION_SUB);

        if (RESET == gd_eval_key_state_get(KEY_TAMPER))
        {
            delay_1ms(50);
            if (RESET == gd_eval_key_state_get(KEY_TAMPER))
            {
                /* turn on LED2 */
                gd_eval_led_on(LED2);
                update_flag = FIRMWARE_UPDATE_MAGIC_WORD;
                printf("\r\n update firmware \r\n");
                _APP_Reset();
            }
            else
            {
                /* turn off LED2 */
                gd_eval_led_off(LED2);
            }
        }

        else if (RESET == gd_eval_key_state_get(KEY_WAKEUP))
        {
            delay_1ms(50);
            if (RESET == gd_eval_key_state_get(KEY_WAKEUP))
            {
                update_flag = FIRMWARE_RECOVERY_MAGIC_WORD;
                printf("\r\n firmware recovery \r\n");
                _APP_Reset();
            }
        }
        else
        {
            gd_eval_led_off(LED2);
        }
    }
}

/*!
    \brief      test status led initialize
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void test_status_led_init(void)
{
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_init(LED4);
}

/*!
    \brief      flash the LED for test
    \param[in]  times: times to flash the LEDs
    \param[out] none
    \retval     none
*/
static void led_flash(uint8_t times)
{
    uint8_t i;
    for (i = 0; i < times; i++)
    {
        /* delay 400 ms */
        // delay_1ms(400);

        /* turn on LEDs */
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);
        gd_eval_led_on(LED3);
        gd_eval_led_on(LED4);

        /* delay 400 ms */
        // delay_1ms(400);
        /* turn off LEDs */
        gd_eval_led_off(LED1);
        gd_eval_led_off(LED2);
        gd_eval_led_off(LED3);
        gd_eval_led_off(LED4);
    }
}

/*!
    \brief      usart configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usart_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USART0 TX */
    gpio_af_set(GPIOA, EVAL_COM_AF, GPIO_PIN_9);

    /* connect port to USART0 RX */
    gpio_af_set(GPIOA, EVAL_COM_AF, GPIO_PIN_10);

    /* configure USART TX as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);

    /* configure USART RX as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    // usart_word_length_set(USART0, USART_WL_8BIT);
    // usart_stop_bit_set(USART0, USART_STB_1BIT);
    // usart_parity_config(USART0, USART_PM_NONE);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    /* 检查是否发送完毕 */
    while (usart_flag_get(USART0, USART_FLAG_TC) == RESET) {}
    return (ch);
    // while (RESET == usart_flag_get(USART0, USART_FLAG_TBE)) {}
    // return ch;
}
