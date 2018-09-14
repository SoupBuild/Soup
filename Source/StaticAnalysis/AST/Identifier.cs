

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// An identifier
    /// </summary>
    public sealed class Identifier : Node
    {
        /// <summary>
        /// Initialize
        /// </summary>
        public Identifier(string value)
        {
            Value = value;
        }

        /// <summary>
        /// Gets or sets the value
        /// </summary>
        public string Value { get; set; }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(Identifier lhs, Identifier rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(Identifier lhs, Identifier rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as Identifier;
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
