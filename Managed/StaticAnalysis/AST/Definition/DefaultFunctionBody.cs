

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The default function body
    /// </summary>
    public sealed class DefaultFunctionBody : Node
    {
        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(DefaultFunctionBody lhs, DefaultFunctionBody rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(DefaultFunctionBody lhs, DefaultFunctionBody rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as DefaultFunctionBody;
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
