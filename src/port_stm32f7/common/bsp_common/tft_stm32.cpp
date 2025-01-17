// Copyright 2020, Ryan Wendland, usb64
// SPDX-License-Identifier: MIT

#include "common.h"
#include "tft.h"
#include "controller_icon.h"
#include "usb64_logo.h"
#include "GuiLite.h"

#define TFT_FRAMEBUFFER_SIZE (TFT_WIDTH * TFT_HEIGHT * TFT_PIXEL_SIZE)
c_surface *psurface_guilite = NULL;
c_display *pdisplay_guilite = NULL;
static uint8_t *_framebuffer = (uint8_t *)(LCD_FB_START_ADDRESS);
struct EXTERNAL_GFX_OP my_gfx_op;
float scale = 1;

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
    BSP_LCD_SetTextColor(rgb);
    BSP_LCD_FillRect(x * scale, y * scale, scale + 1, scale + 1);
}

static void _fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb)
{
    BSP_LCD_SetTextColor(rgb);
    BSP_LCD_FillRect(x0 * scale, y0 * scale, (x1 - x0) * scale, (y1 - y0) * scale);
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
    //Always draw into a 320x240 Framebuffer on stm32
    static c_surface_no_fb surface(320, 240, 2, &my_gfx_op, Z_ORDER_LEVEL_0);
    static c_display display(NULL, 320, 240, &surface);
    scale = MIN(TFT_HEIGHT / 240.0f, TFT_WIDTH / 320.0f);
    my_gfx_op.draw_pixel = _draw_pixel;
    my_gfx_op.fill_rect = _fill_rect;
    psurface_guilite = &surface;
    pdisplay_guilite = &display;
    register_debug_function(_tft_assert, _tft_log_out);
#if (ENABLE_TFT_DISPLAY >= 1)
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, (uint32_t)_framebuffer);
    BSP_LCD_SelectLayer(0);
    BSP_LCD_SetBackColor(TFT_BG_COLOR);
    BSP_LCD_Clear(TFT_BG_COLOR);
    BSP_LCD_DisplayOn();
#endif
}

bool tft_dev_is_busy()
{
    return 0;
}
