﻿module;

#include <array>
#include <ctime>
#include <iostream>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module SoupCore;

import Dubious;
import SoupSyntax;
import json11;

using namespace Dubious;

#include "Logger\Log.h"
#include "Utils\Helpers.h"
#include "Utils\HandledException.h"

#include "Build\BuildEngine.h"
#include "Build\BuildStateChecker.h"
#include "Build\BuildStateJson.h"
#include "Build\BuildStateManager.h"
#include "Compiler\CompileArguments.h"
#include "Compiler\MockCompiler.h"
#include "Config\LocalUserConfigExtensions.h"
#include "Config\LocalUserConfigJson.h"
#include "Logger\ConsoleTraceListener.h"
#include "Logger\TestTraceListener.h"
#include "Package\Recipe.h"
#include "Package\RecipeBuildManager.h"
#include "Package\RecipeExtensions.h"
#include "Package\RecipeJson.h"
