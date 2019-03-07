/*
 * ui.cpp
 *
 *  Created on: 23.01.2019
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

#include "ui.h"
#include <avr/eeprom.h>

struct PhonoData
{
  int8_t cap1;
  int8_t cap2;
  bool res1;
  bool res2;
};
PhonoData eeData EEMEM ;
PhonoData Data;

Ui::Ui()
: m_Xcrement(0)
, m_State(&Ui::CInitState::getInstance())
{}
void Ui::init() { setState(Ui::CListenState::getInstance()); }
void Ui::poll()
{
  portExtender::ReadIO();
  Switch_1::Read();
  Switch_2::Read();
  m_Xcrement = Encoder::Read();

}
void Ui::doEvents()
{
  if(m_Xcrement != 0)     m_State->onXcrement(*this, m_Xcrement);
  if(Encoder::clicked())  m_State->onClick(*this);
  if(Switch_1::raised())  m_State->onClickSW1(*this);
  if(Switch_2::raised())  m_State->onClickSW2(*this);

  /*
  static int16_t i = 0;
  if(Switch_1::lowered()) i++;

  i += m_Xcrement;
  //Cap1::set(i);
  Cap2::set(i);

  Led2::set(Switch_2::high());
  Res2::setHigh(Switch_2::high());

  Led1::set(Encoder::immediate_value());

  portExtender::WriteIO();
  Display::printDec(i);*/

  portExtender::WriteIO();
}



// State machine
void Ui::CInitState::onExit(Ui& context) const
{
  // read values from EEPROM
  eeprom_read_block(&Data, &eeData, sizeof(Data));
  Cap1::set(Data.cap1);
  Cap2::set(Data.cap2);
  Res1::setHigh(Data.res1);
  Res2::setHigh(Data.res2);
  //context.setState(Ui::CListenState::getInstance());
}

void Ui::CMenueListenState::onEntry(Ui& context) const
{
  Display::printText("Lis ");
}
void Ui::CMenueListenState::onClick(Ui& context) const
{
  context.setState(Ui::CListenState::getInstance());
}
void Ui::CMenueListenState::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    context.setState(Ui::CMenueCap1State::getInstance());
  }
  if(xcrement < 0)
  {
    context.setState(Ui::CMenueRPMState::getInstance());
  }
}
void Ui::CMenueCap1State::onEntry(Ui& context) const
{
  Display::printText("Cap1");
}
void Ui::CMenueCap1State::onClick(Ui& context) const
{
  context.setState(Ui::CCap1State::getInstance());
}
void Ui::CMenueCap1State::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    context.setState(Ui::CMenueCap2State::getInstance());
  }
  if(xcrement < 0)
  {
    context.setState(Ui::CMenueListenState::getInstance());
  }
}
void Ui::CMenueCap2State::onEntry(Ui& context) const
{
  Display::printText("Cap2");
}
void Ui::CMenueCap2State::onClick(Ui& context) const
{
  context.setState(Ui::CCap2State::getInstance());
}
void Ui::CMenueCap2State::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    context.setState(Ui::CMenueR1State::getInstance());
  }
  if(xcrement < 0)
  {
    context.setState(Ui::CMenueCap1State::getInstance());
  }
}
void Ui::CMenueR1State::onEntry(Ui& context) const
{
  Display::printText("Res1");
}
void Ui::CMenueR1State::onClick(Ui& context) const
{
  context.setState(Ui::CListenState::getInstance());
}
void Ui::CMenueR1State::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    context.setState(Ui::CMenueR2State::getInstance());
  }
  if(xcrement < 0)
  {
    context.setState(Ui::CMenueCap2State::getInstance());
  }
}
void Ui::CMenueR2State::onEntry(Ui& context) const
{
  Display::printText("Res2");
}
void Ui::CMenueR2State::onClick(Ui& context) const
{
  context.setState(Ui::CListenState::getInstance());
}
void Ui::CMenueR2State::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    context.setState(Ui::CMenueRPMState::getInstance());
  }
  if(xcrement < 0)
  {
    context.setState(Ui::CMenueR1State::getInstance());
  }
}
void Ui::CMenueRPMState::onEntry(Ui& context) const
{
  Display::printText("Freq");
}
void Ui::CMenueRPMState::onClick(Ui& context) const
{
  context.setState(Ui::CListenState::getInstance());
}
void Ui::CMenueRPMState::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    context.setState(Ui::CMenueLoadState::getInstance());
  }
  if(xcrement < 0)
  {
    context.setState(Ui::CMenueR2State::getInstance());
  }
}
void Ui::CMenueLoadState::onEntry(Ui& context) const
{
  Display::printText("Load");
}
void Ui::CMenueLoadState::onClick(Ui& context) const
{
  context.setState(Ui::CListenState::getInstance());
}
void Ui::CMenueLoadState::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    context.setState(Ui::CMenueListenState::getInstance());
  }
  if(xcrement < 0)
  {
    context.setState(Ui::CMenueRPMState::getInstance());
  }
}



void Ui::CListenState::onEntry(Ui& context) const
{
  Display::printText("LIS1");
}
void Ui::CListenState::onClick(Ui& context) const
{
  context.setState(Ui::CMenueListenState::getInstance());
}
void Ui::CListenState::onClickSW1(Ui& context) const
{
  RPM::set_value(true);
  ChannelSwitch::activateCh1();
  Display::printText("LIS1");
}
void Ui::CListenState::onClickSW2(Ui& context) const
{
  RPM::set_value(false);
  ChannelSwitch::activateCh2();
  Display::printText("LIS2");
}
void Ui::CListenState::onExit(Ui& context) const
{
  //??
}

void Ui::CCap1State::onEntry(Ui& context) const
{
  Display::printDec(Cap1::value);
}
void Ui::CCap1State::onClick(Ui& context) const
{
  context.setState(Ui::CMenueCap1State::getInstance());
}
void Ui::CCap1State::onXcrement(Ui& context, int8_t xcrement) const
{
 Cap1::set(Cap1::value + xcrement);
 Display::printDec(Cap1::value);
 Data.cap1 = Cap1::value;
 eeprom_write_block (&Data, &eeData, sizeof(Data));
}
void Ui::CCap1State::onClickSW1(Ui& context) const
{
  Display::printText("T1");
}
void Ui::CCap1State::onClickSW2(Ui& context) const
{
  Display::printText("T2");
}
void Ui::CCap1State::onExit(Ui& context) const
{
  //??
}

void Ui::CCap2State::onEntry(Ui& context) const
{
  Display::printDec(Cap2::value);
}
void Ui::CCap2State::onClick(Ui& context) const
{
  context.setState(Ui::CMenueCap2State::getInstance());
}
void Ui::CCap2State::onXcrement(Ui& context, int8_t xcrement) const
{
  Cap2::set(Cap2::value + xcrement);
  Display::printDec(Cap2::value);
  Data.cap2 = Cap2::value;
  eeprom_write_block (&Data, &eeData, sizeof(Data));
}
void Ui::CCap2State::onClickSW1(Ui& context) const
{
  Display::printText("T1");
}
void Ui::CCap2State::onClickSW2(Ui& context) const
{
  Display::printText("T2");
}
void Ui::CCap2State::onExit(Ui& context) const
{
  //??
}
