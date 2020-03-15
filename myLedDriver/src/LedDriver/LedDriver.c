/***
 * Excerpted from "Test-Driven Development for Embedded C",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material, 
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose. 
 * Visit http://www.pragmaticprogrammer.com/titles/jgade for more book information.
***/
/*- ------------------------------------------------------------------ -*/
/*-    Copyright (c) James W. Grenning -- All Rights Reserved          -*/
/*-    For use by owners of Test-Driven Development for Embedded C,    -*/
/*-    and attendees of Renaissance Software Consulting, Co. training  -*/
/*-    classes.                                                        -*/
/*-                                                                    -*/
/*-    Available at http://pragprog.com/titles/jgade/                  -*/
/*-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3                -*/
/*-                                                                    -*/
/*-    Authorized users may use this source code in your own           -*/
/*-    projects, however the source code may not be used to            -*/
/*-    create training material, courses, books, articles, and         -*/
/*-    the like. We make no guarantees that this source code is        -*/
/*-    fit for any purpose.                                            -*/
/*-                                                                    -*/
/*-    www.renaissancesoftware.net james@renaissancesoftware.net       -*/
/*- ------------------------------------------------------------------ -*/

#include "LedDriver.h"
#include "RuntimeError.h"
#include <stdlib.h>
#include <memory.h>

enum {ALL_LEDS_ON=~0, ALL_LEDS_OFF=~ALL_LEDS_ON};
enum {LED_FIRST=1, LED_LAST=16};

uint16_t *_pLedAddress = 0;
uint16_t _ledImage = 0;

static uint16_t _convertLedNumberToBit(uint16_t ledNumber)
{
    return 1 << (ledNumber-1);
}

static bool _isLedInBounds(uint16_t ledNumber)
{
    bool returnVal;
    if( ledNumber>=LED_FIRST && ledNumber<=LED_LAST )
    {
        returnVal = true;
    }
    else
    {
        returnVal = false;
        RUNTIME_ERROR("LED Driver: out-of-bounds LED", ledNumber);
    }

    return returnVal;
}

static void _setLedImageBit(uint16_t ledNumber)
{
    _ledImage |= _convertLedNumberToBit(ledNumber);
}

static void _clearLedImageBit(uint16_t ledNumber)
{
    _ledImage &= ~_convertLedNumberToBit(ledNumber);
}

static void _updateHardware(void)
{
    *_pLedAddress = _ledImage;
}

void LedDriver_Create(uint16_t *address)
{
    _pLedAddress = address;
    _ledImage = ALL_LEDS_OFF;
    _updateHardware();
}

void LedDriver_Destroy(void)
{
}

void LedDriver_TurnOn(uint16_t ledNumber)
{
    if( _isLedInBounds(ledNumber) )
    {
        _setLedImageBit(ledNumber);
        _updateHardware();
    }
}

void LedDriver_TurnOff(uint16_t ledNumber)
{
    if( _isLedInBounds(ledNumber) )
    {
        _clearLedImageBit(ledNumber);
        _updateHardware();
    }
}

void LedDriver_TurnAllOn(void)
{
    _ledImage = ALL_LEDS_ON;
    _updateHardware();
}

bool LedDriver_IsOn(uint16_t ledNumber)
{
    bool returnVal = false;
    if( _isLedInBounds(ledNumber) )
    {
        returnVal = (_ledImage & _convertLedNumberToBit(ledNumber));
    }
    return returnVal;
}

bool LedDriver_IsOff(uint16_t ledNumber)
{
    bool returnVal = true;
    if( _isLedInBounds(ledNumber) )
    {
        returnVal = (~_ledImage & _convertLedNumberToBit(ledNumber));
    }

    returnVal = returnVal;

    return returnVal;
}
