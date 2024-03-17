// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Path = Opal.Path;

namespace Soup.Build.Discover;

public class ScriptBuilder
{
	private readonly Path scriptPath;

	public ScriptBuilder(Path scriptPath)
	{
		this.scriptPath = scriptPath;
	}

	public async Task BuildScriptAsync()
	{
		using var file = LifetimeManager.Get<IFileSystem>().OpenWrite(this.scriptPath, true);

		using var writer = new StreamWriter(file.GetOutStream());

		await writer.WriteLineAsync("#!/bin/bash");
		await writer.WriteLineAsync();
		await writer.WriteLineAsync();
		await writer.WriteLineAsync("# Stop on first error");
		await writer.WriteLineAsync("set - e");
		await writer.WriteLineAsync();
	}
}