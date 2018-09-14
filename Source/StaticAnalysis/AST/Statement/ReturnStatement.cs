

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Return statement node
    /// </summary>
    public sealed class ReturnStatement : Statement
    {
        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(ReturnStatement lhs, ReturnStatement rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(ReturnStatement lhs, ReturnStatement rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as ReturnStatement;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return 0;
        }
    }
}
