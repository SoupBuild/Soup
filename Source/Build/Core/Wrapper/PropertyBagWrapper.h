// <copyright file="PropertyBagWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Definition/IPropertyBag.h"
#include "PropertyValueWrapper.h"
#include "StringListWrapper.h"

namespace Soup::Build
{
	/// <summary>
	/// Build Property Bag Extension used to make the IPropertyBag interface more usable
	/// </summary>
	export class PropertyBagWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the PropertyBagWrapper class
		/// </summary>
		PropertyBagWrapper(IPropertyBag& value) :
			_value(value)
		{
		}

		/// <summary>
		/// Property access methods
		/// </summary>
		bool HasPropertyValue(std::string_view name) const
		{
			bool result = false;
			auto status = _value.TryHasPropertyValue(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryHasPropertyValue Failed");

			return result;
		}

		PropertyValueWrapper& GetPropertyValue(std::string_view name) const
		{
			IPropertyValue* result = nullptr;
			auto status = _value.TryGetPropertyValue(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryGetPropertyValue Failed");
			if (result == nullptr)
				throw std::runtime_error("TryGetPropertyValue has no value.");

			return PropertyValueWrapper(*result);
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

		PropertyValueWrapper& CreatePropertyValue(std::string_view name)
		{
			IPropertyValue* result = nullptr;
			auto status = _value.TryCreatePropertyValue(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryCreatePropertyValue Failed");
			if (result == nullptr)
				throw std::runtime_error("TryCreatePropertyValue has no value.");

			return PropertyValueWrapper(*result);
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
			auto status = _value.TryHasPropertyStringList(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryHasPropertyStringList Failed");

			return result;
		}

		StringListWrapper& GetPropertyStringList(std::string_view name)
		{
			IList<const char*>* result = nullptr;
			auto status = _value.TryGetPropertyStringList(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryGetPropertyStringList Failed");
			if (result == nullptr)
				throw std::runtime_error("TryGetPropertyStringList has no value.");

			return StringListWrapper(*result);
		}

		StringListWrapper& CreatePropertyStringList(std::string_view name)
		{
			IList<const char*>* result = nullptr;
			auto status = _value.TryCreatePropertyStringList(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryCreatePropertyStringList Failed");
			if (result == nullptr)
				throw std::runtime_error("TryCreatePropertyStringList has no value.");

			return StringListWrapper(*result);
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

		void AppendPropertyStringList(std::string_view name, const std::vector<std::string>& values)
		{
			if (HasPropertyStringList(name))
			{
				auto property = GetPropertyStringList(name);
				return property.Append(values);
			}
			else
			{
				auto property = CreatePropertyStringList(name);
				property.SetAll(values);
			}
		}

		std::vector<std::string> CopyPropertyStringListAsStringVector(std::string_view name)
		{
			if (HasPropertyStringList(name))
			{
				auto property = GetPropertyStringList(name);
				return property.CopyAsStringVector();
			}
			else
			{
				return std::vector<std::string>();
			}
		}

		std::vector<Path> CopyPropertyStringListAsPathVector(std::string_view name)
		{
			auto property = GetPropertyStringList(name);
			return property.CopyAsPathVector();
		}

	private:
		IPropertyBag& _value;
	};
}
