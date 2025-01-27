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
#include "System.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>


//SYSTEM

System::System(const dzn::locator& dzn_locator)
: dzn_meta{"","System",0,0,{& iLEDControl.meta,& iAccelerationSensor.meta,& iAngularVelocitySensor.meta,& iArmForceSensor.meta,& iArmTorqueSensor.meta,& iArmPositionSensor.meta},{& controller.dzn_meta,& kineticEnergyCheck.dzn_meta,& rotationalEnergyCheck.dzn_meta,& armPositionCheck.dzn_meta,& armForceCheck.dzn_meta,& armTorqueCheck.dzn_meta,& baseCaseCheck.dzn_meta},{[this]{iController.check_bindings();},[this]{iLEDControl.check_bindings();},[this]{iAccelerationSensor.check_bindings();},[this]{iAngularVelocitySensor.check_bindings();},[this]{iArmForceSensor.check_bindings();},[this]{iArmTorqueSensor.check_bindings();},[this]{iArmPositionSensor.check_bindings();}}}
, dzn_rt(dzn_locator.get<dzn::runtime>())
, dzn_locator(dzn_locator)


, controller(dzn_locator)
, kineticEnergyCheck(dzn_locator)
, rotationalEnergyCheck(dzn_locator)
, armPositionCheck(dzn_locator)
, armForceCheck(dzn_locator)
, armTorqueCheck(dzn_locator)
, baseCaseCheck(dzn_locator)

, iController(controller.iController)
, iLEDControl(controller.iLEDControl), iAccelerationSensor(kineticEnergyCheck.iAccelerationSensor), iAngularVelocitySensor(rotationalEnergyCheck.iAngularVelocitySensor), iArmForceSensor(armForceCheck.iArmForceSensor), iArmTorqueSensor(armTorqueCheck.iArmTorqueSensor), iArmPositionSensor(armPositionCheck.iArmPositionSensor)
{


  controller.dzn_meta.parent = &dzn_meta;
  controller.dzn_meta.name = "controller";
  kineticEnergyCheck.dzn_meta.parent = &dzn_meta;
  kineticEnergyCheck.dzn_meta.name = "kineticEnergyCheck";
  rotationalEnergyCheck.dzn_meta.parent = &dzn_meta;
  rotationalEnergyCheck.dzn_meta.name = "rotationalEnergyCheck";
  armPositionCheck.dzn_meta.parent = &dzn_meta;
  armPositionCheck.dzn_meta.name = "armPositionCheck";
  armForceCheck.dzn_meta.parent = &dzn_meta;
  armForceCheck.dzn_meta.name = "armForceCheck";
  armTorqueCheck.dzn_meta.parent = &dzn_meta;
  armTorqueCheck.dzn_meta.name = "armTorqueCheck";
  baseCaseCheck.dzn_meta.parent = &dzn_meta;
  baseCaseCheck.dzn_meta.name = "baseCaseCheck";


  connect(kineticEnergyCheck.iKineticEnergyCheck, controller.iNext);
  connect(rotationalEnergyCheck.iRotationalEnergyCheck, kineticEnergyCheck.iNext);
  connect(armForceCheck.iArmForceCheck, rotationalEnergyCheck.iNext);
  connect(armTorqueCheck.iArmTorqueCheck, armForceCheck.iNext);
  connect(armPositionCheck.iArmPositionCheck, armTorqueCheck.iNext);
  connect(baseCaseCheck.iRoot, armPositionCheck.iNext);

  dzn::rank(iController.meta.provides.meta, 0);

}

void System::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void System::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}

////////////////////////////////////////////////////////////////////////////////



//version: 2.9.1