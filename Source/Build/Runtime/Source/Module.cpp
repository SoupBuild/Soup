module;

// TODO: Add a converter level to Opal?
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <any>
#include <array>
#include <algorithm>
#include <codecvt>
#include <ctime>
#include <iomanip>
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

#include <openssl/evp.h>

export module Soup.Build.Runtime;

import json11;
import toml11;
import Monitor.Host;
import Opal;

using namespace Opal;

#include "Contracts/ValuePrimitive.h"
#include "Contracts/ValueTableManager.h"

#include "LocalUserConfig/LocalUserConfig.h"
#include "LocalUserConfig/LocalUserConfigExtensions.h"

#include "OperationGraph/OperationGraphManager.h"

#include "PathList/PathListManager.h"

#include "Recipe/Recipe.h"
#include "Recipe/RecipeBuildStateConverter.h"
#include "Recipe/RecipeExtensions.h"
#include "Recipe/RecipeToml.h"

#include "BuildConstants.h"
#include "BuildEvaluateEngine.h"
#include "Sha3_256.h"