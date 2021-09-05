// <copyright file="RecipeToml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using System;
using System.Threading.Tasks;
using Tomlyn;
using Tomlyn.Model;
using Tomlyn.Syntax;

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
        public static Recipe Deserialize(
            Path recipeFile,
            string content)
        {
            // Read the contents of the recipe file
            var syntax = Toml.Parse(content, recipeFile.ToString());

            // Load the entire root table
            var valueTable = new ValueTable();
            var visitor = new RecipeTomlVisitor(valueTable);
            syntax.Accept(visitor);

            return new Recipe(valueTable, syntax);
        }

        /// <summary>
        /// Save the recipe to the root file
        /// </summary>
        public static async Task SerializeAsync(Recipe recipe, System.IO.Stream stream)
        {
            // Serialize the contents of the recipe
            var documentSyntax = recipe.MirrorSyntax;
            if (documentSyntax == null)
                throw new ArgumentException("The provided recipe does not have a mirrored syntax tree.", nameof(recipe));

            // Write out the entire root table
            var content = documentSyntax.ToString();
            using (var writer = new System.IO.StreamWriter(stream))
            {
                await writer.WriteAsync(content);
            }
        }
    }
}
