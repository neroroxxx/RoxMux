/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/
#ifndef RoxMux_h
#define RoxMux_h

#include <Arduino.h>

//RoxLed Modes
#define ROX_DEFAULT 0
#define ROX_BLINK 1
#define ROX_PULSE 2

// i2c error definitions
#define ROX_I2C_ERROR_TOO_LONG  1
#define ROX_I2C_ERROR_NACK_ADDR 2
#define ROX_I2C_ERROR_NACK_DATA 3
#define ROX_I2C_ERROR_OTHER     4

// utilities
#include "RoxDebug.h"
#include "RoxFlags.h"
#include "RoxTimer.h"
#include "RoxIntervals.h"
#include "RoxScroller.h"
// muxes
#include "Rox74HC40XX.h"
#include "Rox74HC165.h"
#include "Rox74HC595.h"
#include "RoxTCA9548A.h"
#include "RoxMCP2301X.h"
// hardware handlers
#include "RoxButton.h"
#include "RoxEncoder.h"
#include "RoxPot.h"
//#include "RoxLed.h"
#include "RoxVuMeter.h"

#endif
