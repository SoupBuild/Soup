// <copyright file="BuildHistoryJson.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildHistory.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The build state json serializer
	/// </summary>
	export class BuildHistoryJson
	{
	private:
		static constexpr const char* Property_Operations = "operations";
		static constexpr const char* Property_Command = "command";
		static constexpr const char* Property_Input = "input";
		static constexpr const char* Property_Output = "output";

	public:
		/// <summary>
		/// Load from stream
		/// </summary>
		static BuildHistory Deserialize(std::istream& stream)
		{
			// Read the entire file into a string
			std::string content(
				(std::istreambuf_iterator<char>(stream)),
				std::istreambuf_iterator<char>());

			// Read the contents of the build state file
			std::string error = "";
			auto jsonRoot = json11::Json::parse(content, error);
			if (jsonRoot.is_null())
			{
				auto message = "Failed to parse the build state json: " + error;
				throw std::runtime_error(std::move(message));
			}
			else
			{
				return LoadJsonBuildHistory(jsonRoot);
			}
		}

		/// <summary>
		/// Save the BuildHistory to the root file
		/// </summary>
		static void Serialize(const BuildHistory& state, std::ostream& stream)
		{
			// Serialize the contents of the build state
			json11::Json json = BuildJsonBuildHistory(state);

			stream << json.dump();
		}

	private:
		static BuildHistory LoadJsonBuildHistory(const json11::Json& value)
		{
			auto operations = std::vector<OperationInfo>();

			if (!value[Property_Operations].is_null())
			{
				for (auto& value : value[Property_Operations].array_items())
				{
					auto operationInfo = LoadJsonOperationInfo(value);
					operations.push_back(std::move(operationInfo));
				}
			}
			else
			{
				throw std::runtime_error("Missing Required field: knownFiles.");
			}

			return BuildHistory(
				std::move(operations));
		}

		static OperationInfo LoadJsonOperationInfo(const json11::Json& value)
		{
			std::string command;
			std::vector<Path> input;
			std::vector<Path> output;

			if (!value[Property_Command].is_null())
			{
				command = value[Property_Command].string_value();
			}
			else
			{
				throw std::runtime_error("Missing Required field: file.");
			}

			if (!value[Property_Input].is_null())
			{
				auto values = std::vector<Path>();
				for (auto& value : value[Property_Input].array_items())
				{
					values.push_back(Path(value.string_value()));
				}

				input = std::move(values);
			}
			else
			{
				throw std::runtime_error("Missing Required field: input.");
			}

			if (!value[Property_Output].is_null())
			{
				auto values = std::vector<Path>();
				for (auto& value : value[Property_Output].array_items())
				{
					values.push_back(Path(value.string_value()));
				}

				output = std::move(values);
			}
			else
			{
				throw std::runtime_error("Missing Required field: output.");
			}

			return OperationInfo(
				std::move(command),
				std::move(input),
				std::move(output));
		}

		static json11::Json BuildJsonBuildHistory(const BuildHistory& state)
		{
			json11::Json::object result = {};

			// Add required fields
			json11::Json::array operations;
			for (auto& value : state.GetOperations())
			{
				operations.push_back(BuildJsonOperationInfo(value.second));
			}

			result[Property_Operations] = std::move(operations);

			return result;
		}

		static json11::Json BuildJsonOperationInfo(const OperationInfo& info)
		{
			json11::Json::object result = {};

			// Add required fields
			result[Property_Command] = info.Command;

			json11::Json::array input;
			for (auto& value : info.Input)
			{
				input.push_back(value.ToString());
			}

			result[Property_Input] = std::move(input);

			json11::Json::array output;
			for (auto& value : info.Output)
			{
				output.push_back(value.ToString());
			}

			result[Property_Output] = std::move(output);

			return result;
		}
	};
}
