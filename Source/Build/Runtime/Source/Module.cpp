module;

// TODO: Add a converter level to Opal?
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <any>
#include <array>
#include <codecvt>
#include <ctime>
#include <locale>
#include <map>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <optional>
#include <set>
#include <stack>
#include <string>
#include <sstream>
#include <vector>

#include <Windows.h>

#ifdef max
#undef max
#endif

export module Soup.Build.Runtime;

import json11;
import Monitor.Shared;
import Opal;
import Soup.Build;
import Soup.Build.Utilities;

using namespace Opal;

#include "Contracts/ValuePrimitive.h"

#include "OperationGraph/OperationGraphGenerator.h"
#include "OperationGraph/OperationGraphManager.h"

#include "BuildEvaluateEngine.h"
#include "BuildGenerateEngine.h"
