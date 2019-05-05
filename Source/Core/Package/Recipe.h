// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    /// <summary>
    /// The recipe container
    /// </summary>
    class Recipe
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Recipe"/> class.
        /// </summary>
        Recipe(std::string name) :
            m_isDirty(false),
            m_name(std::move(name)),
            m_type(std::nullopt),
            m_version(std::nullopt),
            m_public(std::nullopt)
        {
            Dependencies = new List<PackageReference>();
            _public = null;
            Source = new List<string>();
        }

        /// <summary>
        /// Gets a value indicating whether the content has changed or not
        /// </summary>
        bool GetIsDirty()
        {
            return m_isDirty;
        }

        /// <summary>
        /// Gets or sets the package name
        /// </summary>
        const std::string& GetName()
        {
            return m_name;
        }

        void SetName(std::string value)
        {
            if (value != m_name)
            {
                m_name = std::move(value);
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the package type
        /// </summary>
        RecipeType GetType()
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

        void SetType(RecipeType value)
        {
            if (value != m_type)
            {
                m_type = value;
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the package version
        /// </summary>
        SemanticVersion GetVersion()
        {
            return m_version;
        }

        void SetVersion(SemanticVersion value)
        {
            if (value != m_version)
            {
                m_version = value;
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the list of dependency packages
        /// TODO: Observable?
        /// </summary>
        const std::vector<PackageReference>& GetDependencies()
        {
            return _dependencies;
        }

        void SetDependencies(const std::vector<PackageReference>& value)
        {
            if (value != m_dependencies)
            {
                m_dependencies = value;
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the public file
        /// </summary>
        const std::string& GetPublic()
        {
            return _public;
        }

        void SetPublic(const std::string& value)
        {
            if (value != m_public)
            {
                m_public = value;
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the source values
        /// </summary>
        IList<string> Source
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

    private:
        static const RecipeType DefaultRecipeType = RecipeType.Library;
        std::string m_name;
        std::optional<RecipeType> m_type;
        private SemanticVersion m_version;
        private LanguageStandard m_standard;
        private ObservableCollection<PackageReference> m_dependencies;
        std::optional<stdstring> m_public;
        private ObservableCollection<string> m_source;
    }
}
