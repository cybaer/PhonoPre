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
  if(m_Xcrement != 0)    {m_State->onXcrement(*this, m_Xcrement);}
  if(Encoder::clicked()) m_State->onClick(*this);


  static int16_t i = 5000;
  if(Switch_1::lowered()) i++;

  i += m_Xcrement;

  Led2::set(Switch_2::high());
  Led1::set(Encoder::immediate_value());

  portExtender::WriteIO();
  //Display::printDec(m_Xcrement);
}



// State machine
void Ui::CInitState::onExit(Ui& context) const
{
  // read values from EEPROM
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
  Display::printText("Cap1 ");
}
void Ui::CMenueCap1State::onClick(Ui& context) const
{
  context.setState(Ui::CListenState::getInstance());
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

}
void Ui::CMenueCap2State::onClick(Ui& context) const
{
  context.setState(Ui::CListenState::getInstance());
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

}
void Ui::CMenueRPMState::onClick(Ui& context) const
{
  context.setState(Ui::CListenState::getInstance());
}
void Ui::CMenueRPMState::onXcrement(Ui& context, int8_t xcrement) const
{
  if(xcrement > 0)
  {
    context.setState(Ui::CMenueListenState::getInstance());
  }
  if(xcrement < 0)
  {
    context.setState(Ui::CMenueR2State::getInstance());
  }
}



void Ui::CListenState::onEntry(Ui& context) const
{
  // read state values from EEPROM
  //Display::printDec(3000);
}
void Ui::CListenState::onClick(Ui& context) const
{
  context.setState(Ui::CMenueListenState::getInstance());
}
void Ui::CListenState::onClickSW1(Ui& context) const
{
  //
}
void Ui::CListenState::onClickSW2(Ui& context) const
{
  //
}
void Ui::CListenState::onExit(Ui& context) const
{
  //??
}
