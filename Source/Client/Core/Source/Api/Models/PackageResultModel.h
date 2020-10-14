// <copyright file="PackageResultModel.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Api
{
	/// <summary>
	/// A class representing the package result
	/// </summary>
	export class PackageResultModel
	{
	public:
		PackageResultModel() :
			_name(),
			_description(),
			_latest()
		{
		}

		PackageResultModel(
			std::string name) :
			_name(std::move(name)),
			_description(),
			_latest()
		{
		}

		PackageResultModel(
			std::string name,
			std::optional<SemanticVersion> latest) :
			_name(std::move(name)),
			_description(),
			_latest(latest)
		{
		}

		/// <summary>
		/// Gets the name
		/// </summary>
		const std::string& GetName() const
		{
			return _name;
		}

		/// <summary>
		/// Gets the description
		/// </summary>
		const std::string& Description() const
		{
			return _description;
		}

		/// <summary>
		/// Gets a value indicating whether there is a latest version
		/// </summary>
		bool HasLatest() const
		{
			return _latest.has_value();
		}

		/// <summary>
		/// Gets the latest published version
		/// </summary>
		SemanticVersion GetLatest() const
		{
			if (!HasLatest())
				throw std::runtime_error("No latest.");
			return _latest.value();
		}

		/// <summary>
		/// Gets the list of publications
		/// </summary>
		// const std::vector<PublicationSummaryModel> Publications { get; set; }

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const PackageResultModel& rhs) const
		{
			return _name == rhs._name &&
				_description == rhs._description &&
				_latest == rhs._latest;
		}

		bool operator !=(const PackageResultModel& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		std::string _name;
		std::string _description;
		std::optional<SemanticVersion> _latest;
	};
}
