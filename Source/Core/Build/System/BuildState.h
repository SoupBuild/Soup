// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildPropertyValue.h"
#include "BuildPropertyList.h"

namespace Soup::Build
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	class BuildState : public IBuildState
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildState class
		/// </summary>
		BuildState() :
			_graph(),
			_propertyValues(),
			_propertyStringLists()
		{
		}

		/// <summary>
		/// Build Graph Access Methods
		/// </summary>
		void AddBuildNode(std::shared_ptr<BuildGraphNode> node) noexcept override final
		{
			_graph.GetNodes().push_back(std::move(node));
		}
		
		/// <summary>
		/// Property access methods
		/// </summary>
		BuildSystemResult TryHasPropertyValue(const char* name, bool& result) const noexcept override final
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

		BuildSystemResult TryGetPropertyValue(const char* name, IBuildPropertyValue*& result) noexcept override final
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

		BuildSystemResult TryCreatePropertyValue(const char* name, IBuildPropertyValue*& result) noexcept override final
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

		BuildSystemResult TryHasPropertyStringList(const char* name, bool& result) const noexcept override final
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

		BuildSystemResult TryGetPropertyStringList(const char* name, IBuildPropertyList<const char*>*& result) noexcept override final
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

		BuildSystemResult TryCreatePropertyStringList(const char* name, IBuildPropertyList<const char*>*& result) noexcept override final
		{
			try
			{
				result = nullptr;
				auto insertResult = _propertyStringLists.emplace(name, BuildPropertyList<const char*>());
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
		/// Internal access to build nodes
		/// </summary>
		std::vector<std::shared_ptr<BuildGraphNode>>& GetBuildNodes()
		{
			return _graph.GetNodes();
		}

		const std::vector<std::shared_ptr<BuildGraphNode>>& GetBuildNodes() const
		{
			return _graph.GetNodes();
		}

	private:
		BuildGraph _graph;
		std::map<std::string, BuildPropertyValue> _propertyValues;
		std::map<std::string, BuildPropertyList<const char*>> _propertyStringLists;
	};
}
