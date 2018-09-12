

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The root translation unit node
    /// </summary>
    public class TranslationUnit : Node
    {
        /// <summary>
        /// Gets or sets the option declaration sequence
        /// </summary>
        public DeclarationSequence Declarations { get; set; }
    }
}
