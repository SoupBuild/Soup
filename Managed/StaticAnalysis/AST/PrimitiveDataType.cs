using System;
using System.Collections.Generic;
using System.Text;

namespace Soup.StaticAnalysis.AST
{
    /// <summary>
    /// Primitive data type enumerations
    /// </summary>
    public enum PrimitiveDataType
    {
        /// <summary>
        /// Character
        /// </summary>
        Char,

        /// <summary>
        /// Character 16
        /// </summary>
        Char16,

        /// <summary>
        /// Character 32
        /// </summary>
        Char32,

        /// <summary>
        /// Wide character
        /// </summary>
        WChar,

        /// <summary>
        /// Boolean
        /// </summary>
        Bool,

        /// <summary>
        /// Short integer
        /// </summary>
        Short,

        /// <summary>
        /// Integer
        /// </summary>
        Int,

        /// <summary>
        /// Long Integer
        /// </summary>
        Long,

        /// <summary>
        /// Signed Integer
        /// </summary>
        Signed,

        /// <summary>
        /// Unsighed Integer
        /// </summary>
        Unsigned,

        /// <summary>
        /// Floating point
        /// </summary>
        Float,

        /// <summary>
        /// Double precision floating point
        /// </summary>
        Double,

        /// <summary>
        /// Void type
        /// </summary>
        Void,

        /// <summary>
        /// Auto type
        /// </summary>
        Auto,
    };

    /// <summary>
    /// Primitive data type node
    /// </summary>
    public class PrimitiveDataTypeNode : Node
    {
        /// <summary>
        /// Gets or sets the type
        /// </summary>
        public PrimitiveDataType Type { get; set; }

        /// <summary>
        /// Initialize
        /// </summary>
        public PrimitiveDataTypeNode(PrimitiveDataType type)
        {
            Type = Type;
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(PrimitiveDataTypeNode lhs, PrimitiveDataTypeNode rhs)
        {
            if (object.ReferenceEquals(lhs, null))
                return object.ReferenceEquals(rhs, null);
            else
                return lhs.Equals(rhs);
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(PrimitiveDataTypeNode lhs, PrimitiveDataTypeNode rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Equals
        /// </summary>
        public override bool Equals(object obj)
        {
            var other = obj as PrimitiveDataTypeNode;
            if (object.ReferenceEquals(other, null))
            {
                return false;
            }

            return Type == other.Type;
        }

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Type.GetHashCode();
        }
    }
}
