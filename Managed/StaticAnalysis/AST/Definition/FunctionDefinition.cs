

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
        public DeclarationSpecifierSequence ReturnType { get; set; }

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
        /// Equality operator
        /// </summary>
        public static bool operator ==(FunctionDefinition lhs, FunctionDefinition rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(FunctionDefinition lhs, FunctionDefinition rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as FunctionDefinition;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return ReturnType == other.ReturnType &&
                Identifier ==  other.Identifier &&
                ParameterList ==  other.ParameterList &&
                Body == other.Body;
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
