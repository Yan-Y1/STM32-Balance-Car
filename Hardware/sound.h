#ifndef __SOUND_H 
#define __SOUND_H

#include "stm32f10x.h"                  // Device header

//³õÊ¼»¯·äÃùÆ÷
void Sound_Init(void);

//·äÃùÆ÷Ïì
void Sound_Open(void);

//·äÃùÆ÷²»Ïì
void Sound_Close(void);

#endif
