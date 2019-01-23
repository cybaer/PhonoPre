/*
 * PhonoPre.cpp
 *
 *  Created on: 12.01.2019
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


#include "avrlib/time.h"
#include "avrlib/gpio.h"
#include "avrlib/adc.h"
#include "HardwareConfig.h"
#include "ui.h"

// __Compiler Bug__
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
void __cxa_pure_virtual() {};

volatile bool poll = false;

ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
{
  //ca 2kHz
  poll = true;
}

int main(void)
{
  sei();
  initHW();

  // 8MHz / (8 * 510) = 1960 Hz
  // prescaler(2)_|
  Timer<2>::set_prescaler(2);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::Start();

  Display::reset();
  Display::setDim(10);
  Color::set(true);

  while(1)
  {
    static uint16_t i = 7000;
    if(poll)
    {
      poll = false;
      portExtender::ReadIO();
      Switch_1::Read();
      Switch_2::Read();
      Debug::Toggle();
      if(Switch_1::lowered()) i++;

      i += Encoder::Read();

      Led2::set(Switch_2::high());
      Led1::set(Encoder::immediate_value());
      portExtender::WriteIO();
      Display::printDec(i);
    }
  }
}

