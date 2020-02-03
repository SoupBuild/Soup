module;

#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

export module SoupEngineWrapper;
import Opal;
import SoupEngineDefinition;

using namespace Opal;

#include "BuildStateWrapper.h"
#include "GraphNodeExtensions.h"
#include "ValueListWrapper.h"
#include "ValuePrimitiveWrapper.h"
