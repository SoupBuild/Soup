

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
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as TryFunctionBody;
            if (other == null)
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
