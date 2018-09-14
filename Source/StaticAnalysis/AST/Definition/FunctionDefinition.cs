

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// The function definition
    /// </summary>
    public sealed class FunctionDefinition : Declaration
    {
        /// <summary>
        /// Gets or sets the return type
        /// </summary>
        public Node ReturnType { get; set; }

        /// <summary>
        /// Gets or sets the identifier
        /// </summary>
        public Identifier Identifier { get; set; }

        /// <summary>
        /// Gets or sets the parameter list
        /// </summary>
        public ParameterList ParameterList { get; set; }

        /// <summary>
        /// Gets or sets the body
        /// </summary>
        public Node Body { get; set; }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as FunctionDefinition;
            if (other == null)
            {
                return false;
            }

            return ReturnType.Equals(other.ReturnType) &&
                Identifier.Equals(other.Identifier) &&
                ParameterList.Equals(other.ParameterList) &&
                Body.Equals(other.Body);
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return ReturnType.GetHashCode() ^
                Identifier.GetHashCode() ^
                ParameterList.GetHashCode() ^
                Body.GetHashCode();
        }
    }
}
