﻿module;

#include <any>
#include <codecvt>
#include <ctime>
#include <locale>
#include <map>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <optional>
#include <set>
#include <stack>
#include <string>
#include <sstream>
#include <vector>

#include <Windows.h>

export module Soup.Build.Execute;

import json11;
import Monitor.Shared;
import Opal;
import Soup.Build;
import Soup.Build.Utilities;

using namespace Opal;

#include "BuildHistory.h"
#include "BuildHistoryChecker.h"
#include "BuildHistoryJson.h"
#include "BuildHistoryManager.h"
#include "BuildRunner.h"