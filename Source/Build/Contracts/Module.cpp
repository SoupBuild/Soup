module;

#include <stdint.h>

export module Soup.Build;
import Opal;

using namespace Opal;

#include "IBuildTask.h"
#include "IBuildSystem.h"
#include "IValueTable.h"
#include "IValueList.h"
#include "IValuePrimitive.h"
