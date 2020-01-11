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
import SoupBuildCore;
import json11;
import SoupCore;
import SoupCompilerClang;
import SoupCompilerMSVC;

using namespace Opal;

#include "Tasks/BuildTask.h"
#include "Tasks/RecipeBuildTask.h"
#include "Tasks/ResolveToolsTask.h"
#include "Tasks/StandardLibraryIncludeTask.h"
