#pragma once
#include "defines.h"
#include "Keyboard/Controls.h"


void EmptyFuncVV();

void EmptyFuncVpV(void *);

void EmptyFuncVII(int, int);

void EmptyFuncVI(int);

bool EmptyFuncBV();

bool EmptyFuncBKey(KeyEvent&);

void EmptyFuncVB(bool);

void EmptyFuncpVII(void *, int, int);

void LoggingFloatArray(float *data, int num);

void LoggingUint8Array(uint8 *data, int num);
