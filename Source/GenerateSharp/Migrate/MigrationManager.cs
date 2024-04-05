// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System.Xml;
using Path = Opal.Path;

namespace Soup.Build.Migrate;

public static class MigrationManager
{
	public static async Task RunAsync(Path target)
	{
		switch (target.FileExtension)
		{
			case ".csproj":
				await MigrateCSProjAsync(target);
				break;
			default:
				throw new HandledException($"Invalid target {target}");
		}
	}

	private static async Task MigrateCSProjAsync(Path target)
	{
		using var readArchiveFile = LifetimeManager.Get<IFileSystem>().OpenRead(target);

		using var reader = XmlReader.Create(readArchiveFile.GetInStream(), new XmlReaderSettings() { Async = true });

		var parser = new CSProjParser();
		parser.Deserialize(reader);

		await Task.CompletedTask;
	}
}