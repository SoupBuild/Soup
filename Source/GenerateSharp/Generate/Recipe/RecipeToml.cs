// <copyright file="RecipeToml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System;
using Tomlyn;
using Tomlyn.Model;

namespace Soup.Build.Generate
{
    /// <summary>
    /// The recipe Toml serialize manager
    /// </summary>
    internal static class RecipeToml
	{
		/// <summary>
		/// Load from stream
		/// </summary>
		public static ValueTable Deserialize(
			Path recipeFile,
			string content)
		{
			// Read the contents of the recipe file
			var root = Toml.Parse(content, recipeFile.ToString());

			// Load the entire root table
			return Parse(root.ToModel());
		}

		private static Value Parse(TomlObject source)
		{
			switch (source.Kind)
			{
				case ObjectKind.Boolean:
					return new Value(((TomlBoolean)source).Value);
				case ObjectKind.Integer:
					return new Value(((TomlInteger)source).Value);
				case ObjectKind.Float:
					return new Value(((TomlFloat)source).Value);
				case ObjectKind.String:
					return new Value(((TomlString)source).Value);
				case ObjectKind.OffsetDateTime:
				case ObjectKind.LocalDateTime:
				case ObjectKind.LocalDate:
				case ObjectKind.LocalTime:
					throw new InvalidOperationException("TODO: What to do with datetime?");
				case ObjectKind.Array:
					return new Value(Parse((TomlArray)source));
				case ObjectKind.Table:
					return new Value(Parse((TomlTable)source));
				case ObjectKind.TableArray:
					return Parse((TomlTableArray)source);
				default:
				{
					throw new InvalidOperationException("Unknown toml type.");
				}
			}
		}

		static ValueTable Parse(TomlTable source)
		{
			var result = new ValueTable();
			foreach (var item in source.GetTomlEnumerator())
			{
				var value = Parse(item.Value);
				result.Add(item.Key, value);
			}

			return result;
		}

		static ValueList Parse(TomlArray source)
		{
			var result = new ValueList();
			foreach (var item in source.GetTomlEnumerator())
			{
				result.Add(Parse(item));
			}

			return result;
		}
	}
}
