//- ------------------------------------------------------------------
//-    Copyright (c) James W. Grenning -- All Rights Reserved         
//-    For use by owners of Test-Driven Development for Embedded C,   
//-    and attendees of Renaissance Software Consulting, Co. training 
//-    classes.                                                       
//-                                                                   
//-    Available at http://pragprog.com/titles/jgade/                 
//-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3               
//-                                                                   
//-    Authorized users may use this source code in your own          
//-    projects, however the source code may not be used to           
//-    create training material, courses, books, articles, and        
//-    the like. We make no guarantees that this source code is       
//-    fit for any purpose.                                           
//-                                                                   
//-    www.renaissancesoftware.net james@renaissancesoftware.net      
//- ------------------------------------------------------------------

extern "C"
{
#include "RuntimeErrorStub.h"
#include "LedDriver.h"
}

#include "CppUTest/TestHarness.h"


TEST_GROUP(LedDriver)
{
    uint16_t virtualLeds=0xFFFF;
    void setup()
    {
      LedDriver_Create(&virtualLeds);
    }

    void teardown()
    {
       LedDriver_Destroy();
    }
};

TEST(LedDriver, LedsOffAfterCreate)
{
    virtualLeds=0xFFFF;
    LedDriver_Create(&virtualLeds);
    CHECK_EQUAL(0, virtualLeds);
}

TEST(LedDriver, TurnOnLedOne)
{
    LedDriver_TurnOn(1);
    CHECK_EQUAL(1, virtualLeds);
}

TEST(LedDriver, TurnOffLedOne)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);
    CHECK_EQUAL(0, virtualLeds);
}

TEST(LedDriver, TurnOnMultipleLeds)
{
    LedDriver_TurnOn(9);
    LedDriver_TurnOn(8);
    CHECK_EQUAL(0x180, virtualLeds);
}

TEST(LedDriver, AllOn)
{
    LedDriver_TurnAllOn();
    CHECK_EQUAL(0xFFFF, virtualLeds);
}

TEST(LedDriver, TurnOffAnyLed)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(8);
    CHECK_EQUAL(0xFF7F, virtualLeds);
}

TEST(LedDriver, ledMemoryIsNotReadable)
{
    virtualLeds = 0xFFFF;
    LedDriver_TurnOn(8);
    CHECK_EQUAL(0x80, virtualLeds);
}

TEST(LedDriver, UpperAndLowerBounds)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOn(16);
    CHECK_EQUAL(0x8001, virtualLeds);
}

TEST(LedDriver, OutOfBoundsTurnOnChangesNothing)
{
    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(17);
    LedDriver_TurnOn(33);
    LedDriver_TurnOn(3141);
    CHECK_EQUAL(0, virtualLeds);
}

TEST(LedDriver, OutOfBoundsTurnOffChangesNothing)
{
    LedDriver_TurnAllOn();
    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(17);
    LedDriver_TurnOff(33);
    LedDriver_TurnOff(3141);
    CHECK_EQUAL(0xFFFF, virtualLeds);
}

IGNORE_TEST(LedDriver, OutOfBoundsToDo)
{
    /* TODO: what should we do during runtime? */
}

TEST(LedDriver, OutOfBoundsProducesRuntimeError)
{
    LedDriver_TurnOn(0);
    STRCMP_EQUAL("LED Driver: out-of-bounds LED", RuntimeErrorStub_GetLastError());
    CHECK_EQUAL(0, RuntimeErrorStub_GetLastParameter());

    LedDriver_TurnOn(17);
    STRCMP_EQUAL("LED Driver: out-of-bounds LED", RuntimeErrorStub_GetLastError());
    CHECK_EQUAL(17, RuntimeErrorStub_GetLastParameter());

    LedDriver_TurnOff(65535);
    STRCMP_EQUAL("LED Driver: out-of-bounds LED", RuntimeErrorStub_GetLastError());
    CHECK_EQUAL(65535, RuntimeErrorStub_GetLastParameter());

    LedDriver_TurnOff(-5);
    STRCMP_EQUAL("LED Driver: out-of-bounds LED", RuntimeErrorStub_GetLastError());
    CHECK_EQUAL(-5, (int16_t)RuntimeErrorStub_GetLastParameter()); //casted, since LedDriver_TurnOn() parameter is uint16_t
}

TEST(LedDriver, IsOn)
{
    CHECK_FALSE(LedDriver_IsOn(11));
    LedDriver_TurnOn(11);
    CHECK_TRUE(LedDriver_IsOn(11));
}

TEST(LedDriver, IsOff)
{
    CHECK_TRUE(LedDriver_IsOff(12));
    LedDriver_TurnOn(12);
    CHECK_FALSE(LedDriver_IsOff(12));
}

TEST(LedDriver, OutOfBoundsLedsAreAlwaysOff)
{
    CHECK_FALSE(LedDriver_IsOn(0));
    CHECK_FALSE(LedDriver_IsOn(17));
    CHECK_TRUE(LedDriver_IsOff(0));
    CHECK_TRUE(LedDriver_IsOff(17));
}

