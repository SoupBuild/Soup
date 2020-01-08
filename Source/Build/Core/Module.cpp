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

#include "Definition/IGraphNode.h"
#include "Definition/IBuildTask.h"
#include "Definition/IBuildSystem.h"

#include "Wrapper/BuildStateWrapper.h"
