

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
        public int Value { get; set; }

        /// <summary>
        /// Initialize
        /// </summary>
        public IntegerLiteral(int value)
        {
            Value = value;
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(IntegerLiteral lhs, IntegerLiteral rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(IntegerLiteral lhs, IntegerLiteral rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as IntegerLiteral;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Value == other.Value;
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
