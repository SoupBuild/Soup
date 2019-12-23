module;

#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

export module SoupCompilerClang;

import Dubious;
import SoupCore;
import SoupCompilerMSVC;

using namespace Dubious;

#include "ArgumentBuilder.h"
#include "Compiler.h"
