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
  int8_t channel;
  int8_t cap1;
  int8_t cap2;
  bool res1;
  bool res2;
};
PhonoData eeData EEMEM;
PhonoData Data = {0, 8, 8, false, false};

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
  static int16_t LONG_CLICK_COUNT = 2000L;

  if(m_Xcrement != 0)     m_State->onXcrement(*this, m_Xcrement);
  if(Encoder::clicked())  m_State->onClick(*this);
  if(Switch_1::raised())  m_State->onClickSW1(*this);
  if(Switch_2::raised())  m_State->onClickSW2(*this);

  if(Switch_1::low())
  {
    if(!m_LongClickActiveSW1 && ++m_longClickCounterSW1 >= LONG_CLICK_COUNT)
    {
      m_State->onLongClickSW1(*this);
      m_LongClickActiveSW1 = true;
    }
  }
  else
  {
    m_longClickCounterSW1 = 0;
    m_LongClickActiveSW1 = false;
  }
  portExtender::WriteIO();
}

void setResText(bool high)
{
  if(high)  Display::printText("Hi  ");
  else      Display::printText("Lo  ");
}


// State machine
void Ui::CInitState::onExit(Ui& context) const
{
  eeprom_write_block(&Data, &eeData, sizeof(Data));

  // read values from EEPROM
  eeprom_read_block(&Data, &eeData, sizeof(Data));
  Cap1::set(Data.cap1);
  Cap2::set(Data.cap2);
  Res1::setHigh(Data.res1);
  Res2::setHigh(Data.res2);
  ChannelSwitch::setValue(Data.channel);
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
  context.setState(Ui::CRes1State::getInstance());
}
void Ui::CMenueR1State::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)  context.setState(Ui::CMenueR2State::getInstance());
  if(xcrement < 0)  context.setState(Ui::CMenueCap2State::getInstance());
}
void Ui::CMenueR2State::onEntry(Ui& context) const
{
  Display::printText("Res2");
}
void Ui::CMenueR2State::onClick(Ui& context) const
{
  context.setState(Ui::CRes2State::getInstance());
}
void Ui::CMenueR2State::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)  context.setState(Ui::CMenueRPMState::getInstance());
  if(xcrement < 0)  context.setState(Ui::CMenueR1State::getInstance());
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
  if(xcrement > 0)  context.setState(Ui::CMenueLoadState::getInstance());
  if(xcrement < 0)  context.setState(Ui::CMenueR2State::getInstance());
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
  if(xcrement > 0)  context.setState(Ui::CMenueListenState::getInstance());
  if(xcrement < 0)  context.setState(Ui::CMenueRPMState::getInstance());
}

//// working states ////

void Ui::CListenState::onEntry(Ui& context) const
{
  if(Data.channel == 0)   Display::printText("LIS1");
  else Display::printText("LIS2");
}
void Ui::CListenState::onClick(Ui& context) const
{
  context.setState(Ui::CMenueListenState::getInstance());
}
void Ui::CListenState::onClickSW1(Ui& context) const
{
  ChannelSwitch::activateCh1();
  Display::printText("LIS1");
  Data.channel = ChannelSwitch::value;
  eeprom_write_block (&Data, &eeData, sizeof(Data));
}
void Ui::CListenState::onClickSW2(Ui& context) const
{
  ChannelSwitch::activateCh2();
  Display::printText("LIS2");
  Data.channel = ChannelSwitch::value;
  eeprom_write_block (&Data, &eeData, sizeof(Data));
}
void Ui::CListenState::onExit(Ui& context) const
{

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
}
void Ui::CCap1State::onClickSW1(Ui& context) const
{
  if(context.m_CapValChannel1.valueSW1.isValue)
  {
    Cap1::set(context.m_CapValChannel1.valueSW1.value);
    Display::printDec(Cap1::value);
    Data.cap1 = Cap1::value;
  }
}
void Ui::CCap1State::onClickSW2(Ui& context) const
{
  Display::printText("T2");
}
void Ui::CCap1State::onLongClickSW1(Ui& context) const
{
  context.m_CapValChannel1.valueSW1.setValue(Data.cap1);
  Led1::set(true);
}
void Ui::CCap1State::onLongClickSW2(Ui& context) const
{
  Display::printText("T2");
}
void Ui::CCap1State::onExit(Ui& context) const
{
  eeprom_write_block (&Data, &eeData, sizeof(Data));
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
  eeprom_write_block (&Data, &eeData, sizeof(Data));
}

void Ui::CRes1State::onEntry(Ui& context) const
{
  setResText(Data.res1);
}
void Ui::CRes1State::onClick(Ui& context) const
{
  context.setState(Ui::CMenueR1State::getInstance());
}
void Ui::CRes1State::onXcrement(Ui& context, int8_t xcrement) const
{
  bool isHigh = xcrement > 0;
  Res1::setHigh(isHigh);
  setResText(isHigh);
  Data.res1 = Res1::value;
}
void Ui::CRes1State::onExit(Ui& context) const
{
  eeprom_write_block (&Data, &eeData, sizeof(Data));
}


void Ui::CRes2State::onEntry(Ui& context) const
{
  setResText(Data.res2);
}
void Ui::CRes2State::onClick(Ui& context) const
{
  context.setState(Ui::CMenueR2State::getInstance());
}
void Ui::CRes2State::onXcrement(Ui& context, int8_t xcrement) const
{
  bool isHigh = xcrement > 0;
  Res2::setHigh(isHigh);
  setResText(isHigh);
  Data.res2 = Res2::value;
}
void Ui::CRes2State::onExit(Ui& context) const
{
  eeprom_write_block (&Data, &eeData, sizeof(Data));
}

