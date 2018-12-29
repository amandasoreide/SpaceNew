/**	
*	@file GlobalTiming.cpp
*	Global timing definitions, types and external resources

*	Prefix: SIGLBT_

*	@author Ramon Molla
*	@version 2015-11
*/

#include <GlobalTiming.h>

#define SIGLBT_FPS_CYCLE 40		// Actualizar el contador de fps cada N frames

///Default timing periods for every aspect of the system
const float DefaultPeriods[SIGLBT_TIMER_AMOUNT] =
{
	0.0,	///<Timing the running application. Global time. Not applicable
	0.0,	///<Timing the running application from the last step. Not applicable
	0.0,	///<Time to control calls to RT-DESK Engine Simulation. Not applicable
	16.666,	///<Time elapsed from one Update to another one.
	16.666,	///<Time elapsed from one Render to another one.
	0.0,	///<Timing the Idle time. Not applicable
	0.0,	///<Control message handling time and time memory management.
	0.0		///<Frame Per Seconds accounting
};

CSITimeAcc::CSITimeAcc(){Reset();}

void CSITimeAcc::EndAccCounting(AppTimerType indTmr)
{
	Timers[indTmr].AccumulateUntilNow();

#ifdef DEF_EXP_CSV_FILE
	if (!initReg)
	{
		if (DEF_MIN_TICKS < Timers[SIGLBT_APP_TIMING].TicksUntilNow())
			initReg = true;
	}
	else switch (indTmr)
		{
		case SIGLBT_UPDATE_TIMING:
#ifdef DEF_LEC_PARTIAL
			muestrasTotUpd[indTotUpd++] = Timers[SIGLBT_UPDATE_TIMING].GetLastPeriodms();
#endif
			break;

		case SIGLBT_RENDER_TIMING:
#ifdef DEF_LEC_PARTIAL
			muestrasTotRnd[indTotRnd++] = Timers[SIGLBT_RENDER_TIMING].GetLastPeriodms();
#endif
			break;

		case SIGLBT_IDLE_TIMING:
#ifdef DEF_LEC_PARTIAL
			muestrasTotIdl[indTotIdl++] = Timers[SIGLBT_IDLE_TIMING].GetLastPeriodms();
#endif
			break;

		case SIGLBT_RTDSKMM_TIMING:
#ifdef DEF_LEC_PARTIAL
			muestrasTotRtd[indTotRtd++] = Timers[SIGLBT_RTDSKMM_TIMING].GetLastPeriodms();
#endif
			break;
		}
#endif
}

// Función para calcular y mostrar por pantalla el conteo de frames por segundo
void CSITimeAcc::calcFpsPct(void)
{
	//Percentage in 
#define SIGLBT_HISTORY 0.4
#define SIGLBT_LATEST  0.6

	double	ms[SIGLBT_TIMER_AMOUNT];	//Miliseconds used to render, to update, to do nothing, to discrete simulation,... since last checking

	unsigned int i;

	//********************************* FPS Calculus ***********************************
	//Sampling period is set to SIGLBT_FPS_CYCLE frames. It means that every SIGLBT_FPS_CYCLE frames, everything is calculated
	nFrames++;
	if (nFrames == SIGLBT_FPS_CYCLE)
	{
		//Store and reset frames counters
		AllnFrames += SIGLBT_FPS_CYCLE;
		nFrames = 0;

		//*********************************Timing Calculation***********************************
		//Do not touch global app timing
		for (i = SIGLBT_STEP_APP_TIMING; i < SIGLBT_TIMER_AMOUNT; i++)
		{
			Timers[i].AccumulateUntilNow();
			ms[i] = Timers[i].GetLastPeriodms();
			pct[i] = pct[i] * SIGLBT_HISTORY + SIGLBT_LATEST*(ms[i] / ms[SIGLBT_STEP_APP_TIMING])*100.0;
		}

		fps = SIGLBT_FPS_CYCLE*1000.0 / ms[SIGLBT_STEP_APP_TIMING];

#ifdef DEF_EXP_CSV_FILE
		if (TimerManager.initReg) {
			fpsAcum += fps;
			fpsContTotal++;
		}
#endif
		pctSuma = pct[SIGLBT_UPDATE_TIMING] + pct[SIGLBT_RENDER_TIMING] + pct[SIGLBT_IDLE_TIMING];

		if (DiscreteSimulation)
			pctSuma += pct[SIGLBT_RTDESK_TIMING];
	}
}

void CSITimeAcc::Reset(void)
{
	initReg = false;

#ifdef DEF_LEC_PARTIAL
	indTotRnd = 0;
	indTotUpd = 0;
	indTotIdl = 0;
	indTotRtd = 0;
#endif

	nFrames		 = 0;
	fps			 = 0.0;
	fpsContTotal = 0;
	fpsAcum		 = 0.0;
	AllnFrames	 = 0;

	Timers.resize(SIGLBT_TIMER_AMOUNT);
	ResetSamplingFrequency(HRTM_SLOW);

	for (unsigned int i = SIGLBT_APP_TIMING; i < SIGLBT_TIMER_AMOUNT; i++)
	{
		Timers[i].Reset();
		Timers[i].SetSF(SF);
		pct[i] = 0.0;
	}

	//Set timing for the Update and Render phases
	Timers[SIGLBT_UPDATE_TIMING].SetAlarm(DefaultPeriods[SIGLBT_UPDATE_TIMING]);
	Timers[SIGLBT_RENDER_TIMING].SetAlarm(DefaultPeriods[SIGLBT_RENDER_TIMING]);
}