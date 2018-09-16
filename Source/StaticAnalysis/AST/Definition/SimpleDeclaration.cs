

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The simple declaration
    /// </summary>
    public sealed class SimpleDefinition : Declaration
    {
        /// <summary>
        /// Gets or sets the specifiers
        /// </summary>
        public DeclarationSpecifierSequence Specifiers { get; set; }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(SimpleDefinition lhs, SimpleDefinition rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(SimpleDefinition lhs, SimpleDefinition rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as SimpleDefinition;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Specifiers == other.Specifiers;
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Specifiers.GetHashCode();
        }
    }
}
