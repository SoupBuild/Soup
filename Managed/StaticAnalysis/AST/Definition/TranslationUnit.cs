

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The root translation unit node
    /// </summary>
    public sealed class TranslationUnit : Node
    {
        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(TranslationUnit lhs, TranslationUnit rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(TranslationUnit lhs, TranslationUnit rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Gets or sets the option declaration sequence
        /// </summary>
        public DeclarationSequence Declarations { get; set; }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as TranslationUnit;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Declarations == other.Declarations;
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Declarations.GetHashCode();
        }
    }
}
