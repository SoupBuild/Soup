

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// An identifier
    /// </summary>
    public sealed class Identifier : Node
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
            var other = obj as Identifier;
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
