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
:
m_State(&CInitState::getInstance())
{}

void Ui::poll()
{

}

void Ui::CInitState::onExit(Ui& context) const
{
  // read values from EEPROM
  context.setState(Ui::CListenState::getInstance());
}

void Ui::CListenState::onEntry(Ui& context) const
{
  // read state values from EEPROM
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
