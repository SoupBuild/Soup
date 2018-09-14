

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
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as CompoundStatement;
            if (other == null)
            {
                return false;
            }

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
