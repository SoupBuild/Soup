module;

#include <any>
#include <atomic>
#include <memory>
#include <string>
#include <vector>

export module SoupBuildExtension;
import Opal;

using namespace Opal;

#include "BuildGraphNode.h"
#include "IBuildTask.h"
#include "IBuildSystem.h"
