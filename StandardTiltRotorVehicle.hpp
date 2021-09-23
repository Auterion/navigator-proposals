#include "Vehicle.hpp"
#include "FWOrbitFlightMode.hpp"
#include "MCPosControlFlightMode.hpp"
#include "MCAltControlFlightMode.hpp"


class StandardTiltrotor : public Vehicle {

	virtual FlightModeSelectionResult modeForCommanderLoiter(const FlightMode &current_flight_mode, FlightModeSwitcher &switcher) {
		if (getState().vtol_configuration == VTOL_CONFIG::MULTICOPTER) {
			if (MCPosControlFlightMode::canRun(getState())) {
				bool switched = switcher.switchMode<MCAltControlFlightMode>(getState());
				return switched ? FlightModeSelectionResult::SWITCHED : FlightModeSelectionResult::KEPT_CURRENT;
			} else if (MCAltControlFlightMode::canRun(getState())) {
				bool switched = switcher.switchMode<MCAltControlFlightMode>(getState());
				return switched ? FlightModeSelectionResult::SWITCHED_DIFFERENT_COMMANDER_MODE : FlightModeSelectionResult::KEPT_CURRENT;
			}
			// more possibilities as we go here ..
		} else {
			if (FWOrbitFlightMode::canRun(getState())) {
				bool switched = switcher.switchMode<MCAltControlFlightMode>(getState());
				return switched ? FlightModeSelectionResult::SWITCHED_DIFFERENT_COMMANDER_MODE : FlightModeSelectionResult::KEPT_CURRENT;
			}
			// more possibilities as we go here
		}
	}

	virtual FlightModeSelectionResult modeForCommanderAltCtl(const FlightMode &current_flight_mode, FlightModeSwitcher &switcher) {
		//..
	}

};



class QuantumVector : public StandardTiltrotor {

// ..

};

