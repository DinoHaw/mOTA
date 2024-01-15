/**
 * \file            bsp_debug_drv.c
 * \brief           debug print port drive
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
#include "bsp_debug_drv.h"

static void bsp_debug_gpio_config(void)
{
    /* 配置 USART TX 引脚 */
    rcu_periph_clock_enable(COM_DEBUG_TX_CLK);
    gpio_af_set(COM_DEBUG_TX_PORT, GPIO_AF_7, COM_DEBUG_TX_PIN);
    gpio_mode_set(COM_DEBUG_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_DEBUG_TX_PIN);
    gpio_output_options_set(COM_DEBUG_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, COM_DEBUG_TX_PIN);

    /* 配置 USART RX 引脚 */
    rcu_periph_clock_enable(COM_DEBUG_RX_CLK);
    gpio_af_set(COM_DEBUG_RX_PORT, GPIO_AF_7, COM_DEBUG_RX_PIN);
    gpio_mode_set(COM_DEBUG_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_DEBUG_RX_PIN);
    gpio_output_options_set(COM_DEBUG_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, COM_DEBUG_RX_PIN);

}

static void bsp_debug_usart_config(void)
{
    /* 配置 USART 功能 */
    rcu_periph_clock_enable(COM_DEBUG_CLK);
    
    usart_deinit(COM_DEBUG_UARTx);
    usart_baudrate_set(COM_DEBUG_UARTx, 115200U);
    usart_receive_config(COM_DEBUG_UARTx, USART_RECEIVE_ENABLE);
    usart_transmit_config(COM_DEBUG_UARTx, USART_TRANSMIT_ENABLE);
    
    // usart_receive_fifo_enable(COM_DEBUG_UARTx);
    
    // usart_receiver_timeout_threshold_config(COM_DEBUG_UARTx, 9600);
    // usart_receiver_timeout_enable(COM_DEBUG_UARTx);          /* 使能接收超时 */
    
    // usart_reception_error_dma_disable(COM_DEBUG_UARTx);	    /* 接收错误时禁止 DMA */
    
    usart_enable(COM_DEBUG_UARTx);
    // usart_interrupt_enable(COM_DEBUG_UARTx, USART_INT_RT);	/* 接收超时中断 */
    
    // usart_interrupt_enable(COM_DEBUG_UARTx, USART_INT_IDLE);	/* 空闲中断使能 */
    
    // usart_interrupt_enable(COM_DEBUG_UARTx, USART_INT_RBNE); /* 接收中断使能 */
    // nvic_irq_enable(USART0_IRQn,2);
    
    // usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
}

/* DEBUG 模块硬件初始化 */
void bsp_debug_hardware_init(void)
{
    bsp_debug_gpio_config();
    bsp_debug_usart_config();
}

/* DEBUG 发送一个字节 */
void bsp_debug_send_byte(uint8_t ch)
{
    /* 发送一个字节数据到 UART */
    usart_data_transmit(COM_DEBUG_UARTx, (uint8_t) ch);
    /* 等待发送数据寄存器为空 */
    while (usart_flag_get(COM_DEBUG_UARTx, USART_FLAG_TC) == RESET) {}
}

/* DEBUG 发送字符串 */
void bsp_debug_send_string(char *str, uint32_t cnt)
{
    while(cnt--)
    {
        // printf("\r\n 向DEBUG发送 :%c  ",*str);
        usart_data_transmit(COM_DEBUG_UARTx, *str++ );
        /* 等待发送完成 */
        /* TC: 发送完成 TBE: 发送数据寄存器空 */
        while (usart_flag_get(COM_DEBUG_UARTx, USART_FLAG_TC) == RESET) {}
    }
}