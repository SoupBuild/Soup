// <copyright file="LoadBuildGraphResultContext.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Text.Json.Serialization;

namespace Soup.View;

[JsonSourceGenerationOptions(
	WriteIndented = false,
	PropertyNamingPolicy = JsonKnownNamingPolicy.Unspecified,
	GenerationMode = JsonSourceGenerationMode.Metadata)]
[JsonSerializable(typeof(LoadBuildGraphResult))]
internal partial class LoadBuildGraphResultContext : JsonSerializerContext
{
}
