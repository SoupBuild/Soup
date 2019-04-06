

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Addition
    /// </summary>
    public class AdditionExpression : ArithmeticExpression
    {
        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(AdditionExpression lhs, AdditionExpression rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(AdditionExpression lhs, AdditionExpression rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as AdditionExpression;
            if (object.ReferenceEquals(other, null))
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
