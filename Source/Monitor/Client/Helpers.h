#pragma once

bool EndsWithIgnoreCase(std::wstring_view lhs, std::wstring_view value)
{
	auto lhsLength = lhs.size();
	auto valueLength = value.size();
	if (lhsLength < valueLength)
		return false;
	
	auto offset = lhsLength - valueLength;
	auto lhsEnd = lhs.substr(offset, valueLength);
	return std::equal(
		lhsEnd.begin(),
		lhsEnd.end(),
		value.begin(),
		value.end(),
		[](wchar_t a, wchar_t b)
		{
			return std::tolower(a) == std::tolower(b);
		});
}

bool EndsWithIgnoreCase(std::string_view lhs, std::string_view value)
{
	auto lhsLength = lhs.size();
	auto valueLength = value.size();
	if (lhsLength < valueLength)
		return false;
	
	auto offset = lhsLength - valueLength;
	auto lhsEnd = lhs.substr(offset, valueLength);
	return std::equal(
		lhsEnd.begin(),
		lhsEnd.end(),
		value.begin(),
		value.end(),
		[](char a, char b)
		{
			return std::tolower(a) == std::tolower(b);
		});
}
