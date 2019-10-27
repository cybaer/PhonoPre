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
typedef Gpio<PortD, 0> RPM;

// HW SPI
static const uint8_t SPI_Speed = 32;
typedef SpiMasterBase<MSB_FIRST, SPI_Speed> spi_base;
typedef MCP23S08<spi_base, CS2, 0> portExtender;

typedef DebouncedSwitch<PortPin<portExtender, 1> > Switch_1;
typedef DebouncedSwitch<PortPin<portExtender, 2> > Switch_2;

template<typename SW>
class SWLongClick
{
  static const int16_t LONG_CLICK_COUNT = 2000L;
public:
  static bool raised(void)
  {
    if(SW::low())
    {
      if(!active && ++counter >= LONG_CLICK_COUNT)
      {
        active = true;
        return true;
      }
    }
    else
    {
      counter = 0;
      active = false;
    }
    return false;
  }
  static bool active;
  static uint16_t counter;
};

template<typename SW>
bool SWLongClick<SW>::active = false;
template<typename SW>
uint16_t SWLongClick<SW>::counter = 0;

typedef SWLongClick<Switch_1>  SW1LongClick;
typedef SWLongClick<Switch_2>  SW2LongClick;

typedef RotaryEncoder<PortPin<portExtender, 6>, PortPin<portExtender, 7>, PortPin<portExtender, 3> > Encoder;

template<typename SPIBase, typename SlaveSelect>
class C7Segment
{
public:
  static inline void Begin()
  {
    SlaveSelect::Low();
  }
  static inline void End()
  {
    SlaveSelect::High();
  }
  static inline void Write(uint8_t v)
  {
    Begin();
    SPIBase::Send(v);
    End();
  }

  static void reset()
  {
    Write('v');
  }
  static void setDim(uint8_t dim)
  {
    Write(0x7A); // Command byte
    Write(dim);    // Dim display
  }
  static void printDec(int val)
  {
    if (val > 9999)
      return;
    char buf[12];
    sprintf(buf, "%4d", val);
    Write(buf[0]);
    Write(buf[1]);
    Write(buf[2]);
    Write(buf[3]);
  }
  static void printText(const char *t)
  {
    char buf[5];
    for (int i = 0; i < 4; i++)
    {
      buf[i] = t[i];
    }
    buf[4] = '\0';
    Write(buf[0]);
    Write(buf[1]);
    Write(buf[2]);
    Write(buf[3]);
  }
};

typedef C7Segment<spi_base, CS1> Display;

template<typename Rel1, typename Rel2, typename Rel3, typename Rel4>
class Capacitor
{
public:
  static void init()
  {
    Rel1::set_mode(DIGITAL_OUTPUT);
    Rel2::set_mode(DIGITAL_OUTPUT);
    Rel3::set_mode(DIGITAL_OUTPUT);
    Rel4::set_mode(DIGITAL_OUTPUT);
  }
  static void set(int8_t val)
  {
    value = val;
    Rel1::set_value(val & 0x01);
    Rel2::set_value(val & 0x02);
    Rel3::set_value(val & 0x04);
    Rel4::set_value(val & 0x08);
  }
  static int8_t value;
};
template<typename Rel1, typename Rel2, typename Rel3, typename Rel4>
int8_t Capacitor<Rel1, Rel2, Rel3, Rel4>::value;

typedef Capacitor<Gpio<PortD, 6>, Gpio<PortD, 7>, Gpio<PortD, 4>, Gpio<PortD, 5> > Cap1;
typedef Capacitor<Gpio<PortC, 3>, Gpio<PortC, 4>, Gpio<PortC, 5>, Gpio<PortC, 2> > Cap2;

template<typename Rel1, typename Rel2>
class Switcher
{
public:
  static void init()
  {
    Rel1::set_mode(DIGITAL_OUTPUT);
    Rel2::set_mode(DIGITAL_OUTPUT);
  }
  static void setValue(int8_t in)
  {
    value = in > 2 ? 2 : in < 0 ? 0 : in;
    Rel1::set_value(value & 0x1);
    Rel2::set_value(value & 0x2);
  }
  static void activateCh1()
  {
    setValue(2);
  }
  static void activateCh2()
  {
    setValue(1);
  }
  static int8_t value;
};
template<typename Rel1, typename Rel2>
int8_t Switcher<Rel1, Rel2>::value;

typedef Switcher<Gpio<PortB, 6>, Gpio<PortB, 7> > Res1;
typedef Switcher<Gpio<PortC, 0>, Gpio<PortC, 1> > Res2;
typedef Switcher<Gpio<PortD, 3>, Gpio<PortD, 1> > ChannelSwitch;

template<typename LedPin, typename Color>
class DualColorLED
{
public:
  static void init()
  {
    LedPin::set_mode(DIGITAL_OUTPUT);
  }
  static void setGreen(void)
  {
    Color::clear();
  }
  static void setRed(void)
  {
    Color::set();
  }
  static void set()
  {
    LedPin::set(!Color::value());
  }
  static void clear()
  {
    LedPin::set(Color::value());
  }
  static void set(uint8_t v)
  {
    if (v)
      LedPin::set();
    else
      LedPin::clear();
  }
  static bool get()
  {
    return LedPin::value() != Color::value();
  }
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
  Debug::set_value(false);
  RPM::set_mode(DIGITAL_OUTPUT);
  RPM::set_value(true);
}

inline void initHW(void)
{
  initInputs();
  initOutputs();

  Cap1::init();
  Cap2::init();
  Res1::init();
  Res2::init();

  Switch_1::Init();
  Switch_2::Init();
  Led1::init();
  Led2::init();
  Encoder::Init();

  spi_base::Init();

  portExtender::Init();
}

#endif /* HARDWARECONFIG_H_ */
