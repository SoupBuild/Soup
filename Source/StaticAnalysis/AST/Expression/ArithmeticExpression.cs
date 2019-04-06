

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Arithmetic Expression
    /// </summary>
    public abstract class ArithmeticExpression : ExpressionNode
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
