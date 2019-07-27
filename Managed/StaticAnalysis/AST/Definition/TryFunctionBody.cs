

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The try function body
    /// </summary>
    public sealed class TryFunctionBody : Node
    {
        /// <summary>
        /// Gets or sets the statements
        /// </summary>
        public CompoundStatement Statements { get; set; }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(TryFunctionBody lhs, TryFunctionBody rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(TryFunctionBody lhs, TryFunctionBody rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as TryFunctionBody;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Statements.Equals(other.Statements);
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Statements.GetHashCode();
        }
    }
}
