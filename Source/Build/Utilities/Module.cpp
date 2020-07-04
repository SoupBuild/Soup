module;

#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

export module Soup.Build.Utilities;
import Opal;
import Soup.Build;

using namespace Opal;

#include "Runtime/BuildOperation.h"
#include "Runtime/StringList.h"

#include "Wrapper/BuildStateWrapper.h"
#include "Wrapper/ValueListWrapper.h"
#include "Wrapper/ValuePrimitiveWrapper.h"

#include "BuildOperationExtensions.h"

#include "RecipeLanguageVersion.h"
#include "RecipeType.h"