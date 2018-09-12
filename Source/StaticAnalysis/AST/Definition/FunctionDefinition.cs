

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The function definition
    /// </summary>
    public class FunctionDefinition : Declaration
    {
        /// <summary>
        /// Gets or sets the name
        /// </summary>
        public Identifier Name { get; set; }

        /// <summary>
        /// Gets or sets the body
        /// </summary>
        public CompoundStatement Body { get; set; }
    }
}
