module;

#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

export module Soup.Build.Extensions;
import Opal;
import Soup.Build;

using namespace Opal;

#include "BuildStateWrapper.h"
#include "GraphNodeExtensions.h"
#include "ValueListWrapper.h"
#include "ValuePrimitiveWrapper.h"

#include "RecipeLanguageVersion.h"
#include "RecipeType.h"
