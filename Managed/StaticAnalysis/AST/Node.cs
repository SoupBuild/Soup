

using System;

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Base node
    /// </summary>
    public abstract class Node
    {
        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(Node lhs, Node rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(Node lhs, Node rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            throw new NotImplementedException();
        }
    }
}
