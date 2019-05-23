

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// An integer value
    /// </summary>
    public sealed class QualifiedName : Node
    {
        /// <summary>
        /// Gets or sets the Qualifier
        /// </summary>
        public LiteralNode Qualifier { get; set; }

        /// <summary>
        /// Gets or sets the Qualifier
        /// </summary>
        public Node Name { get; set; }

        /// <summary>
        /// Initialize
        /// </summary>
        public QualifiedName()
        {
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(QualifiedName lhs, QualifiedName rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(QualifiedName lhs, QualifiedName rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as QualifiedName;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Qualifier == other.Qualifier &&
                Name == other.Name;
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Qualifier.GetHashCode() ^
                Name.GetHashCode();
        }
    }
}
