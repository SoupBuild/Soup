// <copyright company="Soup" file="Recipe.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Collections.Generic;
	using System.Collections.ObjectModel;
	using System.Collections.Specialized;
	using System.Linq;
	using Newtonsoft.Json;

	/// <summary>
	/// The recipe container
	/// </summary>
	[JsonObject]
	public class Recipe
	{
		private bool _isDirty;
		private string _name;
		private SemanticVersion _version;
		private ObservableCollection<PackageReference> _dependencies;
		private ObservableCollection<string> _public;
		private ObservableCollection<string> _source;

		[JsonConstructor]
		public Recipe(
			string name,
			SemanticVersion version)
		{
			_isDirty = false;
			_name = name;
			_version = version;

			Dependencies = new List<PackageReference>();
			Public = new List<string>();
			Source = new List<string>();
		}

		[JsonIgnore]
		public bool IsDirty => _isDirty;

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
					_isDirty = true;
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
					_isDirty = true;
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
		public IList<string> Public
		{
			get
			{
				return _public;
			}

			set
			{
				if (_public != null)
				{
					_public.CollectionChanged -= OnCollectionChanged;
					_public = null;
				}

				if (value != null)
				{
					_public = new ObservableCollection<string>(value);
					_public.CollectionChanged += OnCollectionChanged;
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
			Log.Verbose($"\tVersion      : {Version}");
			Log.Verbose($"\tDependencies : [{string.Join(", ", Dependencies.Select((value) => JsonConvert.SerializeObject(value)))}]");
		}

		private void OnCollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
		{
			// Mark the object as dirty
			_isDirty = true;
		}
	}
}
