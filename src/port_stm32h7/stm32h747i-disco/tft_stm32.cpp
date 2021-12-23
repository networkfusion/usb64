// Copyright 2020, Ryan Wendland, usb64
// SPDX-License-Identifier: MIT

#include "usb64_common.h"
#include "tft.h"
#include "controller_icon.h"
#include "usb64_logo.h"
#include "GuiLite.h"
#include "stm32_lcd.h" //For util functinos.

#define TFT_FRAMEBUFFER_SIZE (TFT_WIDTH * TFT_HEIGHT * TFT_PIXEL_SIZE)
c_surface *psurface_guilite = NULL;
c_display *pdisplay_guilite = NULL;
// #if TFT_USE_FRAMEBUFFER
// static uint8_t *_framebuffer = (uint8_t *)(LCD_FB_START_ADDRESS);
// #else
// static uint8_t *_framebuffer = (uint8_t *)(LCD_FB_START_ADDRESS);
// struct EXTERNAL_GFX_OP my_gfx_op;
// #endif

static void _tft_assert(const char *file, int line)
{
    debug_print_error("[TFT] Error: Assert in %s on line %d\n", file, line);
    while (1)
        ;
}

static void _tft_log_out(const char *log)
{
    debug_print_status(log);
}

#if TFT_USE_FRAMEBUFFER == 0
static void _draw_pixel(int x, int y, unsigned int rgb)
{
    //Device specific pixel draw
    UTIL_LCD_SetPixel(x, y, rgb);
}

static void _fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb)
{
    UTIL_LCD_SetTextColor(rgb);
    UTIL_LCD_FillRect(x0, y0, x1 - x0, y1 - y0, 0); // black
    //Weird, but the above FillRect leaves before finshing properly. This fixes it?
    UTIL_LCD_FillRect(x0, y0, 1, 1, 0); //black
}
#endif

void tft_dev_draw(bool force)
{
    //Dont need to do anything. This TFT just updates from the SDRAM buffer automatically.
}

extern "C" void BSP_LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params)
{
    //Overrise the internal clock config as it messes with other clock divs and multipliers.
    //Dont do anything. All clocks are setup properly at boot
}

void tft_dev_init()
{
#if TFT_USE_FRAMEBUFFER
    static c_surface surface(TFT_WIDTH, TFT_HEIGHT, 2, Z_ORDER_LEVEL_0);
    static c_display display(_framebuffer, TFT_WIDTH, TFT_HEIGHT, &surface);
    psurface_guilite = &surface;
    pdisplay_guilite = &display;
#else
    static c_surface_no_fb surface(TFT_WIDTH, TFT_HEIGHT, 2, &my_gfx_op, Z_ORDER_LEVEL_0);
    static c_display display(NULL, TFT_WIDTH, TFT_HEIGHT, &surface);
    my_gfx_op.draw_pixel = _draw_pixel;
    my_gfx_op.fill_rect = _fill_rect;
#endif
    psurface_guilite = &surface;
    pdisplay_guilite = &display;
    register_debug_function(_tft_assert, _tft_log_out);
#if (ENABLE_TFT_DISPLAY >= 1)
    // /* LTDC, DSI initialization and LCD screen initialization */
    // lcd_status = BSP_LCD_Init(0,LCD_ORIENTATION_LANDSCAPE);
    // BSP_LCD_SetLayerAddress(0,0, LCD_FRAME_BUFFER);
    // /* Get the LCD width and height */
    // BSP_LCD_GetXSize(0,&LCD_X_Size);
    // BSP_LCD_GetYSize(0,&LCD_Y_Size);

    /* Initialize the LCD */
    BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
    UTIL_LCD_SetFuncDriver(&LCD_Driver);
    UTIL_LCD_SetLayer(0); //TODO: FIXME: was BSP_LCD_LayerDefaultInit(0, (uint32_t)_framebuffer);
    UTIL_LCD_Clear(0);
    UTIL_LCD_SetBackColor(TFT_BG_COLOR);
    UTIL_LCD_Clear(TFT_BG_COLOR);
    BSP_LCD_DisplayOn(0);
#endif
}

bool tft_dev_is_busy()
{
    return 0;
}
