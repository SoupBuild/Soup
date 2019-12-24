// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	class BuildState : public BuildEx::IBuildState
	{
	public:
		BuildState() :
			_properties()
		{
		}

		const std::any& GetProperty(const char* name) override final
		{
			return _properties.at(name);
		}

		void SetProperty(const char* name, std::any value) override final
		{
			_properties.insert_or_assign(name, std::move(value));
		}

	private:
		std::map<std::string, std::any> _properties;
	};
}
