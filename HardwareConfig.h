/*
 * HardwareConfig.h
 *
 *  Created on: 09.02.2017
 *      Author: cybaer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

#include <stdio.h>
#include "avrlib/base.h"
#include "avrlib/gpio.h"
#include "avrlib/adc.h"
#include "avrlib/spi.h"
#include "avrlib/devices/mcp23s08.h"
#include "avrlib/devices/switch.h"
#include "avrlib/devices/rotary_encoder.h"



using namespace avrlib;
typedef Gpio<PortD, 2> Debug;
typedef Gpio<PortB, 2> CS1;
typedef Gpio<PortB, 1> CS2;

// HW SPI
static const uint8_t SPI_Speed = 32;
typedef SpiMasterBase<MSB_FIRST, SPI_Speed> spi_base;
typedef SpiMaster<CS1, MSB_FIRST, SPI_Speed> spi_Display;
typedef MCP23S08<spi_base, CS2, 0> portExtender;

typedef DebouncedSwitch<PortPin<portExtender, 1> > Switch_1;
typedef DebouncedSwitch<PortPin<portExtender, 2> > Switch_2;

typedef RotaryEncoder<PortPin<portExtender, 6>, PortPin<portExtender, 7>, PortPin<portExtender, 3> > Encoder;


  template<typename SPI>
  class C7Segment
  {
  public:

    static void reset() { SPI::Write('v'); }
    static void setDim(uint8_t dim)
    {
      SPI::Write(0x7A); // Command byte
      SPI::Write(dim);    // Dim display
    }
    static void printDec(int val)
    {
      if(val > 9999) return;
      char buf[12];
      sprintf(buf, "%4d", val);
      SPI::Write(buf[0]);
      SPI::Write(buf[1]);
      SPI::Write(buf[2]);
      SPI::Write(buf[3]);
    }
    static void printText(const char *t)
    {
      char buf[5];
      for(int i=0; i<4; i++)
      {
        buf[i] = t[i];
      }
      buf[4] = '\0';
      SPI::Write(buf[0]);
      SPI::Write(buf[1]);
      SPI::Write(buf[2]);
      SPI::Write(buf[3]);
    }
    /*void printScrollText(const char *t)
    {
      m_Scrolling = true;
      for(int i=0; i<20; i++)
      {
        m_ScrollText[i] = t[i];
      }
      m_ScrollText[20] = '\0';
      m_Pos = 0;
      scroll();
    }
    void scroll(bool bb = true)
    {
      if(m_Scrolling && bb)
      {
        printText(&m_ScrollText[m_Pos]);
        if(++m_Pos >13)
          m_Pos = 0;
      }
      else
      {
        m_Scrolling = false;
        m_Pos = 0;
      }
    }
    void printBeat()
    {
      if(m_Beat)
      {
        m_Beat = false;
        sendDecimalPointControl(1 << m_Bit);
        if(++m_Bit > 3)
          m_Bit = 0;
      }
    }

    void toggleColon()
    {
      m_Colon = !m_Colon;
      setColon(m_Colon);
    }
    void setColon(bool on)
    {
      m_DecimalPoints = on ? (m_DecimalPoints | 0x10) : (m_DecimalPoints & ~0x10);
      sendDecimalPointControl(m_DecimalPoints);
    }
    void setApostroph(bool on)
    {
      m_DecimalPoints = on ? (m_DecimalPoints | 0x20) : (m_DecimalPoints & ~0x20);
      sendDecimalPointControl(m_DecimalPoints);
    }
    void sendDecimalPointControl(uint8_t data)
    {
      m_Serial.print(0x77,BYTE); // Command byte
      m_Serial.print(data,BYTE);
    }*/
  private:



  };

  typedef C7Segment<spi_Display> Display;


  template<typename LedPin, typename Color>
  class DualColorLED
  {
  public:
    static void init()
    {
      LedPin::set_mode(DIGITAL_OUTPUT);
    }
    static void set() { LedPin::set(Color::value()); }
    static void set(uint8_t v) { LedPin::set(v == Color::value()); }
  };

  typedef PortPin<portExtender, 4> Color;
  typedef DualColorLED<PortPin<portExtender, 0>, Color> Led1;
  typedef DualColorLED<PortPin<portExtender, 5>, Color> Led2;

  inline void initInputs(void)
  {

  }
  inline void initOutputs(void)
  {
    Debug::set_mode(DIGITAL_OUTPUT);
    Debug::set_value(true);
  }

  inline void initHW(void)
  {
    initInputs();
    initOutputs();
    spi_Display::Init();
    Switch_1::Init();
    Switch_2::Init();
    Led1::init();
    Led2::init();
    Encoder::Init();

    portExtender::Init();
  }

#endif /* HARDWARECONFIG_H_ */
