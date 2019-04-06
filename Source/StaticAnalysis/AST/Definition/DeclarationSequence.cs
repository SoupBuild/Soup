

using System.Collections.Generic;
using System.Linq;

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Declaration sequence
    /// </summary>
    public sealed class DeclarationSequence : Node
    {
        /// <summary>
        /// Gets or sets the list of declarations
        /// </summary>
        public IList<Declaration> Declarations { get; set; }

        /// <summary>
        /// Initialize
        /// </summary>
        public DeclarationSequence() :
            this(new List<Declaration>())
        {
        }

        /// <summary>
        /// Initialize
        /// </summary>
        public DeclarationSequence(IList<Declaration> declarations)
        {
            Declarations = declarations;
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(DeclarationSequence lhs, DeclarationSequence rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(DeclarationSequence lhs, DeclarationSequence rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as DeclarationSequence;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Declarations.SequenceEqual(other.Declarations);
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
