

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
        /// Equality operator
        /// </summary>
        public static bool operator ==(ParameterList lhs, ParameterList rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(ParameterList lhs, ParameterList rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as ParameterList;
            if (object.ReferenceEquals(other, null))
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
