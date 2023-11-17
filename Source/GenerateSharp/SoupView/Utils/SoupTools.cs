
using Opal;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json;

namespace Soup.View;

public static class SoupTools
{
	[DllImport("SoupTools", CallingConvention = CallingConvention.Cdecl)]
	private static extern string LoadBuildGraph(string workingDirectory);

	public static PackageProvider LoadBuildGraph(Path workingDirectory)
	{
		var loadResult = LoadBuildGraph(workingDirectory.ToString());

		var byteArray = Encoding.UTF8.GetBytes(loadResult);
		var memoryStream = new System.IO.MemoryStream(byteArray);

		var result = JsonSerializer.Deserialize(memoryStream, LoadBuildGraphResultContext.Default.LoadBuildGraphResult) ??
			throw new InvalidOperationException("Failed to deserialize the result");

		if (!result.IsSuccess)
			throw new InvalidOperationException(result.Message);

		if (result.Graph is null)
			throw new InvalidOperationException("Successful load build graph did not have a graph");

		var packageGraphLookup = new Dictionary<int, PackageGraph>();
		foreach (var graph in result.Graph.PackageGraphs)
		{
			packageGraphLookup.Add(graph.Id, graph);
		}

		var packageLookup = new Dictionary<int, PackageInfo>();
		foreach (var package in result.Graph.Packages)
		{
			packageLookup.Add(package.Id, package);
		}

		var provider = new PackageProvider()
		{
			RootPackageGraphId = result.Graph.RootPackageGraphId,
			PackageGraphLookup = packageGraphLookup,
			PackageLookup = packageLookup,
		};

		return provider;
	}
}