// <copyright company="Soup" file="Recipe.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Collections.Generic;
	using System.Collections.ObjectModel;
	using System.Collections.Specialized;
	using System.ComponentModel;
	using System.Linq;
	using Newtonsoft.Json;
	using Newtonsoft.Json.Converters;

	/// <summary>
	/// The recipe container
	/// </summary>
	[JsonObject]
	public class Recipe
	{
		private static RecipeType DefaultRecipeType = RecipeType.Library;
		private string _name;
		private RecipeType? _type;
		private SemanticVersion _version;
		private LanguageStandard _standard;
		private ObservableCollection<PackageReference> _dependencies;
		private string _public;
		private ObservableCollection<string> _source;

		[JsonConstructor]
		public Recipe(
			string name)
		{
			IsDirty = false;
			_name = name;
			_type = null;
			_version = null;
			_standard = LanguageStandard.Default;

			Dependencies = new List<PackageReference>();
			_public = null;
			Source = new List<string>();
		}

		[JsonIgnore]
		public bool IsDirty { get; private set; }

		[JsonProperty("name")]
		public string Name
		{
			get
			{
				return _name;
			}

			set
			{
				if (value != _name)
				{
					_name = value;
					IsDirty = true;
				}
			}
		}

		[JsonProperty("type")]
		[DefaultValue(RecipeType.Library)]
		[JsonConverter(typeof(StringEnumConverter))]
		public RecipeType Type
		{
			get
			{
				if (_type != null)
				{
					return (RecipeType)_type;
				}
				else
				{
					return DefaultRecipeType;
				}
			}

			set
			{
				if (value != _type)
				{
					_type = value;
					IsDirty = true;
				}
			}
		}

		[JsonProperty("version")]
		[JsonConverter(typeof(SemanticVersionJsonConverter))]
		public SemanticVersion Version
		{
			get
			{
				return _version;
			}

			set
			{
				if (value != _version)
				{
					_version = value;
					IsDirty = true;
				}
			}
		}

		[JsonProperty("standard")]
		[DefaultValue(LanguageStandard.Default)]
		[JsonConverter(typeof(StringEnumConverter))]
		public LanguageStandard Standard
		{
			get
			{
				return _standard;
			}

			set
			{
				if (value != _standard)
				{
					_standard = value;
					IsDirty = true;
				}
			}
		}

		[JsonProperty("dependencies")]
		public IList<PackageReference> Dependencies
		{
			get
			{
				return _dependencies;
			}

			set
			{
				if (_dependencies != null)
				{
					_dependencies.CollectionChanged -= OnCollectionChanged;
					_dependencies = null;
				}

				if (value != null)
				{
					_dependencies = new ObservableCollection<PackageReference>(value);
					_dependencies.CollectionChanged += OnCollectionChanged;
				}
			}
		}

		[JsonProperty("public")]
		public string Public
		{
			get
			{
				return _public;
			}

			set
			{
				if (value != _public)
				{
					_public = value;
					IsDirty = true;
				}
			}
		}

		[JsonProperty("source")]
		public IList<string> Source
		{
			get
			{
				return _source;
			}

			set
			{
				if (_source != null)
				{
					_source.CollectionChanged -= OnCollectionChanged;
					_source = null;
				}

				if (value != null)
				{
					_source = new ObservableCollection<string>(value);
					_source.CollectionChanged += OnCollectionChanged;
				}
			}
		}

		public void WriteContents()
		{
			Log.Verbose($"Recipe");
			Log.Verbose($"\tName         : {Name}");
			Log.Verbose($"\tType         : {Type}");
			Log.Verbose($"\tVersion      : {Version}");
			Log.Verbose($"\tStandard     : {Standard}");
			Log.Verbose($"\tDependencies : [{string.Join(", ", Dependencies.Select((value) => JsonConvert.SerializeObject(value)))}]");
		}

		private void OnCollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
		{
			// Mark the object as dirty
			IsDirty = true;
		}
	}
}
