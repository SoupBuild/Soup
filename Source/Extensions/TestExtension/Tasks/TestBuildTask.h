// <copyright file="TestBuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

export class TestBuildTask : public Opal::Memory::ReferenceCounted<Soup::Build::IBuildTask>
{
public:
	const char* GetName() const noexcept override final
	{
		return "TestBuild";
	}

	Soup::Build::OperationResult Execute(Soup::Build::IBuildState& state) noexcept override final
	{
		return 0;
	}
};
