// <copyright file="IBuildStateManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildState.h"
#include "Path.h"

namespace Soup
{
    /// <summary>
    /// The build state manager interface
    /// The individual implementations will store manage the build state for a specific directory
    /// Interface mainly used to allow for unit testing client code
    /// </summary>
    class IBuildStateManager
    {
    public:
        /// <summary>
        /// Load the build state from the provided directory
        /// </summary>
        virtual bool TryLoadState(const Path& directory, BuildState& result) = 0;

        /// <summary>
        /// Save the build state for the provided directory
        /// </summary>
        virtual void SaveState(const Path& directory, const BuildState& state) = 0;
    };
}
