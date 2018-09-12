

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Arithmetic Expression
    /// </summary>
    public class ArithmeticExpressionNode : ExpressionNode
    {
        /// <summary>
        /// The left expression
        /// </summary>
        public ExpressionNode Left { get; set; }

        /// <summary>
        /// The right expression
        /// </summary>
        public ExpressionNode Right { get; set; }
    }
}
