// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <dzn/meta.hh>

namespace dzn {
  struct locator;
  struct runtime;
}



#include <iostream>
#include <map>

#ifndef ENUM_Behavior
#define ENUM_Behavior 1


struct Behavior
{
  enum type
  {
    Unsafe,Safe
  };
};


#endif // ENUM_Behavior
#ifndef ENUM_UnsafeTriggered
#define ENUM_UnsafeTriggered 1


struct UnsafeTriggered
{
  enum type
  {
    No,Yes
  };
};


#endif // ENUM_UnsafeTriggered

/********************************** INTERFACE *********************************/
#ifndef ICONTROLLER_HH
#define ICONTROLLER_HH



struct IController
{
#ifndef ENUM_IController_State
#define ENUM_IController_State 1


  struct State
  {
    enum type
    {
      Idle,Operating
    };
  };


#endif // ENUM_IController_State

  struct
  {
    std::function< void()> initialise;
    std::function< void()> destruct;
    std::function< void()> reset;
    std::function< ::UnsafeTriggered::type()> do_checks;
  } in;

  struct
  {
    std::function< void()> initialise_imu;
    std::function< void()> initialise_mutexes;
    std::function< void()> initialise_semaphores;
    std::function< void()> destruct_mutexes;
    std::function< void()> destruct_semaphores;
  } out;

  dzn::port::meta meta;
  inline IController(const dzn::port::meta& m) : meta(m) {}

  void check_bindings() const
  {
    if (! in.initialise) throw dzn::binding_error(meta, "in.initialise");
    if (! in.destruct) throw dzn::binding_error(meta, "in.destruct");
    if (! in.reset) throw dzn::binding_error(meta, "in.reset");
    if (! in.do_checks) throw dzn::binding_error(meta, "in.do_checks");

    if (! out.initialise_imu) throw dzn::binding_error(meta, "out.initialise_imu");
    if (! out.initialise_mutexes) throw dzn::binding_error(meta, "out.initialise_mutexes");
    if (! out.initialise_semaphores) throw dzn::binding_error(meta, "out.initialise_semaphores");
    if (! out.destruct_mutexes) throw dzn::binding_error(meta, "out.destruct_mutexes");
    if (! out.destruct_semaphores) throw dzn::binding_error(meta, "out.destruct_semaphores");

  }
};

inline void connect (IController& provided, IController& required)
{
  provided.out = required.out;
  required.in = provided.in;
  provided.meta.requires = required.meta.requires;
  required.meta.provides = provided.meta.provides;
}


#ifndef ENUM_TO_STRING_Behavior
#define ENUM_TO_STRING_Behavior 1
inline std::string to_string(::Behavior::type v)
{
  switch(v)
  {
    case ::Behavior::Unsafe: return "Behavior_Unsafe";
    case ::Behavior::Safe: return "Behavior_Safe";

  }
  return "";
}
#endif // ENUM_TO_STRING_Behavior
#ifndef ENUM_TO_STRING_UnsafeTriggered
#define ENUM_TO_STRING_UnsafeTriggered 1
inline std::string to_string(::UnsafeTriggered::type v)
{
  switch(v)
  {
    case ::UnsafeTriggered::No: return "UnsafeTriggered_No";
    case ::UnsafeTriggered::Yes: return "UnsafeTriggered_Yes";

  }
  return "";
}
#endif // ENUM_TO_STRING_UnsafeTriggered
#ifndef ENUM_TO_STRING_IController_State
#define ENUM_TO_STRING_IController_State 1
inline std::string to_string(::IController::State::type v)
{
  switch(v)
  {
    case ::IController::State::Idle: return "State_Idle";
    case ::IController::State::Operating: return "State_Operating";

  }
  return "";
}
#endif // ENUM_TO_STRING_IController_State

#ifndef STRING_TO_ENUM_Behavior
#define STRING_TO_ENUM_Behavior 1
inline ::Behavior::type to_Behavior(std::string s)
{
  static std::map<std::string, ::Behavior::type> m = {
    {"Behavior_Unsafe", ::Behavior::Unsafe},
    {"Behavior_Safe", ::Behavior::Safe},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_Behavior
#ifndef STRING_TO_ENUM_UnsafeTriggered
#define STRING_TO_ENUM_UnsafeTriggered 1
inline ::UnsafeTriggered::type to_UnsafeTriggered(std::string s)
{
  static std::map<std::string, ::UnsafeTriggered::type> m = {
    {"UnsafeTriggered_No", ::UnsafeTriggered::No},
    {"UnsafeTriggered_Yes", ::UnsafeTriggered::Yes},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_UnsafeTriggered
#ifndef STRING_TO_ENUM_IController_State
#define STRING_TO_ENUM_IController_State 1
inline ::IController::State::type to_IController_State(std::string s)
{
  static std::map<std::string, ::IController::State::type> m = {
    {"State_Idle", ::IController::State::Idle},
    {"State_Operating", ::IController::State::Operating},
  };
  return m.at(s);
}
#endif // STRING_TO_ENUM_IController_State


#endif // ICONTROLLER_HH

/********************************** INTERFACE *********************************/
/********************************** COMPONENT *********************************/
#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include "LEDControl.hh"
#include "ISafetyCheck.hh"



struct Controller
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;
#ifndef ENUM_Controller_State
#define ENUM_Controller_State 1


  struct State
  {
    enum type
    {
      Idle,Operating
    };
  };


#endif // ENUM_Controller_State

  ::Controller::State::type systemState;
  bool unsafe_acknowledged;
  char* red;
  char* blue;

  ::UnsafeTriggered::type reply_UnsafeTriggered;
  ::Behavior::type reply_Behavior;

  std::function<void ()> out_iController;

  ::IController iController;

  ::ILEDControl iLEDControl;
  ::ISafetyCheck iNext;


  Controller(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const Controller& m)  {
    (void)m;
    return os << "[" << m.systemState <<", " << m.unsafe_acknowledged <<", " << m.red <<", " << m.blue <<"]" ;
  }
  private:
  void iController_initialise();
  void iController_destruct();
  void iController_reset();
  ::UnsafeTriggered::type iController_do_checks();

};

#endif // CONTROLLER_HH

/********************************** COMPONENT *********************************/


//version: 2.9.1
