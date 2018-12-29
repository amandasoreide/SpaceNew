/**	
*	@file GlobalTiming.h
*	Global timing definitions, types and external resources

*	Prefix: SIGLBT_

*	@author Ramon Molla
*	@version 2015-11
*/


#ifndef SIGLB_GLOBALT_H
#define SIGLB_GLOBALT_H

#include <HRTimerManager.h>
#include <ExecutionMode.h>

typedef enum
	{
		SIGLBT_TEXTURE_ANIMAT_TIMER,	///<Timer for accounting the animation of textures
		SIGLBT_END_MAX_TIMERS
	} SIGLBT_TIMERS;

enum AppTimerType
{
	SIGLBT_APP_TIMING,		///<Timer for timing the running application. Global time
	SIGLBT_STEP_APP_TIMING,	///<Timer for timing the running application from the last step 
	SIGLBT_RTDESK_TIMING,	///<Timer to control calls to RT-DESK Engine Simulation.
	SIGLBT_UPDATE_TIMING,	///<Timer to measure the time of Update.
	SIGLBT_RENDER_TIMING,	///<Timer to measure the time of Render.
	SIGLBT_IDLE_TIMING,		///<Timer to measure the time of Idle.
	SIGLBT_RTDSKMM_TIMING,	///<Timer to control message handling time and time memory management.
	SIGLBT_FPS_TIMING,		///<Timer to control the Frame Per Seconds
	SIGLBT_TIMER_AMOUNT		///<Amount of global timers used all around the application. For management purpouses only
};

/** \class CSITimeAcc
	\brief Class Time Accounting
	
	Counts, accumulate, get averages,... on every significant aspect of the program: fps, idle timing, rendering timing,...
*/
class CSITimeAcc : public cHRTimerManager
{
#ifdef DEF_LEC_PARTIAL
#define DEF_LONG_ARRAYS 30000			//Longitud de los arrays utilizados para medir parciales
	double muestrasTotRnd[DEF_LONG_ARRAYS];
	int indTotRnd;

	double muestrasTotUpd[DEF_LONG_ARRAYS];
	int indTotUpd;

	double muestrasTotIdl[DEF_LONG_ARRAYS];
	int indTotIdl;

	double muestrasTotRtd[DEF_LONG_ARRAYS];
	int indTotRtd;

#endif
public:
	bool	DiscreteSimulation,		//Is simulation continuous or discrete?
			initReg;				//Semaphore used to start time accounting when a starting up time delay is accomplished

	unsigned int nFrames;			// Contador del número de frames visualizados
	unsigned int AllnFrames;		// Contador del número de frames visualizados
	float	fps;					// Velocidad actual de visualización (frames por segundo)
	//Percentages
	double	pct[SIGLBT_TIMER_AMOUNT];

	double	pctSuma;
	unsigned int	fpsContTotal;	//Variable que cuenta el total de frames procesados.
	double	fpsAcum;

	double	timeMsg;				//Tiempo Mensaje GDESK

	void calcFpsPct	(void);
	void Reset		(void);

	CSITimeAcc		();

	void EndAccCounting(AppTimerType indTmr);
};

extern CSITimeAcc TimerManager;

#endif
