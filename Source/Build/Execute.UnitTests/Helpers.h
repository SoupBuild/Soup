// <copyright file="BuildHistoryCheckerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	std::time_t CreateDateTime(int year, int month, int day, int hour, int minutes)
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