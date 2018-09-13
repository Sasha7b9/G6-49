#include "Signals.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Parameter paramsSineModulationA[] =
{
    Parameter(Parameter::Amplitude, 0.0f, 65535.0f, "65535", 1, Order::Kilo),
    Parameter(Parameter::Duration,  0.0f, 255.0f,   "25500", 2, Order::One)
};

static Parameter params_SineA[Parameter::Number] =
{
    Parameter(Parameter::Frequency,          0.1f,    100e6f, "15000", 0, Order::Kilo),
    Parameter(Parameter::Period,             100e-8f, 100e6f, "05000", 0, Order::Milli),
    Parameter(Parameter::Amplitude,          0.0f,    10.0f,  "50000", 0, Order::One),
    Parameter(Parameter::Offset,             0.0f,    10.0f,  "50000", 0, Order::One),
    Parameter(Parameter::ModulationRampSine, -1.0f,    -1.0f, "",      0, Order::One, paramsSineModulationA, 2, NUM_DIGITS - 1, ' ')
};

static Parameter  params_RampPlusA[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f, "10000",  0, Order::Kilo),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f,  "10000",  0, Order::Milli),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f,  "10000",  1, Order::One),
    Parameter(Parameter::Offset,    0.0f,    10.0f,  "50000",  0, Order::One),
};

static Parameter params_RampMinusA[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One),
    Parameter(Parameter::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};

static Parameter params_MeanderA[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f,  "10000", 0, Order::Kilo),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f,   "10000", 0, Order::Milli),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f,   "10000", 1, Order::One),
    Parameter(Parameter::Offset,    0.0f,    10.0f,   "50000", 0, Order::One)
};

static Parameter params_ImpulseA[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One),
    Parameter(Parameter::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static Form formsA[] =
{
    Form(Form::Sine,      params_SineA,      5),
    Form(Form::RampPlus,  params_RampPlusA,  4),
    Form(Form::RampMinus, params_RampMinusA, 4),
    Form(Form::Meander,   params_MeanderA,   4),
    Form(Form::Impulse,   params_ImpulseA,   4)
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Parameter paramsSineModulationB[] =
{
    Parameter(Parameter::Amplitude, 0.0f, 65535.0f, "65535", 1, Order::Kilo),
    Parameter(Parameter::Duration,  0.0f, 255.0f,   "25500", 2, Order::One)
};

static Parameter params_SineB[Parameter::Number] =
{
    Parameter(Parameter::Frequency,          0.1f,    100e6f, "40000", 0, Order::Kilo),
    Parameter(Parameter::Period,             100e-8f, 100e6f, "10000", 0, Order::Milli),
    Parameter(Parameter::Amplitude,          0.0f,    10.0f,  "10000", 1, Order::One),
    Parameter(Parameter::Offset,             0.0f,    10.0f,  "50000", 0, Order::One),
    Parameter(Parameter::Phase,              0.0f,    360.0f, "10000", 0, Order::One),
    Parameter(Parameter::ModulationRampSine, -1.0f,   -1.0f,  "10000", 0, Order::One, paramsSineModulationB, 2)
};

static Parameter  params_RampPlusB[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f,  "50000", 0, Order::One),
    Parameter(Parameter::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};

static Parameter params_RampMinusB[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f,  "50000", 0, Order::One),
    Parameter(Parameter::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};

static Parameter params_MeanderB[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One),
    Parameter(Parameter::Offset,    0.0f,    10.0f,  "50000", 0, Order::One)
};

static Parameter params_ImpulseB[Parameter::Number] =
{
    Parameter(Parameter::Frequency, 0.1f,    100e6f, "10000", 0, Order::Kilo),
    Parameter(Parameter::Period,    3.3e-8f, 10e6f,  "10000", 0, Order::Milli),
    Parameter(Parameter::Amplitude, 0.0f,    10.0f,  "10000", 1, Order::One),
    Parameter(Parameter::Offset,    0.0f,    10.0f,  "50000", 0, Order::One),
};


//----------------------------------------------------------------------------------------------------------------------------------------------------
static Form formsB[] =
{
    Form(Form::Sine,        params_SineB,      6),
    Form(Form::RampPlus,    params_RampPlusB,  4),
    Form(Form::RampMinus,   params_RampMinusB, 4),
    Form(Form::Meander,     params_MeanderB,   4),
    Form(Form::Impulse,     params_ImpulseB,   4)
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Wave wave[Chan::Number] =
{
    Wave(Chan::A, formsA, 5),
    Wave(Chan::B, formsB, 5)
};
