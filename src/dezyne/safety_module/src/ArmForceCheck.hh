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

/********************************** COMPONENT *********************************/
#ifndef ARMFORCECHECK_HH
#define ARMFORCECHECK_HH

#include "Sensors.hh"
#include "ISafetyCheck.hh"
#include "ISafetyCheck.hh"
#include "Resolver.hh"



struct ArmForceCheck
{
  dzn::meta dzn_meta;
  dzn::runtime& dzn_rt;
  dzn::locator const& dzn_locator;


  ::Behavior::type reply_Behavior;

  std::function<void ()> out_iArmForceCheck;

  ::ISafetyCheck iArmForceCheck;

  ::IArmForceSensor iArmForceSensor;
  ::ISafetyCheck iNext;
  ::IResolver iResolver;


  ArmForceCheck(const dzn::locator&);
  void check_bindings() const;
  void dump_tree(std::ostream& os) const;
  friend std::ostream& operator << (std::ostream& os, const ArmForceCheck& m)  {
    (void)m;
    return os << "[" << "]" ;
  }
  private:
  ::Behavior::type iArmForceCheck_do_check();

  ::Behavior::type and_safety_states (::Behavior::type current,::Behavior::type next);
};

#endif // ARMFORCECHECK_HH

/********************************** COMPONENT *********************************/


//version: 2.9.1
