#pragma once

#include "base/types.h"

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif

extern "C" NEGINE_API PyObject* PyInit_negine_core();

