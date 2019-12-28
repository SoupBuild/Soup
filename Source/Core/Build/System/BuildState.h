// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	class BuildState : public IBuildState
	{
	public:
		BuildState() :
			_properties(),
			_graph()
		{
		}

		bool HasProperty(const char* name) override final
		{
			return _properties.contains(name);
		}

		const std::any& GetProperty(const char* name) override final
		{
			return _properties.at(name);
		}

		void SetProperty(const char* name, std::any value) override final
		{
			_properties.insert_or_assign(name, std::move(value));
		}

		void AddBuildNode(std::shared_ptr<BuildGraphNode> node) override final
		{
			_graph.GetNodes().push_back(std::move(node));
		}

		std::vector<std::shared_ptr<BuildGraphNode>>& GetBuildNodes()
		{
			return _graph.GetNodes();
		}

		const std::vector<std::shared_ptr<BuildGraphNode>>& GetBuildNodes() const
		{
			return _graph.GetNodes();
		}

	private:
		std::map<std::string, std::any> _properties;
		BuildGraph _graph;
	};
}
