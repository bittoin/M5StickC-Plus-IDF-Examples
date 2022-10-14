#pragma once

#include <stdio.h>

/*
    ************************************
    Definição de variáveis para o SAC-DM
    ************************************
*/
#define SAMPLE_SIZE 500

static float threshold = 1.20;
static long lastMsg = 0, loopTimer = 0;
static int value = 0, readings = 0, peaks_x = 0, peaks_y = 0, peaks_z = 0;
static double rho_x, rho_y, rho_z;

static int16_t signals_x[3] = {0, 0, 0};
static int16_t signals_y[3] = {0, 0, 0};
static int16_t signals_z[3] = {0, 0, 0};

static int16_t accX = 0;
static int16_t accY = 0;
static int16_t accZ = 0;

void func(void);
