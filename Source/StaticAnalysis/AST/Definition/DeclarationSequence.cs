

using System.Collections.Generic;

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Declaration sequence
    /// </summary>
    public class DeclarationSequence : Node
    {
        /// <summary>
        /// Gets or sets the list of declarations
        /// </summary>
        public IList<Declaration> Declarations { get; set; } = new List<Declaration>();
    }
}
