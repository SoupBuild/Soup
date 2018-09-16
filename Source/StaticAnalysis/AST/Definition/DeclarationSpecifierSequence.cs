

using System.Collections.Generic;
using System.Linq;

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Declaration specifier sequence
    /// </summary>
    public sealed class DeclarationSpecifierSequence : Node
    {
        /// <summary>
        /// Gets or sets the list of declaration specifiers
        /// </summary>
        public IList<Node> Specifiers { get; set; }

        /// <summary>
        /// Initialize
        /// </summary>
        public DeclarationSpecifierSequence() :
            this(new List<Node>())
        {
        }

        /// <summary>
        /// Initialize
        /// </summary>
        public DeclarationSpecifierSequence(IList<Node> specifiers)
        {
            Specifiers = specifiers;
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(DeclarationSpecifierSequence lhs, DeclarationSpecifierSequence rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(DeclarationSpecifierSequence lhs, DeclarationSpecifierSequence rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as DeclarationSpecifierSequence;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Specifiers.SequenceEqual(other.Specifiers);
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
