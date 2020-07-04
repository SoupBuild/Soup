// <copyright file="Module.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

module;

#include <any>
#include <array>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module RecipeBuild;

import Opal;
import Soup.Build;
import Soup.Build.Utilities;
import Soup.Build.Evaluation;
import SoupCompiler;
import SoupCompilerClang;
import SoupCompilerMSVC;

using namespace Opal;

#include "Tasks/BuildTask.h"
#include "Tasks/RecipeBuildTask.h"
#include "Tasks/ResolveDependenciesTask.h"
#include "Tasks/ResolveToolsTask.h"
#include "Tasks/StandardLibraryIncludeTask.h"
