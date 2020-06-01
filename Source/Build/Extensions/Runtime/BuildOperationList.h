// <copyright file="BuildOperationList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Extensions
{
	export class BuildOperation;

	/// <summary>
	/// Build list implementation for simple objects
	/// </summary>
	export class BuildOperationList : public IList<IBuildOperation*>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildOperationList class
		/// </summary>
		BuildOperationList();
		BuildOperationList(std::vector<Memory::Reference<BuildOperation>> values);

		/// <summary>
		/// Size access methods
		/// </summary>
		uint64_t GetSize() const noexcept override final;

		ApiCallResult TryResize(uint64_t size) noexcept override final;

		/// <summary>
		/// Type specific accessor methods
		/// TODO: This being const does not really make sense, the operation can edit its children
		/// </summary>
		ApiCallResult TryGetValueAt(uint64_t index, IBuildOperation*& result) const noexcept override final;

		/// <summary>
		/// Property setter methods.
		/// </summary>
		ApiCallResult TrySetValueAt(uint64_t index, IBuildOperation* value) noexcept override final;

		void SetValueAt(uint64_t index, BuildOperation* value);

		/// <summary>
		/// Internal access
		/// </summary>
		std::vector<Memory::Reference<BuildOperation>>& GetValues();
		const std::vector<Memory::Reference<BuildOperation>>& GetValues() const;

#ifdef DEBUG
		void CheckCycles(const std::set<int64_t>& rootOperations, const BuildOperation& operation);
#endif

	private:
		std::vector<Memory::Reference<BuildOperation>> _values;
	};
}
