

using System.Collections.Generic;
using System.Linq;

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The initializer declarator list node
    /// </summary>
    public sealed class InitializerDeclaratorList : Node
    {
        /// <summary>
        /// Gets or sets the list of items
        /// </summary>
        public IList<InitializerDeclarator> Items { get; set; }

        /// <summary>
        /// Initialize
        /// </summary>
        public InitializerDeclaratorList() :
            this(new List<InitializerDeclarator>())
        {
        }

        /// <summary>
        /// Initialize
        /// </summary>
        public InitializerDeclaratorList(IList<InitializerDeclarator> items)
        {
            Items = items;
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(InitializerDeclaratorList lhs, InitializerDeclaratorList rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(InitializerDeclaratorList lhs, InitializerDeclaratorList rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as InitializerDeclaratorList;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Items.SequenceEqual(other.Items);
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Items.GetHashCode();
        }
    }
}
