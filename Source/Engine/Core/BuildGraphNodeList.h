// <copyright file="BuildGraphNodeList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	class BuildGraphNode;

	/// <summary>
	/// Build list implementation for simple objects
	/// </summary>
	class BuildGraphNodeList : public IList<IGraphNode*>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildGraphNodeList class
		/// </summary>
		BuildGraphNodeList();
		BuildGraphNodeList(std::vector<Memory::Reference<BuildGraphNode>> values);

		/// <summary>
		/// Size access methods
		/// </summary>
		uint64_t GetSize() const noexcept override final;

		OperationResult Resize(uint64_t size) noexcept override final;

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		OperationResult TryGetValueAt(uint64_t index, IGraphNode*& result) noexcept override final;

		/// <summary>
		/// Property setter methods.
		/// </summary>
		OperationResult TrySetValueAt(uint64_t index, IGraphNode* value) noexcept override final;

		void SetValueAt(uint64_t index, BuildGraphNode* value);

		/// <summary>
		/// Internal access
		/// </summary>
		const std::vector<Memory::Reference<BuildGraphNode>>& GetValues() const;

#ifdef DEBUG
		void CheckCycles(const std::set<int64_t>& rootNodes, const BuildGraphNode& node);
#endif

	private:
		std::vector<Memory::Reference<BuildGraphNode>> _values;
	};
}
