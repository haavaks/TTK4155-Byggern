#pragma once

#include "sam.h"
#include <inttypes.h>

void timer_init();

void timer_start();

void timer_stop();

uint32_t timer_value();
