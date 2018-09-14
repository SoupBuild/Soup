

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Return statement node
    /// </summary>
    public sealed class ReturnStatement : Statement
    {
        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as ReturnStatement;
            if (other == null)
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return 0;
        }
    }
}
