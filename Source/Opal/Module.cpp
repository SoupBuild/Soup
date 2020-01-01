module;
#include <algorithm>
#include <array>
#include <atomic>
#include <fstream>
#include <filesystem>
#include <locale>
#include <map>
#include <sstream>
#include <string>

#include <Windows.h>
#include <psapi.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
export module Opal;

#include "Core/Path.h"
#include "Core/SemanticVersion.h"

#include "Memory/IReferenceCounted.h"
#include "Memory/Reference.h"
#include "Memory/ReferenceCounted.h"

#include "System/SmartHandle.h"
#include "System/DynamicLibraryManager.h"
#include "System/ProcessManager.h"
#include "System/MockEnvironment.h"
#include "System/MockFileSystem.h"
#include "System/MockProcessManager.h"
#include "System/PlatformProcessManager.h"
#include "System/ScopedFileSystemRegister.h"
#include "System/ScopedProcessManagerRegister.h"
#include "System/STLEnvironment.h"
#include "System/STLFileSystem.h"
