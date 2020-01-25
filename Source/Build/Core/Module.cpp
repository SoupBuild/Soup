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
#include "Definition/IValueTable.h"
#include "Definition/IValueList.h"
#include "Definition/IValuePrimitive.h"

#include "Wrapper/BuildStateWrapper.h"
#include "Wrapper/ValueListWrapper.h"
#include "Wrapper/ValuePrimitiveWrapper.h"
