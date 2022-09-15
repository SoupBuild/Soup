// <copyright file="SML.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	export class SMLTable;
	export class SMLList;
	export class SMLValue;

	export enum class SMLValueType
	{
		Empty,
		Boolean,
		Integer,
		Float,
		String,
		Table,
		Array,
	};

	class SMLTable
	{
	public:
		SMLTable() :
			_value()
		{
		}

		SMLTable(std::unordered_map<std::string, SMLValue> value) :
			_value(std::move(value))
		{
		}

		const SMLValue& operator[](const std::string& key) const
		{
			return _value.at(key);
		}

		const std::unordered_map<std::string, SMLValue>& GetValue() const
		{
			return _value;
		}

	private:
		std::unordered_map<std::string, SMLValue> _value;
	};

	class SMLArray
	{
	public:
		SMLArray() :
			_value()
		{
		}

		SMLArray(std::vector<SMLValue> value) :
			_value(std::move(value))
		{
		}

		const SMLValue& operator[](size_t key) const
		{
			return _value.at(key);
		}

		const size_t GetSize() const
		{
			return _value.size();
		}

		const std::vector<SMLValue>& GetValue() const
		{
			return _value;
		}

	private:
		std::vector<SMLValue> _value;
	};

	/// <summary>
	/// The SML Document
	/// </summary>
	export class SMLDocument
	{
	public:
		/// <summary>
		/// Load from stream
		/// </summary>
		static SMLDocument Parse(std::istream& stream);

	public:
		SMLDocument(SMLTable root) :
			_root(std::move(root))
		{
		}

		const SMLTable& GetRoot() const
		{
			return _root;
		}

	private:
		SMLTable _root;
	};

	class SMLValue
	{
	public:
		SMLValue() :
			_type(SMLValueType::Empty),
			_value()
		{
		}

		SMLValue(int64_t value) :
			_type(SMLValueType::Integer),
			_value(value)
		{
		}

		SMLValue(std::string value) :
			_type(SMLValueType::String),
			_value(std::move(value))
		{
		}

		SMLValue(SMLArray value) :
			_type(SMLValueType::Array),
			_value(std::move(value))
		{
		}

		SMLValue(SMLTable value) :
			_type(SMLValueType::Table),
			_value(std::move(value))
		{
		}

		SMLValueType GetType() const
		{
			return _type;
		}

		const SMLArray& AsArray() const
		{
			if (_type != SMLValueType::Array)
				throw std::runtime_error("Incorrect access type: Value is not Array");
			else
				return std::any_cast<const SMLArray&>(_value);
		}

		const SMLTable& AsTable() const
		{
			if (_type != SMLValueType::Table)
				throw std::runtime_error("Incorrect access type: Value is not Table");
			else
				return std::any_cast<const SMLTable&>(_value);
		}

		const std::string& AsString() const
		{
			if (_type != SMLValueType::String)
				throw std::runtime_error("Incorrect access type: Value is not String");
			else
				return std::any_cast<const std::string&>(_value);
		}

		int64_t AsInteger() const
		{
			if (_type != SMLValueType::Integer)
				throw std::runtime_error("Incorrect access type: Value is not Integer");
			else
				return std::any_cast<int64_t>(_value);
		}

		bool AsBoolean() const
		{
			if (_type != SMLValueType::Boolean)
				throw std::runtime_error("Incorrect access type: Value is not Boolean");
			else
				return std::any_cast<bool>(_value);
		}

		double AsFloat() const
		{
			if (_type != SMLValueType::Float)
				throw std::runtime_error("Incorrect access type: Value is not Float");
			else
				return std::any_cast<double>(_value);
		}

	private:
		SMLValueType _type;
		std::any _value;
	};

	std::ostream& operator<<(std::ostream& stream, const SMLValue& value);

	std::ostream& operator<<(std::ostream& stream, const SMLDocument& value)
	{
		for (const auto& tableValue : value.GetRoot().GetValue())
		{
			stream << tableValue.first << ": " << tableValue.second << "\n";
		}

		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const SMLTable& value)
	{
		stream << '{';
		for (const auto& tableValue : value.GetValue())
		{
			stream << tableValue.first << ": " << tableValue.second << "\n";
		}

		stream << '}';

		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const SMLArray& value)
	{
		stream << '[';
		for (const auto& arrayValue : value.GetValue())
		{
			stream << arrayValue << '\n';
		}

		stream << ']';

		return stream;
	}

	std::ostream& operator<<(std::ostream& stream, const SMLValue& value)
	{
		switch (value.GetType())
		{
			case SMLValueType::Empty:
				break;
			case SMLValueType::Boolean:
				stream << value.AsBoolean();
				break;
			case SMLValueType::Integer:
				stream << value.AsInteger();
				break;
			case SMLValueType::Float:
				stream << value.AsFloat();
				break;
			case SMLValueType::String:
				// TODO: Escape characters
				stream << "\"" << value.AsString() << "\"";
				break;
			case SMLValueType::Array:
				stream <<  value.AsArray();
				break;
			case SMLValueType::Table:
				stream <<  value.AsTable();
				break;
			default:
				throw std::runtime_error("Unknown SML type.");
		}

		return stream;
	}
}
