// <copyright file="RecipeToml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using System;
using System.Threading.Tasks;
using Tomlyn;
using Tomlyn.Model;

namespace Soup.Build.Utilities
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

        /// <summary>
        /// Save the recipe to the root file
        /// </summary>
        public static async Task SerializeAsync(ValueTable recipeTable, System.IO.Stream stream)
        {
            // Serialize the contents of the recipe
            var root = Build(recipeTable);
            var content = root.ToString();

            // Write out the entire root table
            using (var writer = new System.IO.StreamWriter(stream))
            {
                await writer.WriteAsync(content);
            }
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

        static TomlObject? Build(Value value)
        {
            TomlObject? result = null;
            switch (value.Type)
            {
                case ValueType.Empty:
                    // Leave empty
                    break;
                case ValueType.Table:
                    result = Build(value.AsTable());
                    break;
                case ValueType.List:
                    result = Build(value.AsList());
                    break;
                case ValueType.String:
                    result = new TomlString(value.AsString());
                    break;
                case ValueType.Integer:
                    result = new TomlInteger(value.AsInteger());
                    break;
                case ValueType.Float:
                    result = new TomlFloat(value.AsFloat());
                    break;
                case ValueType.Boolean:
                    result = new TomlBoolean(value.AsBoolean());
                    break;
                default:
                    throw new InvalidOperationException("Unknown value type.");
            }

            /// TODO: result.comments() = value.();

            return result;
        }

        static TomlTable Build(IValueTable table)
        {
            var result = new TomlTable();
            foreach (var value in table)
            {
                result.Add(value.Key, Build(value.Value));
            }

            return result;
        }

        static TomlArray Build(IValueList list)
        {
            var result = new TomlArray();
            foreach (var value in list)
            {
                result.Add(Build(value));
            }

            return result;
        }
    }
}
