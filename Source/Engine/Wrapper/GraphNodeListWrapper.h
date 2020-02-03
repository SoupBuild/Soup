// <copyright file="GraphNodeListWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	class GraphNodeWrapper;

	/// <summary>
	/// Build State property list implementation, string specialization
	/// </summary>
	class GraphNodeListWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the GraphNodeListWrapper class
		/// </summary>
		GraphNodeListWrapper(IList<IGraphNode*>& value);

		/// <summary>
		/// Size access methods
		/// </summary>
		uint64_t GetSize() const noexcept;

		void Resize(uint64_t size);

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		GraphNodeWrapper GetValueAt(uint64_t index) const;

		/// <summary>
		/// Property setter methods.
		/// </summary>
		void SetValueAt(uint64_t index, GraphNodeWrapper& value);

		/// <summary>
		/// Extended helpers for easy updating of entire contents
		/// </summary>
		void Append(GraphNodeWrapper& value);
		void Append(const GraphNodeListWrapper& values);
		void Append(std::vector<GraphNodeWrapper>& values);

	private:
		IList<IGraphNode*>& _value;
	};
}
