// <copyright file="SoupApiClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client
{
	/// <summary>
	/// A container for the generate closure result.
	/// </summary>
	public partial class GenerateClosureResultModel
	{
		/// <summary>
		/// Gets or sets the result
		/// </summary>
		[JsonPropertyName("result")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public GenerateClosureResult Result { get; set; }

		/// <summary>
		/// Gets or sets the message
		/// </summary>
		[JsonPropertyName("message")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Message { get; set; }

		/// <summary>
		/// Gets or sets the runtime closure.
		/// </summary>
		[JsonPropertyName("runtimeClosure")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public ICollection<PackageLocalOrPublicExactReferenceWithSubGraphsModel> RuntimeClosure { get; set; }

		/// <summary>
		/// Gets or sets the collection of build closures.
		/// </summary>
		[JsonPropertyName("buildClosures")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public IDictionary<string, ICollection<PackageLocalOrPublicExactReferenceModel>> BuildClosures { get; set; }

		/// <summary>
		/// Gets or sets the collection of tool closures.
		/// </summary>
		[JsonPropertyName("toolClosures")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public IDictionary<string, ICollection<PackageLocalOrPublicExactReferenceModel>> ToolClosures { get; set; }

	}

	public enum GenerateClosureResult
	{
		Success,
		Failure,
	}

	/// <summary>
	/// A class representing a local or public reference to an exact package with subgraph closure references.
	/// </summary>
	public partial class PackageLocalOrPublicExactReferenceWithSubGraphsModel
	{
		/// <summary>
		/// Gets or sets the local package id.
		/// </summary>
		[JsonPropertyName("localId")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? LocalId { get; set; }

		/// <summary>
		/// Gets or sets the public package reference.
		/// </summary>

		[JsonPropertyName("public")]

		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public PackagePublicExactReferenceModel Public { get; set; }

		/// <summary>
		/// Gets or sets the build.
		/// </summary>

		[JsonPropertyName("build")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Build { get; set; }

		/// <summary>
		/// Gets or sets the build.
		/// </summary>

		[JsonPropertyName("tool")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Tool { get; set; }

	}

	/// <summary>
	/// A class representing a feed reference to an exact package.
	/// </summary>
	public partial class PackagePublicExactReferenceModel
	{
		/// <summary>
		/// Gets or sets the language.
		/// </summary>

		[JsonPropertyName("language")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Language { get; set; }

		/// <summary>
		/// Gets or sets the name.
		/// </summary>

		[JsonPropertyName("name")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Name { get; set; }

		/// <summary>
		/// Gets or sets the version.
		/// </summary>

		[JsonPropertyName("version")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public SemanticVersionExactModel Version { get; set; } = new SemanticVersionExactModel();

	}

	/// <summary>
	/// A class representing the semantic version fully qualified model
	/// </summary>
	[System.CodeDom.Compiler.GeneratedCode("NJsonSchema", "13.18.2.0 (NJsonSchema v10.8.0.0 (Newtonsoft.Json v13.0.0.0))")]
	public partial class SemanticVersionExactModel
	{
		/// <summary>
		/// Gets or sets the version major.
		/// </summary>

		[JsonPropertyName("major")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Major { get; set; }

		/// <summary>
		/// Gets or sets the version minor.
		/// </summary>

		[JsonPropertyName("minor")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Minor { get; set; }

		/// <summary>
		/// Gets or sets the version patch.
		/// </summary>

		[JsonPropertyName("patch")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Patch { get; set; }

	}

	/// <summary>
	/// A class representing a local or public reference to an exact package.
	/// </summary>
	public partial class PackageLocalOrPublicExactReferenceModel
	{
		/// <summary>
		/// Gets or sets the local package id.
		/// </summary>
		[JsonPropertyName("localId")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? LocalId { get; set; }

		/// <summary>
		/// Gets or sets the public package reference.
		/// </summary>
		[JsonPropertyName("public")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public PackagePublicExactReferenceModel Public { get; set; }
	}

	public partial class ProblemDetails
	{
		[JsonPropertyName("type")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Type { get; set; }

		[JsonPropertyName("title")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Title { get; set; }

		[JsonPropertyName("status")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? Status { get; set; }

		[JsonPropertyName("detail")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Detail { get; set; }

		[JsonPropertyName("instance")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Instance { get; set; }

		private IDictionary<string, object> _additionalProperties;

		[JsonExtensionData]
		public IDictionary<string, object> AdditionalProperties
		{
			get { return _additionalProperties ?? (_additionalProperties = new Dictionary<string, object>()); }
			set { _additionalProperties = value; }
		}

	}

	/// <summary>
	/// A container for the generate closure request.
	/// </summary>
	public partial class GenerateClosureRequestModel
	{
		/// <summary>
		/// Gets or sets the roots package.
		/// </summary>
		[JsonPropertyName("rootPackage")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public PackageLocalReferenceModel RootPackage { get; set; } = new PackageLocalReferenceModel();

		/// <summary>
		/// Gets or sets the collection of local packages that will be resolved.
		/// </summary>
		[JsonPropertyName("localPackages")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public ICollection<PackageLocalReferenceModel> LocalPackages { get; set; } = new System.Collections.ObjectModel.Collection<PackageLocalReferenceModel>();

		/// <summary>
		/// Gets or sets the collection of known public packages that will be resolved.
		/// </summary>

		[JsonPropertyName("publicPackages")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public ICollection<PackagePublicReferenceModel> PublicPackages { get; set; } = new System.Collections.ObjectModel.Collection<PackagePublicReferenceModel>();

		/// <summary>
		/// Gets or sets the collection of preferred versions for public packages.
		/// </summary>
		[JsonPropertyName("preferredVersions")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public ICollection<PackagePublicExactReferenceModel> PreferredVersions { get; set; } = new System.Collections.ObjectModel.Collection<PackagePublicExactReferenceModel>();

	}

	/// <summary>
	/// A class representing the package result.
	/// </summary>
	public partial class PackageLocalReferenceModel
	{
		/// <summary>
		/// Gets or sets the unique id.
		/// </summary>
		[JsonPropertyName("id")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Id { get; set; }

		/// <summary>
		/// Gets or sets the language reference.
		/// </summary>
		[JsonPropertyName("language")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public LanguageReferenceModel Language { get; set; } = new LanguageReferenceModel();

		/// <summary>
		/// Gets or sets the list of dependencies.
		/// </summary>
		[JsonPropertyName("dependencies")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public IDictionary<string, ICollection<int>> Dependencies { get; set; }
	}

	/// <summary>
	/// A class representing the language version reference.
	/// </summary>
	public partial class LanguageReferenceModel
	{
		/// <summary>
		/// Gets or sets the name.
		/// </summary>
		[JsonPropertyName("name")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Name { get; set; }

		/// <summary>
		/// Gets or sets the version.
		/// </summary>
		[JsonPropertyName("version")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public SemanticVersionModel Version { get; set; } = new SemanticVersionModel();

	}

	/// <summary>
	/// A class representing the semantic version model
	/// </summary>
	public partial class SemanticVersionModel
	{
		/// <summary>
		/// Gets or sets the version major.
		/// </summary>
		[JsonPropertyName("major")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Major { get; set; }

		/// <summary>
		/// Gets or sets the version minor.
		/// </summary>
		[JsonPropertyName("minor")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? Minor { get; set; }

		/// <summary>
		/// Gets or sets the version patch.
		/// </summary>
		[JsonPropertyName("patch")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? Patch { get; set; }

	}

	/// <summary>
	/// A class representing the package result.
	/// </summary>
	public partial class PackagePublicReferenceModel
	{
		/// <summary>
		/// Gets or sets the unique id.
		/// </summary>
		[JsonPropertyName("id")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Id { get; set; }

		/// <summary>
		/// Gets or sets the language.
		/// </summary>
		[JsonPropertyName("language")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Language { get; set; }

		/// <summary>
		/// Gets or sets the name.
		/// </summary>
		[JsonPropertyName("name")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Name { get; set; }

		/// <summary>
		/// Gets or sets the version.
		/// </summary>
		[JsonPropertyName("version")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public SemanticVersionModel Version { get; set; } = new SemanticVersionModel();

	}

	/// <summary>
	/// A class representing the package result.
	/// </summary>
	public partial class LanguageModel
	{
		/// <summary>
		/// Gets or sets the name.
		/// </summary>
		[JsonPropertyName("name")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Name { get; set; }

		/// <summary>
		/// Gets or sets the description.
		/// </summary>
		[JsonPropertyName("description")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Description { get; set; }
	}

	/// <summary>
	/// A class representing the package result.
	/// </summary>
	public partial class PackageModel
	{
		/// <summary>
		/// Gets or sets the name.
		/// </summary>
		[JsonPropertyName("name")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Name { get; set; }

		/// <summary>
		/// Gets or sets the description.
		/// </summary>
		[JsonPropertyName("description")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Description { get; set; }

		/// <summary>
		/// Gets or sets the latest published version.
		/// </summary>
		[JsonPropertyName("latest")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public SemanticVersion Latest { get; set; }

		/// <summary>
		/// Gets or sets the list of versions.
		/// </summary>
		[JsonPropertyName("versions")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public ICollection<PackageVersionModel> Versions { get; set; }
	}

	/// <summary>
	/// The semantic version class.
	/// </summary>
	public partial class SemanticVersion
	{
		/// <summary>
		/// Gets or sets the version major.
		/// </summary>
		[JsonPropertyName("major")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Major { get; set; }

		/// <summary>
		/// Gets or sets the version minor.
		/// </summary>

		[JsonPropertyName("minor")]

		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? Minor { get; set; }

		/// <summary>
		/// Gets or sets the version patch.
		/// </summary>

		[JsonPropertyName("patch")]

		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? Patch { get; set; }

	}

	/// <summary>
	/// Package version model.
	/// </summary>
	[System.CodeDom.Compiler.GeneratedCode("NJsonSchema", "13.18.2.0 (NJsonSchema v10.8.0.0 (Newtonsoft.Json v13.0.0.0))")]
	public partial class PackageVersionModel
	{
		/// <summary>
		/// Gets or sets the version.
		/// </summary>

		[JsonPropertyName("version")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public SemanticVersion Version { get; set; } = new SemanticVersion();

		/// <summary>
		/// Gets or sets the date published.
		/// </summary>

		[JsonPropertyName("datePublished")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public System.DateTimeOffset DatePublished { get; set; }

		/// <summary>
		/// Gets or sets the total downloads count.
		/// </summary>

		[JsonPropertyName("totalDownloads")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int TotalDownloads { get; set; }

	}

	/// <summary>
	/// A container for the package creation or update request.
	/// </summary>
	[System.CodeDom.Compiler.GeneratedCode("NJsonSchema", "13.18.2.0 (NJsonSchema v10.8.0.0 (Newtonsoft.Json v13.0.0.0))")]
	public partial class PackageCreateOrUpdateModel
	{
		/// <summary>
		/// Gets or sets the description.
		/// </summary>

		[JsonPropertyName("description")]

		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		[StringLength(1024)]
		public string Description { get; set; }

	}

	/// <summary>
	/// A container for the search result.
	/// </summary>
	[System.CodeDom.Compiler.GeneratedCode("NJsonSchema", "13.18.2.0 (NJsonSchema v10.8.0.0 (Newtonsoft.Json v13.0.0.0))")]
	public partial class SearchPackagesModel
	{
		/// <summary>
		/// Gets or sets the total packages that match the query.
		/// </summary>

		[JsonPropertyName("total")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Total { get; set; }

		/// <summary>
		/// Gets or sets the start offset for this set of packages.
		/// </summary>

		[JsonPropertyName("offset")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Offset { get; set; }

		/// <summary>
		/// Gets or sets the list of packages.
		/// </summary>

		[JsonPropertyName("packages")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public ICollection<PackageSummaryModel> Packages { get; set; } = new System.Collections.ObjectModel.Collection<PackageSummaryModel>();

	}

	/// <summary>
	/// A container for the package summary result.
	/// </summary>
	public partial class PackageSummaryModel
	{
		/// <summary>
		/// Gets or sets the name.
		/// </summary>

		[JsonPropertyName("name")]

		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Name { get; set; }

		/// <summary>
		/// Gets or sets the description.
		/// </summary>

		[JsonPropertyName("description")]

		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Description { get; set; }

	}

	public partial class FileParameter
	{
		public FileParameter(System.IO.Stream data)
			: this(data, null, null)
		{
		}

		public FileParameter(System.IO.Stream data, string fileName)
			: this(data, fileName, null)
		{
		}

		public FileParameter(System.IO.Stream data, string fileName, string contentType)
		{
			Data = data;
			FileName = fileName;
			ContentType = contentType;
		}

		public System.IO.Stream Data { get; private set; }

		public string FileName { get; private set; }

		public string ContentType { get; private set; }
	}

	public partial class FileResponse : System.IDisposable
	{
		private System.IDisposable _client;
		private System.IDisposable _response;

		public int StatusCode { get; private set; }

		public IReadOnlyDictionary<string, IEnumerable<string>> Headers { get; private set; }

		public System.IO.Stream Stream { get; private set; }

		public bool IsPartial
		{
			get { return StatusCode == 206; }
		}

		public FileResponse(int statusCode, IReadOnlyDictionary<string, IEnumerable<string>> headers, System.IO.Stream stream, System.IDisposable client, System.IDisposable response)
		{
			StatusCode = statusCode;
			Headers = headers;
			Stream = stream;
			_client = client;
			_response = response;
		}

		public void Dispose()
		{
			Stream.Dispose();
			if (_response != null)
				_response.Dispose();
			if (_client != null)
				_client.Dispose();
		}
	}

}