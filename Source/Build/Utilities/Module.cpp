module;

#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

export module Soup.Build.Utilities;
import Opal;
import Soup.Build;

using namespace Opal;

#include "Contracts/ReadOnlyStringList.h"

#include "Wrapper/BuildStateWrapper.h"
#include "Wrapper/ValueListWrapper.h"
#include "Wrapper/ValuePrimitiveWrapper.h"

#include "SharedOperations.h"