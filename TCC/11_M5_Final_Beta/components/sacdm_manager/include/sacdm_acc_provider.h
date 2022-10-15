#pragma once

#include <stdio.h>
#include "mpu6886.h"

typedef struct {
    int accX;
    int accY;
    int accZ;
} raw_acc_val;

void sacdm_acc_provider_init(void);
void sacdm_acc_provider_read(raw_acc_val* acc_val);
