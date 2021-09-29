#include <cstdint>
#include <unistd.h>


namespace util {
	// source: https://gist.github.com/ruby0x1/81308642d0325fd386237cfa3b44785c
	constexpr uint32_t val_32_const = 0x811c9dc5;
	constexpr uint32_t prime_32_const = 0x1000193;
	constexpr uint64_t val_64_const = 0xcbf29ce484222325;
	constexpr uint64_t prime_64_const = 0x100000001b3;
	inline constexpr uint32_t hash_32_fnv1a_const(const char *const str, const uint32_t value = val_32_const) noexcept
	{
		return (str[0] == '\0') ? value : hash_32_fnv1a_const(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
	}
};




class VehicleState {

/*
# Things that get checked for being able to run a flightmode

- Arming state
- Current position
- Current velocity
- Current speed
- Home position
- Land detection
- Vehicle type
- Sticks inputs available

- params

*/
public:
    /**
     * Updates all fields in this vehicle state
     * Object by copying from the uORB topics
     */
    void update() {

    }

};



// forward defined non-existent setpoint class
class Setpoints {
    /*
     This class should contain
     position, velocity and acceleration setpoints.
     These setpoints then get sent to the controllers
     that execute on those
     */

};

/**
 * A flight mode is a vehicle specific (multicopter, fixed-wing, or other) specific
 * class that implements a certain vehicle behaviour. This could be something like
 * Taking off, or flying an orbit. FlightModes get instantiated by the vehicle class
 * as a reaction to commander state / user actions or as part of a mission item.
 */
class FlightMode {
public:
	using IdType = uint32_t;

    /**
     * Each FlightMode is supposed to have its own specific ID. 
     * This function is provided such that flight modes can generate
     * a compile-time integer ID from their name
     */
	template<size_t N>
	static constexpr uint32_t ID(const char (&name)[N])
	{
		return (0xffffff & util::hash_32_fnv1a_const(name))
	}

    /**
     * Each FlightMode should have a MODE_ID static public field. This function
     * should get overridden to report back this field. 
     * @return IdType 
     */
	virtual IdType getModeId() const = 0;

    /**
     * Function that gets called during execution of the flight mode
     * to generate the set points. It received the current vehicle state,
     * as ell as the time that has elapsed since the last time this function
     * was executed. The vehicle state also contains stick positions, so the flight mode
     * can also react to stick movements. The function is supposed to use some sort
     * of trajectory planning and to return set points for the controllers. 
     * @param vehicle_state 
     * @param delta_time 
     * @return Setpoints 
     */
    virtual Setpoints generateSetpoints(const VehicleState &vehicle_state, float delta_time) = 0; 

};




enum class CommanderState {
	MANUAL,
	ALTCTL,
	POSCTL,
	AUTO_MISSION,
	AUTO_LOITER,
	AUTO_RTL,
	ACRO,
	OFFBOARD,
	STAB,
	AUTO_TAKEOFF,
	AUTO_LAND,
	AUTO_FOLLOW_TARGET,
	AUTO_PRECLAND,
	ORBIT,
	MAX
};

enum class NavCommand {
	IDLE,
	WAYPOINT,
	LOITER_UNLIMITED,
	LOITER_TIME_LIMIT,
	RETURN_TO_LAUNCH,
	LAND,
	TAKEOFF,
	LOITER_TO_ALT,
	DO_FOLLOW_REPOSITION,
	VTOL_TAKEOFF,
	VTOL_LAND,
	DELAY,
	DO_JUMP,
	DO_CHANGE_SPEED,
	DO_SET_HOME,
	DO_SET_SERVO,
	DO_LAND_START,
	DO_SET_ROI_LOCATION,
	DO_SET_ROI_WPNEXT_OFFSET,
	DO_SET_ROI_NONE,
	DO_CONTROL_VIDEO,
	DO_SET_ROI,
	DO_DIGICAM_CONTROL,
	DO_MOUNT_CONFIGURE,
	DO_MOUNT_CONTROL,
	DO_SET_CAM_TRIGG_INTERVAL,
	DO_SET_CAM_TRIGG_DIST,
	OBLIQUE_SURVEY,
	SET_CAMERA_MODE,
	SET_CAMERA_ZOOM,
	SET_CAMERA_FOCUS,
	DO_GIMBAL_MANAGER_PITCHYAW,
	DO_GIMBAL_MANAGER_CONFIGURE,
	IMAGE_START_CAPTURE,
	IMAGE_STOP_CAPTURE,
	DO_TRIGGER_CONTROL,
	VIDEO_START_CAPTURE,
	VIDEO_STOP_CAPTURE,
	DO_VTOL_TRANSITION,
	FENCE_RETURN_POINT,
	FENCE_POLYGON_VERTEX_INCLUSION,
	FENCE_POLYGON_VERTEX_EXCLUSION,
	FENCE_CIRCLE_INCLUSION,
	FENCE_CIRCLE_EXCLUSION,
	CONDITION_GATE
};




class FWOrbitFlightMode : public FlightMode {
public:
	static constexpr IdType MODE_ID = FlightMode::ID("FWOrbitFlightMode");
	virtual IdType getModeId() const {
		return MODE_ID;
	}

