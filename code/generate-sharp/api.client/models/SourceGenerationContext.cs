// <copyright file="SourceGenerationContext.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client;

[JsonSourceGenerationOptions(
	WriteIndented = false,
	PropertyNamingPolicy = JsonKnownNamingPolicy.CamelCase,
	GenerationMode = JsonSourceGenerationMode.Metadata)]
[JsonSerializable(typeof(GenerateClosureRequestModel))]
[JsonSerializable(typeof(GenerateClosureResultModel))]
[JsonSerializable(typeof(LanguageModel))]
[JsonSerializable(typeof(PackageCreateOrUpdateModel))]
[JsonSerializable(typeof(PackageModel))]
[JsonSerializable(typeof(PackageVersionModel))]
[JsonSerializable(typeof(SearchPackagesModel))]
internal sealed partial class SourceGenerationContext : JsonSerializerContext
{
}