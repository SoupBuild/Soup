import Detours;

#include <fstream>
#include <iostream>
#include <vector>

#include <Windows.h>

#define DllExport __declspec( dllexport )

export module Monitor.Detours;

#include "FileAccessReporter.h"
