// <copyright file="RecipeToml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using System.Threading.Tasks;
using Tomlyn;
using Tomlyn.Syntax;

namespace Soup.Build.Utilities
{
    /// <summary>
    /// The recipe Toml serialize manager
    /// </summary>
    public static class ValueTableTomlUtilities
    {
        /// <summary>
        /// Load from stream
        /// </summary>
        public static ValueTable Deserialize(
            Path tomlFile,
            string content)
        {
            // Read the contents of the recipe file
            var documentSyntax = Toml.Parse(content, tomlFile.ToString());

            // Load the entire root table
            var valueTable = new ValueTable()
            {
                MirrorSyntax = documentSyntax,
            };
            var visitor = new ValueTableTomlVisitor(valueTable);
            documentSyntax.Accept(visitor);

            return valueTable;
        }

        /// <summary>
        /// Save the document syntax to the root file
        /// </summary>
        public static async Task SerializeAsync(DocumentSyntax documentSyntax, System.IO.Stream stream)
        {
            // Write out the entire root table
            var content = documentSyntax.ToString();
            using (var writer = new System.IO.StreamWriter(stream))
            {
                await writer.WriteAsync(content);
            }
        }
    }
}
