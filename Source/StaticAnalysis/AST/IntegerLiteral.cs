

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// An integer value
    /// </summary>
    public sealed class IntegerLiteral : LiteralNode
    {
        /// <summary>
        /// Gets or sets the value
        /// </summary>
        public string Value { get; set; }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as IntegerLiteral;
            if (other == null)
            {
                return false;
            }

            return Value.Equals(other.Value);
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Value.GetHashCode();
        }
    }
}
