

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The delete function body
    /// </summary>
    public sealed class DeleteFunctionBody : Node
    {
        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(DeleteFunctionBody lhs, DeleteFunctionBody rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(DeleteFunctionBody lhs, DeleteFunctionBody rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as DeleteFunctionBody;
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
