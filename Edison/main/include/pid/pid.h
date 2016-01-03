#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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
void Pid_Run(pidData_t *pidData, double input, int n_print_flag);

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

//.c

void Pid_Init(
        pidData_t *pidData,
        double kp, double ki, double kd,
        ctrlDir_t controllerDir,
        uint32_t samplePeriodMs)
{
    // Pid_SetOutputLimits(pidData, 0.0, 100.0);
    Pid_SetOutputLimits(pidData, -0.2, 0.2);

    pidData->samplePeriodMs = samplePeriodMs;				// Default controller sample time is 0.1 seconds

    Pid_SetControllerDirection(pidData, controllerDir);
    Pid_SetTunings(pidData, kp, ki, kd);
    pidData->prevInput = 0;
    pidData->prevOutput = 0;
}

void Pid_Run(pidData_t *pidData, double input, int n_print_flag)
{
    // Compute all the working error variables
    //double input = *_input;

    pidData->error = pidData->setPoint - input;

    // Integral calcs

    pidData->iTerm += (pidData->zKi * pidData->error);
    // Perform min/max bound checking on integral term
    if (pidData->iTerm > pidData->outMax)
        pidData->iTerm = pidData->outMax;
    else if (pidData->iTerm < pidData->outMin)
        pidData->iTerm = pidData->outMin;

    pidData->inputChange = (input - pidData->prevInput);
    pidData->dTerm = -pidData->zKd*pidData->inputChange;

    // Compute PID Output
    pidData->output = pidData->prevOutput + pidData->zKp*pidData->error + pidData->iTerm + pidData->dTerm;

    if (pidData->output > pidData->outMax)
        pidData->output = pidData->outMax;
    else if (pidData->output < pidData->outMin)
        pidData->output = pidData->outMin;

    // Remember input value to next call
    pidData->prevInput = input;
    // Remember last output for next call
    pidData->prevOutput = pidData->output;
    if (n_print_flag == 1)
    {
        printf("error=%f\tiTerm=%f\tdTerm=%f\n", pidData->error, pidData->iTerm, pidData->dTerm);
    }

}

void Pid_SetSetPoint(pidData_t *pidData, double setPoint)
{
    pidData->setPoint = setPoint;
}


void Pid_SetTunings(pidData_t *pidData, double kp, double ki, double kd)
{
    if (kp<0 || ki<0 || kd<0)
        return;

    pidData->actualKp = kp;
    pidData->actualKi = ki;
    pidData->actualKd = kd;

    double sampleTimeInSec = ((double)pidData->samplePeriodMs) / 1000.0;

    // Calculate time-step-scaled PID terms
    pidData->zKp = kp;
    pidData->zKi = ki * sampleTimeInSec;
    pidData->zKd = kd / sampleTimeInSec;

    if (pidData->controllerDir == PID_REVERSE)
    {
        pidData->zKp = (0 - pidData->zKp);
        pidData->zKi = (0 - pidData->zKi);
        pidData->zKd = (0 - pidData->zKd);
    }
}


void Pid_SetSamplePeriod(pidData_t *pidData, uint32_t newSamplePeriodMs)
{
    if (newSamplePeriodMs > 0)
    {
        double ratio = (double)newSamplePeriodMs
            / (double)pidData->samplePeriodMs;
        pidData->zKi *= ratio;
        pidData->zKd /= ratio;
        pidData->samplePeriodMs = newSamplePeriodMs;
    }
}

void Pid_SetOutputLimits(pidData_t *pidData, double min, double max)
{
    if (min >= max)
        return;
    pidData->outMin = min;
    pidData->outMax = max;

}


void Pid_SetControllerDirection(pidData_t *pidData, ctrlDir_t controllerDir)
{
    if (controllerDir != pidData->controllerDir)
    {
        // Invert control constants
        pidData->zKp = (0 - pidData->zKp);
        pidData->zKi = (0 - pidData->zKi);
        pidData->zKd = (0 - pidData->zKd);
    }
    pidData->controllerDir = controllerDir;
}


double Pid_GetKp(pidData_t *pidData)
{
    return pidData->actualKp;
}


double Pid_GetKi(pidData_t *pidData)
{
    return pidData->actualKi;
}


double Pid_GetKd(pidData_t *pidData)
{
    return pidData->actualKd;
}


ctrlDir_t Pid_GetDirection(pidData_t *pidData)
{
    return pidData->controllerDir;
}


double Pid_GetError(pidData_t *pidData)
{
    return pidData->error;
}


double Pid_GetDTerm(pidData_t *pidData)
{
    return pidData->dTerm;
}


double Pid_GetInputChange(pidData_t *pidData)
{
	return pidData->inputChange;
}

float Pid_rm_noise(float normGyro)
{
    int tmp=0;
    tmp = (int)(normGyro*500);
    return (float)tmp/500.0;
}
