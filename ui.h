/*
 * ui.h
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

#ifndef UI_H_
#define UI_H_

// __Compiler Bug__
__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);
extern "C" void __cxa_pure_virtual();


#include "HardwareConfig.h"
#include "avrlib/base.h"

using namespace avrlib;

class Ui
{
public:
  Ui();
  void init();
  void poll();
  void doEvents();

private:


class IUiState
  {
  public:
    //** Empty Constructor */
    IUiState(void) {}
    virtual void onEntry(Ui&) const {}
    virtual void onExit(Ui&) const {}
    virtual void onClick(Ui&) const {}
    virtual void onXcrement(Ui&, int8_t xcrement) const {}
    virtual void onClickSW1(Ui&) const {}
    virtual void onClickSW2(Ui&) const {}

    //virtual void onLongClick(Ui&) const {}

  private:
    /** Forbidden */
    DISALLOW_COPY_AND_ASSIGN (IUiState);
  };

  /**
   * State machine
   */
  class CInitState: public IUiState
  {
  public:
    static CInitState& getInstance(void)
    {
      static CInitState s_instance;
      return s_instance;
    }
    virtual void onExit(Ui&) const;

  };

  class CListenState: public IUiState
  {
  public:
    static CListenState& getInstance(void)
    {
      static CListenState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onExit(Ui&) const;
    //virtual void onClick(Ui&) const;
    //virtual void onXcrement(Ui&, int8_t xcrement) const;
    virtual void onClickSW1(Ui&) const;
    virtual void onClickSW2(Ui&) const;

  };

  void setState(IUiState& state)
    {
      m_State->onExit(*this);
      m_State = &state;
      m_State->onEntry(*this);
    }

    IUiState* m_State;
};

static Ui ui;

#endif /* UI_H_ */
