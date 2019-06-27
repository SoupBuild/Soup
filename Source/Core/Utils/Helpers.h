// <copyright file="Helpers.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once

namespace Soup
{
    std::string ToString(bool value)
    {
        return value ? "true" : "false";
    }
}