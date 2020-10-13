/*
  https://www.RoxXxtar.com/bmc
  Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/
#ifndef RoxMux_h
#define RoxMux_h

#include <Arduino.h>

// i2c errors
#define ROX_I2C_ERROR_TOO_LONG  1
#define ROX_I2C_ERROR_NACK_ADDR 2
#define ROX_I2C_ERROR_NACK_DATA 3
#define ROX_I2C_ERROR_OTHER     4

#include "RoxDebug.h"
#include "RoxFlags.h"
#include "RoxTimer.h"
#include "RoxScroller.h"
#include "Rox74HC40XX.h"
#include "Rox74HC165.h"
#include "Rox74HC595.h"
#include "RoxVuMeter.h"
#include "RoxMCP2301X.h"
#include "RoxButton.h"
#include "RoxEncoder.h"
#include "RoxPot.h"

#endif
