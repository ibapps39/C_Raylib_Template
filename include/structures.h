#pragma once
#include "stdio.h"
#include "common.h"

char* INFO_LABEL = "";
int INFO_NUM = 0;
float INFO_NUMF = 0.0f;
double INFO_NUMD = 0.00;
char* INFO_TEXT = "";

void DRAW_WHATEVER()
{
    const char* (*INFO_MSG)(char*, float) = TextFormat("%s : %.2f", INFO_LABEL, INFO_NUM);
    DrawText(INFO_MSG, 0, 0, 20, WHITE);
}