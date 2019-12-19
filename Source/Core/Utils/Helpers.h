// <copyright file="Helpers.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Path.h"

namespace Soup
{
	export std::string EscapeString(std::string_view value)
	{
		std::stringstream stringBuilder;

		// TODO: THIS IS HORRIBLY WRONG! Utf8 could have multi-byte characters
		for (size_t i = 0; i < value.size(); i++)
		{
			// Add the escape character to all known escapes 
			// TODO: This is only a partial set
			if (value[i] == '\\' || value[i] == '"')
				stringBuilder << '\\';

			stringBuilder << value[i];
		}

		return stringBuilder.str();
	}

	export std::string ToString(const std::vector<std::string>& values)
	{
		std::stringstream stringBuilder;

		bool first = true;
		for (auto& value : values)
		{
			if (!first)
				stringBuilder << " ";
			
			stringBuilder << value;
			first = false;
		}

		return stringBuilder.str();
	}

	export std::string ToString(const std::vector<Path>& values)
	{
		std::stringstream stringBuilder;

		bool first = true;
		for (auto& value : values)
		{
			if (!first)
				stringBuilder << " ";
			
			stringBuilder << value.ToString();
			first = false;
		}

		return stringBuilder.str();
	}

	export std::string ToString(bool value)
	{
		return value ? "true" : "false";
	}

	export std::time_t CreateDateTime(int year, int month, int day, int hour, int minutes)
	{
		std::tm timeInfo = std::tm();

		// Year is offset from 1900
		timeInfo.tm_year = year - 1900;
		timeInfo.tm_mon = month;
		timeInfo.tm_mday = day;
		timeInfo.tm_hour = hour;
		timeInfo.tm_min = minutes;

		std::time_t time = std::mktime(&timeInfo);
		return time;
	}
}