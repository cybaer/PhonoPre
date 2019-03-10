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
  int8_t m_Xcrement;

  int16_t m_longClickCounterSW1;
  int16_t m_longClickCounterSW2;
  bool m_LongClickActiveSW1;
  bool m_LongClickActiveSW2;

  struct CapValue
  {
    bool isValue = false;
    int8_t value = 0;

    void setValue(int8_t val)
    {
      isValue = true;
      value = val;
    }
    void reset(void) { isValue = false; }
  };
  struct CapValues
  {
    CapValue valueSW1;
    CapValue valueSW2;
  };
  CapValues m_CapValChannel1;
  CapValues m_CapValChannel2;

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
    virtual void onLongClickSW1(Ui&) const {}
    virtual void onLongClickSW2(Ui&) const {}

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

  class CMenueListenState: public IUiState
  {
  public:
    static CMenueListenState& getInstance(void)
    {
      static CMenueListenState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };

  class CMenueCap1State: public IUiState
  {
  public:
    static CMenueCap1State& getInstance(void)
    {
      static CMenueCap1State s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };
  class CMenueCap2State: public IUiState
  {
  public:
    static CMenueCap2State& getInstance(void)
    {
      static CMenueCap2State s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };
  class CMenueR1State: public IUiState
  {
  public:
    static CMenueR1State& getInstance(void)
    {
      static CMenueR1State s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };
  class CMenueR2State: public IUiState
  {
  public:
    static CMenueR2State& getInstance(void)
    {
      static CMenueR2State s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };
  class CMenueRPMState: public IUiState
  {
  public:
    static CMenueRPMState& getInstance(void)
    {
      static CMenueRPMState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };
  class CMenueLoadState: public IUiState
  {
  public:
    static CMenueLoadState& getInstance(void)
    {
      static CMenueLoadState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
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
    virtual void onClick(Ui&) const;
    //virtual void onXcrement(Ui&, int8_t xcrement) const;
    virtual void onClickSW1(Ui&) const;
    virtual void onClickSW2(Ui&) const;
  };

  class CCap1State: public IUiState
  {
  public:
    static CCap1State& getInstance(void)
    {
      static CCap1State s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onExit(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
    virtual void onClickSW1(Ui&) const;
    virtual void onClickSW2(Ui&) const;
    virtual void onLongClickSW1(Ui&) const;
    virtual void onLongClickSW2(Ui&) const;
  };

  class CCap2State: public IUiState
  {
  public:
    static CCap2State& getInstance(void)
    {
      static CCap2State s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onExit(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
    virtual void onClickSW1(Ui&) const;
    virtual void onClickSW2(Ui&) const;
    virtual void onLongClickSW1(Ui&) const;
    virtual void onLongClickSW2(Ui&) const;
  };

  class CRes1State: public IUiState
  {
  public:
    static CRes1State& getInstance(void)
    {
      static CRes1State s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onExit(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
  };

  class CRes2State: public IUiState
  {
  public:
    static CRes2State& getInstance(void)
    {
      static CRes2State s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onExit(Ui&) const;
    virtual void onClick(Ui&) const;
    virtual void onXcrement(Ui&, int8_t xcrement) const;
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
