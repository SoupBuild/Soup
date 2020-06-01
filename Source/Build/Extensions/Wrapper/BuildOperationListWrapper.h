// <copyright file="BuildOperationListWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Extensions
{
	export class BuildOperationWrapper;

	/// <summary>
	/// Build State property list implementation, string specialization
	/// </summary>
	export class BuildOperationListWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildOperationListWrapper class
		/// </summary>
		BuildOperationListWrapper(IList<IBuildOperation*>& value);

		/// <summary>
		/// Size access methods
		/// </summary>
		uint64_t GetSize() const noexcept;

		void Resize(uint64_t size);

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		BuildOperationWrapper GetValueAt(uint64_t index) const;

		/// <summary>
		/// Property setter methods.
		/// </summary>
		void SetValueAt(uint64_t index, BuildOperationWrapper& value);

		/// <summary>
		/// Extended helpers for easy updating of entire contents
		/// </summary>
		void Append(BuildOperationWrapper& value);
		void Append(const BuildOperationListWrapper& values);
		void Append(std::vector<BuildOperationWrapper>& values);

	private:
		IList<IBuildOperation*>& _value;
	};
}
