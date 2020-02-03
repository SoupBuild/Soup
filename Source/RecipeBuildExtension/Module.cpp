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

export module RecipeBuildExtension;

import Opal;
import json11;
import SoupEngineDefinition;
import SoupEngineWrapper;
import SoupEngineCore;
import SoupCore;
import SoupCompilerClang;
import SoupCompilerMSVC;

using namespace Opal;

#include "Tasks/BuildTask.h"
#include "Tasks/RecipeBuildTask.h"
#include "Tasks/ResolveToolsTask.h"
#include "Tasks/StandardLibraryIncludeTask.h"
