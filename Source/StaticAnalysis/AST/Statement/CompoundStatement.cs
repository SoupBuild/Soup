

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Compound statement node
    /// </summary>
    public sealed class CompoundStatement : Node
    {
        /// <summary>
        /// Gets or sets the list of statements
        /// </summary>
        public IList<Statement> Statements { get; set; }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(CompoundStatement lhs, CompoundStatement rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(CompoundStatement lhs, CompoundStatement rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as CompoundStatement;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            if (Statements == null)
                return other.Statements == null;
            else
                return Statements.SequenceEqual(other.Statements);
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
