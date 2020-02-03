module;

#include <any>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

export module SoupEngineCore;

import Opal;
import SoupEngineDefinition;
import SoupEngineWrapper;

using namespace Opal;

#include "BuildSystem.h"
#include "BuildState.h"
#include "ValueList.h"
#include "ValuePrimitive.h"
#include "ValueTable.h"