	static bool canRun(const VehicleState& state) {
		return false;
	}


    virtual Setpoints generateSetpoints(const VehicleState &vehicle_state, float delta_time) {
        return Setpoints{};
    }

};



class MCPosControlFlightMode : public FlightMode {
public:
	static constexpr IdType MODE_ID = FlightMode::ID("MCPosControlFlightMode");
	virtual IdType getModeId() const {
		return MODE_ID;
	}

	static bool canRun(const VehicleState& state) {
		return false;
	}

    virtual Setpoints generateSetpoints(const VehicleState &vehicle_state, float delta_time) {
        return Setpoints{};
    }
};



class MCAltControlFlightMode : public FlightMode {
public:
	static constexpr IdType MODE_ID = FlightMode::ID("MCAltControlFlightMode");

	virtual IdType getModeId() const {
		return MODE_ID;
	}

	static bool canRun(const VehicleState& state) {
		return false;
	}


    virtual Setpoints generateSetpoints(const VehicleState &vehicle_state, float delta_time) {
        return Setpoints{};
    }
};


union FlightModeUnion {
	FWOrbitFlightMode fw_orbit_flight_mode;
	MCPosControlFlightMode mc_pos_control_flight_mode;
	MCAltControlFlightMode mc_alt_control_flight_mode;
};


class FlightModeManager {

private:
	// State
	Vehicle& _vehicle;

	FlightModeUnion _current_flight_mode_memory;
	FlightMode* _current_flight_mode{reinterpret_cast<FlightMode*>(&_current_flight_mode_memory)};
	FlightModeSwitcher _flight_mode_switcher;

	FlightModeManager() : _flight_mode_switcher(&_current_flight_mode_memory) {};

	/**
	 * @brief Function that gets executed ~20Hz.
	 * Checks current FlightMode and assigns a new FlightMode
	 * in case of changes
	 *
	 * @param commander_state
	 */
	void _navigatorTick(const CommanderState &commander_state) {
		switch (commander_state) {
			case CommanderState::AUTO_LOITER: {
				auto result = _vehicle.modeForCommanderLoiter(*_current_flight_mode, _flight_mode_switcher);
                // depending on result, inform commander (switch to other mode), navigator(advance mission), or user (fail safes)
                break;
			}
			case CommanderState::ALTCTL: {
				auto result = _vehicle.modeForCommanderLoiter(*_current_flight_mode, _flight_mode_switcher);
                // depending on result, inform commander (switch to other mode), navigator(advance mission), or user (fail safes)				break;
			}
            //...
		}

	}
};


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






class Vehicle {
private:
	VehicleState _state{};


protected:

    /**
     * returns the vehicles current state. This object should include
     * all information to decide whether a FlightMode can execute or not, 
     * and to actually generate setpoints for executing it. 
     */
	const VehicleState& getState() const {
		return _state;
	}

public:
    /*
     * For each commander state / action as well as for each item in a mission, there
     * is an overwritable function in the vehicle. The function takes the current state
     * of the vehicle, the intention from user or mission, and selects an appropriate 
     * flight mode to execute that. A flight mode should only ever be chosen if it can
     * run. It is also expected that these functions / flight modes check geofence, 
     * object avoidance themselves as part of their canRun functions.
     */


	virtual FlightModeSelectionResult modeForCommanderLoiter(
		const FlightMode &current_flight_mode,
		FlightModeSwitcher &switcher
	) = 0;

	virtual FlightModeSelectionResult modeForCommanderAltCtl(
		const FlightMode& currentMode,
		FlightModeSwitcher &switcher
	) = 0;


    /*
     * After selection of a flight mode using the above functions,
     * This function is executed to check if the vehicle itself
     * has any fail safe states, such as battery emergencies. 
     * If this is the case, this function shall assign a flight
     * mode that reacts to that. 
     */
 	virtual FlightModeSelectionResult checkVehicleFailsafe(
		const FlightMode &current_flight_mode,
		FlightModeSwitcher &switcher
	) = 0;

    /*
     * Update the vehicle state by reading from the uORB messages 
     */
    void updateState() {
        _state.update();
    }

	// more overwritable functions, one for each possible commander state / action + mission item
};




class StandardTiltrotor : public Vehicle {

	virtual FlightModeSelectionResult modeForCommanderLoiter(const FlightMode &current_flight_mode, FlightModeSwitcher &switcher) {
		if (getState().vtol_configuration == VTOL_CONFIG::MULTICOPTER) {
			if (MCPosControlFlightMode::canRun(getState())) {
				bool switched = switcher.switchMode<MCPosControlFlightMode>(getState());
				return switched ? FlightModeSelectionResult::SWITCHED : FlightModeSelectionResult::KEPT_CURRENT;
			} else if (MCAltControlFlightMode::canRun(getState())) {
				bool switched = switcher.switchMode<MCAltControlFlightMode>(getState());
				return switched ? FlightModeSelectionResult::SWITCHED_DIFFERENT_COMMANDER_MODE : FlightModeSelectionResult::KEPT_CURRENT;
			}
			// more possibilities as we go here ..
		} else {
			if (FWOrbitFlightMode::canRun(getState())) {
				bool switched = switcher.switchMode<FWOrbitFlightMode>(getState());
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

