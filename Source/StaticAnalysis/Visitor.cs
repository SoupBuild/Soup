
using Antlr4.Runtime.Misc;

namespace Soup.StaticAnalysis
{
    /// <summary>
    /// Visitor
    /// </summary>
    public class Visitor : Cpp17BaseVisitor<object>
    {
        /// <summary>
        /// Visit namespace definition
        /// </summary>
        public override object VisitNamespaceDefinition(
            [NotNull] Cpp17Parser.NamespaceDefinitionContext context)
        {
            return base.VisitNamespaceDefinition(context);
        }
    }
}
