module;

#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

export module SoupCompilerMSVC;

import Dubious;
import SoupCore;

using namespace Dubious;

#include "ArgumentBuilder.h"
#include "Compiler.h"
