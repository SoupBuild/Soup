// <copyright file="BuildStateWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Definition/IBuildState.h"
#include "BuildPropertyValueWrapper.h"
#include "BuildPropertyListWrapper.h"

namespace Soup::Build
{
	/// <summary>
	/// Build State Extension used to make the IBuildState interface more usable
	/// </summary>
	export class BuildStateWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildStateWrapper class
		/// </summary>
		BuildStateWrapper(IBuildState& state) :
			_state(state)
		{
		}

		/// <summary>
		/// Build Graph Access Methods
		/// </summary>
		void AddBuildNode(std::shared_ptr<BuildGraphNode> node)
		{
			_state.AddBuildNode(std::move(node));
		}
		
		/// <summary>
		/// Property access methods
		/// </summary>
		bool HasPropertyValue(std::string_view name) const
		{
			bool result = false;
			auto status = _state.TryHasPropertyValue(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryHasPropertyValue Failed");

			return result;
		}

		BuildPropertyValueWrapper& GetPropertyValue(std::string_view name) const
		{
			IBuildPropertyValue* result = nullptr;
			auto status = _state.TryGetPropertyValue(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryGetPropertyValue Failed");
			if (result == nullptr)
				throw std::runtime_error("TryGetPropertyValue has no value.");

			return BuildPropertyValueWrapper(*result);
		}

		std::string_view GetPropertyStringValue(std::string_view name) const
		{
			auto property = GetPropertyValue(name);
			return property.GetStringValue();
		}

		int64_t GetPropertyIntegerValue(std::string_view name) const
		{
			auto property = GetPropertyValue(name);
			return property.GetIntegerValue();
		}

		double GetPropertyDoubleValue(std::string_view name) const
		{
			auto property = GetPropertyValue(name);
			return property.GetDoubleValue();
		}

		bool GetPropertyBooleanValue(std::string_view name) const
		{
			auto property = GetPropertyValue(name);
			return property.GetBooleanValue();
		}

		BuildPropertyValueWrapper& CreatePropertyValue(std::string_view name)
		{
			IBuildPropertyValue* result = nullptr;
			auto status = _state.TryCreatePropertyValue(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryCreatePropertyValue Failed");
			if (result == nullptr)
				throw std::runtime_error("TryCreatePropertyValue has no value.");

			return BuildPropertyValueWrapper(*result);
		}

		void SetPropertyStringValue(std::string_view name, std::string_view value)
		{
			if (HasPropertyValue(name))
			{
				auto property = GetPropertyValue(name);
				property.SetStringValue(value);
			}
			else
			{
				auto property = CreatePropertyValue(name);
				property.SetStringValue(value);
			}
		}

		void SetPropertyIntegerValue(std::string_view name, int64_t value)
		{
			if (HasPropertyValue(name))
			{
				auto property = GetPropertyValue(name);
				property.SetIntegerValue(value);
			}
			else
			{
				auto property = CreatePropertyValue(name);
				property.SetIntegerValue(value);
			}
		}

		void SetPropertyDoubleValue(std::string_view name, double value)
		{
			if (HasPropertyValue(name))
			{
				auto property = GetPropertyValue(name);
				property.SetDoubleValue(value);
			}
			else
			{
				auto property = CreatePropertyValue(name);
				property.SetDoubleValue(value);
			}
		}

		void SetPropertyBooleanValue(std::string_view name, bool value)
		{
			if (HasPropertyValue(name))
			{
				auto property = GetPropertyValue(name);
				property.SetBooleanValue(value);
			}
			else
			{
				auto property = CreatePropertyValue(name);
				property.SetBooleanValue(value);
			}
		}

		bool HasPropertyStringList(std::string_view name) const
		{
			bool result = false;
			auto status = _state.TryHasPropertyStringList(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryHasPropertyStringList Failed");

			return result;
		}

		BuildPropertyListWrapper<const char*>& GetPropertyStringList(std::string_view name)
		{
			IBuildPropertyList<const char*>* result = nullptr;
			auto status = _state.TryGetPropertyStringList(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryGetPropertyStringList Failed");
			if (result == nullptr)
				throw std::runtime_error("TryGetPropertyStringList has no value.");

			return BuildPropertyListWrapper(*result);
		}

		BuildPropertyListWrapper<const char*>& CreatePropertyStringList(std::string_view name)
		{
			IBuildPropertyList<const char*>* result = nullptr;
			auto status = _state.TryCreatePropertyStringList(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryCreatePropertyStringList Failed");
			if (result == nullptr)
				throw std::runtime_error("TryCreatePropertyStringList has no value.");

			return BuildPropertyListWrapper(*result);
		}

		/// <summary>
		/// Extended helpers for easy updating of entire contents
		/// </summary>
		void SetPropertyStringList(std::string_view name, const std::vector<std::string>& values)
		{
			auto property = CreatePropertyStringList(name);
			property.SetAll(values);
		}

		void SetPropertyStringList(std::string_view name, const std::vector<Path>& values)
		{
			auto property = CreatePropertyStringList(name);
			property.SetAll(values);
		}

		std::vector<std::string> CopyPropertyStringListAsStringVector(std::string_view name)
		{
			auto property = GetPropertyStringList(name);
			return property.CopyAsStringVector();
		}

		std::vector<Path> CopyPropertyStringListAsPathVector(std::string_view name)
		{
			auto property = GetPropertyStringList(name);
			return property.CopyAsPathVector();
		}

	private:
		IBuildState& _state;
	};
}
