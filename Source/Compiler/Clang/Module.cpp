module;

#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

export module SoupCompilerClang;

import Opal;
import SoupCore;
import SoupCompilerMSVC;

using namespace Opal;

#include "ArgumentBuilder.h"
#include "Compiler.h"
