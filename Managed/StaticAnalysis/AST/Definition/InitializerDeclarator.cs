

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The initializer declarator node
    /// </summary>
    public sealed class InitializerDeclarator : Node
    {
        /// <summary>
        /// Gets or sets the declarator
        /// </summary>
        public Node Declarator { get; set; }

        /// <summary>
        /// Gets or sets the optional initializer
        /// </summary>
        public Node Initializer { get; set; }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(InitializerDeclarator lhs, InitializerDeclarator rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(InitializerDeclarator lhs, InitializerDeclarator rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as InitializerDeclarator;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Declarator == other.Declarator &&
                Initializer == other.Initializer;
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Declarator.GetHashCode() ^
                Initializer.GetHashCode();
        }
    }
}
