
export module Soup.Core;

import std.core;
import json11;

#include "Build\BuildEngine.h"
#include "Build\BuildStateJson.h"
#include "Compiler\CompileArguments.h"
#include "Compiler\MockCompiler.h"
#include "Logger\Log.h"
#include "Logger\ConsoleTraceListener.h"
#include "Logger\TestTraceListener.h"
#include "Package\Recipe.h"
#include "Package\RecipeExtensions.h"
#include "Package\RecipeJson.h"
#include "Utils\Path.h"
#include "Utils\SemanticVersion.h"
