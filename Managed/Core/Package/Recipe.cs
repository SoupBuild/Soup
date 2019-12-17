// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
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
        private static readonly RecipeType DefaultRecipeType = RecipeType.Library;
        private string _name;
        private RecipeType? _type;
        private SemanticVersion _version;
        private LanguageStandard _standard;
        private ObservableCollection<PackageReference> _dependencies;
        private string _public;
        private ObservableCollection<string> _source;
        private ObservableCollection<string> _includePaths;

        /// <summary>
        /// Initializes a new instance of the <see cref="Recipe"/> class.
        /// </summary>
        [JsonConstructor]
        public Recipe(string name)
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

        /// <summary>
        /// Gets a value indicating whether the content has changed or not
        /// </summary>
        [JsonIgnore]
        public bool IsDirty { get; private set; }

        /// <summary>
        /// Gets or sets the package name
        /// </summary>
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

        /// <summary>
        /// Gets or sets the package type
        /// </summary>
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

        /// <summary>
        /// Gets or sets the package version
        /// </summary>
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

        /// <summary>
        /// Gets or sets the language standard
        /// </summary>
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

        /// <summary>
        /// Gets or sets the list of dependency packages
        /// </summary>
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

        /// <summary>
        /// Gets or sets the public file
        /// </summary>
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

        /// <summary>
        /// Gets or sets the source values
        /// </summary>
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

        /// <summary>
        /// Gets or sets the source values
        /// </summary>
        [JsonProperty("includePaths")]
        public IList<string> IncludePaths
        {
            get
            {
                return _includePaths;
            }

            set
            {
                if (_includePaths != null)
                {
                    _includePaths.CollectionChanged -= OnCollectionChanged;
                    _includePaths = null;
                }

                if (value != null)
                {
                    _includePaths = new ObservableCollection<string>(value);
                    _includePaths.CollectionChanged += OnCollectionChanged;
                }
            }
        }

        /// <summary>
        /// Write the contents
        /// </summary>
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
