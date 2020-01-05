module;

#include <any>
#include <atomic>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

export module SoupBuildCore;
import Opal;

using namespace Opal;

#include "Definition/BuildGraphNode.h"
#include "Definition/IBuildTask.h"
#include "Definition/IBuildSystem.h"

#include "Wrapper/PropertyBagWrapper.h"
