

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The root translation unit node
    /// </summary>
    public sealed class TranslationUnit : Node
    {
        /// <summary>
        /// Gets or sets the option declaration sequence
        /// </summary>
        public DeclarationSequence Declarations { get; set; }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as TranslationUnit;
            if (other == null)
            {
                return false;
            }

            return Declarations == other.Declarations;
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
