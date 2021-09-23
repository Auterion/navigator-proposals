#pragma once

#include "FlightMode.hpp"


enum class FlightModeSelectionResult {
	KEPT_CURRENT,
	SWITCHED,
	SWITCHED_DIFFERENT_COMMANDER_MODE,
	MISSION_ITEM_DONE,
	SWITCHED_FAIL_SAFE_MODE
};


class FlightModeSwitcher {
private:
	void *_flight_modes_address;
public:
	FlightModeSwitcher(void * flight_modes_address) : _flight_modes_address(flight_modes_address) {};

	template<class T, class ... Args>
	bool switchMode(Args && ... args) {
		if (_flight_modes_address != nullptr) {
			if (static_cast<FlightMode*>(_flight_modes_address)->getModeId() == T::MODE_ID) {
				return false;
			}
			static_cast<FlightMode*>(_flight_modes_address)->~FlightMode();
		}
		new (_flight_modes_address) T{...args};
		return true;
	}
};
