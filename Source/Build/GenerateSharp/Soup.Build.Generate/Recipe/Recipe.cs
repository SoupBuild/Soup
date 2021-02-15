// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Utilities;
using System.Collections.Generic;

namespace Soup.Build.Generate
{
	/// <summary>
	/// The recipe container
	/// </summary>
	internal class Recipe
	{
		private static string Property_Dependencies => "Dependencies";
		private static string Property_Runtime => "Runtime";
		private static string Property_Build => "Build";
		private static string Property_Test => "Test";
		private static string Property_Language => "Language";
		private static string Property_Name => "Name";
		private static string Property_Version => "Version";

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe()
		{
			SetName(string.Empty);
			SetLanguage(string.Empty);
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe(
			string name,
			string language)
		{
			SetName(name);
			SetLanguage(language);
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe(
			string name,
			string language,
			SemanticVersion? version,
			IList<PackageReference>? runtimeDependencies,
			IList<PackageReference>? buildDependencies,
			IList<PackageReference>? testDependencies)
		{
			SetName(name);
			SetLanguage(language);

			if (version.has_value())
				SetVersion(version.value());

			if (runtimeDependencies.has_value())
				SetRuntimeDependencies(runtimeDependencies.value());

			if (buildDependencies.has_value())
				SetBuildDependencies(buildDependencies.value());

			if (testDependencies.has_value())
				SetTestDependencies(testDependencies.value());
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe(RecipeTable table) :
			_table(std::move(table))
		{
			if (!HasValue(_table, Property_Name))
				throw std::runtime_error("Missing required property Name");
			if (!HasValue(_table, Property_Language))
				throw std::runtime_error("Missing required property Language");
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		public Value NameValue => GetValue(_table, Property_Name);

		public string Name
		{
			get { return NameValue.AsString(); }
			set { EnsureValue(_table, Property_Name).SetValueString(value); }
		}

		/// <summary>
		/// Gets or sets the package language
		/// </summary>
		public Value LanguageValue => GetValue(_table, Property_Language);

		public string Language
		{
			get { return LanguageValue.AsString(); }
			set { EnsureValue(_table, Property_Language).SetValueString(value); }
		}

		/// <summary>
		/// Gets or sets the package version
		/// </summary>
		public bool HasVersion => HasValue(_table, Property_Version);

		public SemanticVersion Version
		{
			get
			{
				if (!HasVersion)
					throw new InvalidOperationException("No version.");

				return SemanticVersion.Parse(
					GetValue(_table, Property_Version).AsString());
			}
			set
            {
				EnsureValue(_table, Property_Version).SetValueString(value.ToString());
			}
		}

		/// <summary>
		/// Gets or sets the list of named dependency packages
		/// </summary>
		public bool HasNamedDependencies(string name)
		{
			return HasDependencies() && HasValue(GetDependencies(), name);
		}

		public IList<PackageReference> GetNamedDependencies(string name)
		{
			if (!HasNamedDependencies(name))
				throw new InvalidOperationException("No named dependencies.");

			var values = GetValue(GetDependencies(), name).AsList();
			var result = new List<PackageReference>();
			foreach (var value in values)
			{
				result.Add(PackageReference.Parse(value.AsString()));
			}

			return result;
		}

		public void SetNamedDependencies(string name, IList<PackageReference> values)
		{
			var stringValues = new ValueList();
			foreach (var value in values)
			{
				stringValues.Add(new Value(value.ToString()));
			}

			EnsureValue(EnsureDependencies(), name).SetValueList(stringValues);
		}

		/// <summary>
		/// Gets or sets the list of runtime dependency packages
		/// </summary>
		public bool HasRuntimeDependencies => HasNamedDependencies(Property_Runtime);

		public IList<PackageReference> RuntimeDependencies
		{
			get { return GetNamedDependencies(Property_Runtime); }
			set { SetNamedDependencies(Property_Runtime, value); }
		}

		/// <summary>
		/// Gets or sets the list of build dependency packages
		/// </summary>
		public bool HasBuildDependencies => HasNamedDependencies(Property_Build);

		public IList<PackageReference> BuildDependencies
		{
			get { return GetNamedDependencies(Property_Build); }
			set { SetNamedDependencies(Property_Build, value); }
		}

		/// <summary>
		/// Gets or sets the list of test dependency packages
		/// </summary>
		public bool HasTestDependencies => HasNamedDependencies(Property_Test);

		public IList<PackageReference> TestDependencies
		{
			get { return GetNamedDependencies(Property_Test); }
			set { SetNamedDependencies(Property_Test, value); }
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public RecipeTable& GetTable()
		{
			return _table;
		}

		/// <summary>
		/// Gets or sets the table of dependency packages
		/// </summary>
		private bool HasDependencies()
		{
			return HasValue(_table, Property_Dependencies);
		}

		private RecipeTable& GetDependencies()
		{
			if (!HasDependencies())
				throw std::runtime_error("No dependencies.");

			auto& values = GetValue(_table, Property_Dependencies).AsTable();
			return values;
		}

		private RecipeTable& EnsureDependencies()
		{
			auto& value = EnsureValue(_table, Property_Dependencies);
			switch (value.GetType())
			{
				case RecipeValueType::Table:
					// All good.
					break;
				case RecipeValueType::Empty:
					value.SetValueTable(RecipeTable());
					break;
				default:
					throw std::runtime_error("The recipe already has a non-table dependencies property");
			}

			return value.AsTable();
		}

		private bool HasValue(RecipeTable& table, std::string_view key)
		{
			return table.contains(key.data());
		}

		private const RecipeValue& GetValue(RecipeTable table, std::string_view key)
		{
			auto findItr = table.find(key.data());
			if (findItr != table.end())
			{
				return findItr->second;
			}
			else
			{
				throw std::runtime_error("Requested recipe value does not exist in the table.");
			}
		}

		private RecipeValue& GetValue(RecipeTable& table, std::string_view key)
		{
			auto findItr = table.find(key.data());
			if (findItr != table.end())
			{
				return findItr->second;
			}
			else
			{
				throw std::runtime_error("Requested recipe value does not exist in the table.");
			}
		}

		private RecipeValue& EnsureValue(RecipeTable& table, std::string_view key)
		{
			auto findItr = table.find(key.data());
			if (findItr != table.end())
			{
				return findItr->second;
			}
			else
			{
				auto insertResult = table.emplace(key.data(), RecipeValue());
				if (insertResult.second)
				{
					return insertResult.first->second;
				}
				else
				{
					throw std::runtime_error("Failed to insert a recipe value.");
				}
			}
		}

		private ValueTable _table;
	}
}
