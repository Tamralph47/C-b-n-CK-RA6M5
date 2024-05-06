/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "hal_data.h"
#include "stdio.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);

void user_uart_callback(uart_callback_args_t *p_args);
void console_write(const char *buffer);

static volatile bool is_transfer_complete = false;

void hs300x_callback(rm_hs300x_callback_args_t * p_args);
static volatile bool hs300x_complete = false;

extern bsp_leds_t g_bsp_leds;

/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
#if BSP_TZ_SECURE_BUILD

    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif

    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

    /* Set the blink frequency (must be <= bsp_delay_units */
    const uint32_t freq_in_hz = 2;

    /* Calculate the delay in terms of bsp_delay_units */
    const uint32_t delay = bsp_delay_units / freq_in_hz;

    /* LED type structure */
    bsp_leds_t leds = g_bsp_leds;

    /* If this board has no LEDs then trap here */
    if (0 == leds.led_count)
    {
        while (1)
        {
            ;                          // There are no LEDs on this board
        }
    }

    /* Holds level to set for pins */
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;

    char write_buffer[200] = {};

    fsp_err_t            err = FSP_SUCCESS;
    rm_hs300x_raw_data_t hs300x_raw_data;
    rm_hs300x_data_t     hs300x_data;
    uint8_t              calculated_flag = 0;

    /*Initialize UART */
    R_SCI_UART_Open(&g_console_write_ctrl, &g_console_write_cfg);
    sprintf(write_buffer, "hello world!!\r\n");

    /* Open the I2C bus if it is not already open. */
        rm_comms_i2c_bus_extended_cfg_t * p_extend_hs300x =
            (rm_comms_i2c_bus_extended_cfg_t *) g_hs300x_sensor0_cfg.p_instance->p_cfg->p_extend;
        i2c_master_instance_t * p_driver_instance_hs300x = (i2c_master_instance_t *) p_extend_hs300x->p_driver_instance;
        p_driver_instance_hs300x->p_api->open(p_driver_instance_hs300x->p_ctrl, p_driver_instance_hs300x->p_cfg);

        /* Initalize HS300x sensor */
        RM_HS300X_Open(&g_hs300x_sensor0_ctrl,&g_hs300x_sensor0_cfg);

    while (1)
    {
        /* Send clear screen message & cursor-home command */
        sprintf(write_buffer, "\x1b[H");
        console_write(write_buffer);

        /* Enable access to the PFS registers. If using r_ioport module then register protection is automatically
         * handled. This code uses BSP IO functions to show how it is used.
         */
        R_BSP_PinAccessEnable();

        /* Update all board LEDs */
        for (uint32_t i = 0; i < leds.led_count; i++)
        {
            /* Get pin to toggle */
            uint32_t pin = leds.p_leds[i];

            /* Write to this pin */
            R_BSP_PinWrite((bsp_io_port_pin_t) pin, pin_level);
        }

        /* Protect PFS registers */
        R_BSP_PinAccessDisable();

        /* Toggle level for next write */
        if (BSP_IO_LEVEL_LOW == pin_level)
        {
            pin_level = BSP_IO_LEVEL_HIGH;
        }
        else
        {
            pin_level = BSP_IO_LEVEL_LOW;
        }

        hs300x_complete = false;

        /* Start Measurement */
        RM_HS300X_MeasurementStart(&g_hs300x_sensor0_ctrl);
        while (false == hs300x_complete)
        {
            /* Wait callback */
        }
        do
        {
            hs300x_complete = false;
            /* Read ADC Data from HS300X */
            RM_HS300X_Read(&g_hs300x_sensor0_ctrl, &hs300x_raw_data);
            while (false == hs300x_complete)
            {
                /* Wait callback */
            }
            /* Calculate Humidity and Temperatuere values from ADC data */
            err = RM_HS300X_DataCalculate(&g_hs300x_sensor0_ctrl, &hs300x_raw_data, &hs300x_data);
            if (FSP_SUCCESS == err)
            {
                calculated_flag = 1;
            }
            else if (FSP_ERR_SENSOR_INVALID_DATA == err)
            {
                /* Stale data */
                calculated_flag = 0;
            }
            else
            {
                /* Do nothing */
            }
        } while (0 == calculated_flag);

        sprintf(write_buffer, "Humidity value: %d.%d \n\r", hs300x_data.humidity.integer_part, hs300x_data.humidity.decimal_part);
        console_write(write_buffer);

        sprintf(write_buffer, "Temperature value: %d.%d \n\r", hs300x_data.temperature.integer_part, hs300x_data.temperature.decimal_part);
        console_write(write_buffer);

        /* Delay */
        R_BSP_SoftwareDelay(delay, bsp_delay_units);
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        IOPORT_CFG_OPEN(&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);
    }
}

void user_uart_callback(uart_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        case UART_EVENT_TX_COMPLETE:
        {
            is_transfer_complete = true;
            break;
        }
        default:{
            /* Do nothing*/
        }
    }
}

void console_write(const char *buffer)
{
    is_transfer_complete = false;
    R_SCI_UART_Write(&g_console_write_ctrl, (uint8_t *) buffer, strlen(buffer));

    while (!is_transfer_complete)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    }
}

void hs300x_callback(rm_hs300x_callback_args_t * p_args)
{
    switch (p_args->event)
    {
        case RM_HS300X_EVENT_SUCCESS:
        {
            hs300x_complete = true;
            break;
        }
        default:
        {
            /* Do nothing */
        }

    }
}
