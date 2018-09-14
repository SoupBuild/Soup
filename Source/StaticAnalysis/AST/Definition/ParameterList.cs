

using System.Collections.Generic;
using System.Linq;

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The function definition
    /// </summary>
    public sealed class ParameterList : Node
    {
        /// <summary>
        /// Gets or sets the list of parameters
        /// </summary>
        public IList<Node> Parameters { get; set; }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as ParameterList;
            if (other == null)
            {
                return false;
            }

            return Parameters.SequenceEqual(other.Parameters);
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Parameters.GetHashCode();
        }
    }
}
