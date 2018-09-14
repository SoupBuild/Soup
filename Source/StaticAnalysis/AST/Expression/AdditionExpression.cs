

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Addition
    /// </summary>
    public class AdditionExpression : ArithmeticExpression
    {

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as AdditionExpression;
            if (other == null)
            {
                return false;
            }

            return Left.Equals(other.Left) &&
                Right.Equals(other.Right);
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Left.GetHashCode() ^ Right.GetHashCode();
        }
    }
}
