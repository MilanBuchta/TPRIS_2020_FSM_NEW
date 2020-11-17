/*
 * FSM.h
 *
 *  Created on: 10. 11. 2020
 *      Author: martin
 */

#ifndef GATECONTROL_H_
#define GATECONTROL_H_
#include <fsl_debug_console.h>
#include "etl/state_chart.h"
#include "etl/array.h"
#include "fsl_pit.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include <stdio.h>

struct Events {
	enum {
		S1, S2, CMD_CLOSE, CMD_OPEN, TIME_OUT, INIT,CMD_ENERGENCY
	};
};

//***************************************************************************
// States
//***************************************************************************
struct States {
	enum {
		CLOSE, OPEN, IDLE, ERROR,EMERGENCY
	};
};


//***********************************
// The Gate control FSM.
//***********************************

#define LED_OFF() GPIO_SetPinsOutput(BOARD_INITPINS_LED_BLUE_GPIO, BOARD_INITPINS_LED_BLUE_GPIO_PIN_MASK)
#define LED_ON() GPIO_ClearPinsOutput(BOARD_INITPINS_LED_BLUE_GPIO, BOARD_INITPINS_LED_BLUE_GPIO_PIN_MASK)

class GateControl: public etl::state_chart<GateControl> {
public:
	GateControl() :
			etl::state_chart<GateControl>(*this, transitionTable.data(),
					transitionTable.data() + transitionTable.size(),
					stateTable.data(), stateTable.data() + stateTable.size(),
					States::IDLE) {
	}
//***********************************
	void OnTranToClose() {
		PRINTF("%s\n", __func__);

	}
//***********************************
	void OnTranToIdle() {
		PRINTF("%s\n", __func__);

	}
//***********************************
	void OnTranToOpen() {
		PRINTF("%s\n", __func__);
	}
//***********************************
	void OnTranToError() {
		PRINTF("%s\n", __func__);
	}
//***********************************
	void OnEnterIdle() {
		PRINTF("%s\n",__func__);

			LED_ON();

	}
//***********************************
	void OnExitIdle() {
		PRINTF("%s\n",__func__);
		LED_OFF();

	}
//**********************************
	void OnEnterOpen() {
		PRINTF("%s\n",__func__);
		PIT_StartTimer(PIT, kPIT_Chnl_0);
	}
//**********************************
	void OnExitOpen() {
		PRINTF("%s\n",__func__);
		PIT_StopTimer(PIT, kPIT_Chnl_0);
	}
//***********************************
	void OnEnterClose() {
		PRINTF("%s\n",__func__);
		PIT_StartTimer(PIT, kPIT_Chnl_0);
	}
//***********************************
	void OnExitClose() {
		PRINTF("%s\n",__func__);
		PIT_StopTimer(PIT, kPIT_Chnl_0);
	}
//***********************************
	bool Guard() {
		PRINTF("%s\n",__func__);
		return guard;
	}
//***********************************
	bool NotGuard() {
		PRINTF("%s\n",__func__);
		return !guard;
	}

	bool GuardS1AndS2() {
		PRINTF("%s\n",__func__);
		return S1 || S2;
	}

	bool GuardS1NotS2() {
		PRINTF("%s\n",__func__);
		return !S1 && !S2;
	}

	void OnEnterEmergency() {
		PRINTF("%s\n",__func__);
	}

	void OnExitEmergency() {
		PRINTF("%s\n",__func__);
	}

	void OnTranToEmergency() {
		PRINTF("%s\n",__func__);
	}

	void OnEnterError() {
		PRINTF("%s\n",__func__);

	}

	void OnExitError() {
		PRINTF("%s\n",__func__);
	}

	void delay_pit(uint32_t t) {
		volatile uint32_t i = 0;
		for (i = 0; i < 150000 * t; ++i) { //200000 -> 200ms // 20Mhz clozk
			__asm("NOP");
			/* delay */
		}
	}

//***********************************
	void Null() {
//		std::cout << "Action: Null\n";
	}
	bool guard;
	bool S1 = false;
	bool S2 = false;
	static const etl::array<GateControl::transition,11> transitionTable;
	static const etl::array<GateControl::state, 5> stateTable;
};
#endif /* GATECONTROL_H_ */
