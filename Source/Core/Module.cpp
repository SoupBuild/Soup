module;

#include <any>
#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <optional>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module SoupCore;

import Opal;
import SoupBuildCore;
import json11;
import toml11;

using namespace Opal;

#include "Logger\Log.h"

#include "Utils\Helpers.h"
#include "Utils\HandledException.h"

#include "Build\Runner\BuildHistory.h"
#include "Build\Runner\BuildHistoryChecker.h"
#include "Build\Runner\BuildHistoryJson.h"
#include "Build\Runner\BuildHistoryManager.h"
#include "Build\Runner\BuildRunner.h"

#include "Build\System\BuildState.h"
#include "Build\System\GraphNodeExtensions.h"
#include "Build\System\ValueList.h"
#include "Build\System\ValuePrimitive.h"
#include "Build\System\ValueTable.h"

#include "Build\BuildUtilities.h"

#include "Compiler\ICompiler.h"
#include "Compiler\CompileArguments.h"
#include "Compiler\MockCompiler.h"

#include "Config\LocalUserConfigExtensions.h"
#include "Config\LocalUserConfigJson.h"

#include "Logger\ConsoleTraceListener.h"
#include "Logger\ScopedTraceListenerRegister.h"
#include "Logger\TestTraceListener.h"

#include "Package\Recipe.h"
#include "Package\RecipeBuildManager.h"
#include "Package\RecipeExtensions.h"
#include "Package\RecipeJson.h"
#include "Package\RecipeToml.h"
