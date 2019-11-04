#ifndef __GUIFONT_CREATE_H
#define __GUIFONT_CREATE_H

#include "GUI.h"
#include "stm32f4xx.h"

extern GUI_FONT FONT_TTF_24;
extern GUI_FONT FONT_TTF_48;
extern GUI_FONT FONT_TTF_72;
extern GUI_FONT FONT_TTF_96;
extern GUI_FONT FONT_TTF_120;

void Create_TTF_Font(void);

#endif
