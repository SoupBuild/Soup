module;

#include <any>
#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <regex>
#include <optional>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

export module Soup.Core;

import Opal;
import Soup.Build;
import Soup.Build.Utilities;
import Soup.Build.Runtime;
import toml11;

using namespace Opal;

#include "Utils/Helpers.h"
#include "Utils/HandledException.h"

#include "Package/PackageManager.h"
#include "Package/RecipeBuildRunner.h"
