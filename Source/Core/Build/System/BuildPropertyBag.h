// <copyright file="BuildPropertyBag.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildPropertyValue.h"
#include "BuildStringList.h"

namespace Soup::Build
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	class BuildPropertyBag : public IPropertyBag
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildPropertyBag class
		/// </summary>
		BuildPropertyBag() :
			_propertyValues(),
			_propertyStringLists()
		{
		}

		/// <summary>
		/// Property access methods
		/// </summary>
		OperationResult TryHasPropertyValue(const char* name, bool& result) const noexcept override final
		{
			try
			{
				result = false;
				result = _propertyValues.contains(name);
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		OperationResult TryGetPropertyValue(const char* name, IPropertyValue*& result) noexcept override final
		{
			try
			{
				result = nullptr;
				auto findResult = _propertyValues.find(name);
				if (findResult != _propertyValues.end())
				{
					result = &findResult->second;
					return 0;
				}
				else
				{
					// The property does not exists
					return -3;
				}
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		OperationResult TryCreatePropertyValue(const char* name, IPropertyValue*& result) noexcept override final
		{
			try
			{
				result = nullptr;
				auto insertResult = _propertyValues.emplace(name, BuildPropertyValue());
				if (insertResult.second)
				{
					result = &insertResult.first->second;
					return 0;
				}
				else
				{
					// The property already exists
					return -2;
				}
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		OperationResult TryHasPropertyStringList(const char* name, bool& result) const noexcept override final
		{
			try
			{
				result = false;
				result = _propertyStringLists.contains(name);
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		OperationResult TryGetPropertyStringList(const char* name, IList<const char*>*& result) noexcept override final
		{
			try
			{
				result = nullptr;
				auto findResult = _propertyStringLists.find(name);
				if (findResult != _propertyStringLists.end())
				{
					result = &findResult->second;
					return 0;
				}
				else
				{
					// The property does not exists
					return -3;
				}
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		OperationResult TryCreatePropertyStringList(const char* name, IList<const char*>*& result) noexcept override final
		{
			try
			{
				result = nullptr;
				auto insertResult = _propertyStringLists.emplace(name, BuildStringList());
				if (insertResult.second)
				{
					result = &insertResult.first->second;
					return 0;
				}
				else
				{
					// The property already exists
					return -2;
				}
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		/// <summary>
		/// Internal access to the state
		/// </summary>
		const std::map<std::string, BuildPropertyValue>& GetPropertyValues() const
		{
			return _propertyValues;
		}

		const std::map<std::string, BuildStringList>& GetPropertyLists() const
		{
			return _propertyStringLists;
		}

		void Log()
		{
			auto stream = std::stringstream();
			stream << "BuildState\n";
			for (auto& propertyValue : _propertyValues)
			{
				stream << "\t" << propertyValue.first << "=\"" << propertyValue.second.ToString() << "\"\n";
			}

			for (auto& propertyList : _propertyStringLists)
			{
				stream << "\t" << propertyList.first << "=[";
				bool isFirst = true;
				for (auto& value : propertyList.second.GetValues())
				{
					if (!isFirst)
					{
						stream << ", ";
					}
					
					stream << "\"" << value << "\"";
					isFirst = false;
				}

				stream << "]\n";
			}

			Log::Diag(stream.str());
		}

	private:
		std::map<std::string, BuildPropertyValue> _propertyValues;
		std::map<std::string, BuildStringList> _propertyStringLists;
	};
}
