// Copyright 2020, Ryan Wendland, usb64
// SPDX-License-Identifier: MIT

//#include "usb64_conf.h"
#include "tusb.h"
// #include "stm32f7xx_hal.h"
// #include "board.h"

void USB_IRQHandler(void)
{
    tuh_int_handler(0);
}

void usbh_dev_init()
{
    //Device specific usb host hardware init.
    //Look here for some rough guidance https://github.com/hathach/tinyusb/tree/master/hw/bsp
    //Normally need to init hardware registers and USB clocks and hook the device USB IRQ to `USB_IRQHandler`.
}
