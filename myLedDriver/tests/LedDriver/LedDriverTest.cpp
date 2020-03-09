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

