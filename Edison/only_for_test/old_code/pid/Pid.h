#include <stdint.h>
typedef enum			//!< Enumerates the controller direction modes
{
	PID_DIRECT,			//!< Direct drive (+error gives +output)
	PID_REVERSE			//!< Reverse driver (+error gives -output)
} ctrlDir_t;

typedef struct
{
	double zKp;					//!< Time-step scaled proportional constant for quick calculation (equal to actualKp)
	double zKi;					//!< Time-step scaled integral constant for quick calculation
	double zKd;					//!< Time-step scaled derivative constant for quick calculation
	double actualKp;			//!< Actual (non-scaled) proportional constant
	double actualKi;			//!< Actual (non-scaled) integral constant
	double actualKd;			//!< Actual (non-scaled) derivative constant
	double prevInput;			//!< Actual (non-scaled) proportional constant
	double inputChange;			//!< The change in input between the current and previous value
	double setPoint;			//!< The set-point the PID control is trying to make the output converge to.
	double error;				//!< The error between the set-point and actual output (set point - output, positive
	//!< when actual output is lagging set-point
	double output;				//!< The control output. This is updated when Pid_Go() is called
	double prevOutput;			//!< The output value calculated the previous time Pid_Go was called
	double samplePeriodMs;		//!< The sample period (in milliseconds) between successive Pid_Go() calls.
	//!< The constants with the z prefix are scaled according to this value.
	double pTerm;				//!< The proportional term that is summed as part of the output (calculated in Pid_Go())
	double iTerm;				//!< The integral term that is summed as part of the output (calculated in Pid_Go())
	double dTerm;				//!< The derivative term that is summed as part of the output (calculated in Pid_Go())
	double outMin;				//!< The minimum output value. Anything lower will be limited to this floor.
	double outMax;				//!< The maximum output value. Anything higher will be limited to this ceiling.
	ctrlDir_t controllerDir;	//!< The control direction for the PID instance.
} pidData_t;

//initialization
void Pid_Init(
	pidData_t *pidData,
	double kp,
	double ki,
	double kd,
	ctrlDir_t controllerDir,
	uint32_t samplePeriodMs);

//Computes new PID values
//Call once per sampleTimeMs. Output is stored in the pidData structure.
void 	Pid_Run(pidData_t *pidData, double input);

//Returns the last calculated error
double 	Pid_GetError(pidData_t *pidData);

void	Pid_SetSetPoint(pidData_t *pidData, double setPoint);

//The PID will either be connected to a direct acting process (+error leads to +output, aka inputs are positive) 
//or a reverse acting process (+error leads to -output, aka inputs are negative)
void 	Pid_SetControllerDirection(pidData_t *pidData, ctrlDir_t controllerDir);

//Changes the sample time
void 	Pid_SetSamplePeriod(pidData_t *pidData, uint32_t newSamplePeriodMs);

//Returns the porportional constant
double 	Pid_GetKp(pidData_t *pidData);

//Returns the integral constant				
double 	Pid_GetKi(pidData_t *pidData);

//Returns the derivative constant				 
double 	Pid_GetKd(pidData_t *pidData);

//Returns the direction		  
ctrlDir_t Pid_GetDirection(pidData_t *pidData);

//Returns the last calculated derivative term				
double 	Pid_GetDTerm(pidData_t *pidData);

//Returns the last calculated input change
double 	Pid_GetInputChange(pidData_t *pidData);

//This function allows the controller's dynamic performance to be adjusted. 
//It's called automatically from the init function, but tunings can also
//be adjusted on the fly during normal operation
void 	Pid_SetTunings(pidData_t *pidData, double kp, double ki, double kd);
void	Pid_SetOutputLimits(pidData_t *pidData, double min, double max);