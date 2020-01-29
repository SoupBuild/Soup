#pragma once
#include "BitReader.h"

namespace Soup
{
	constexpr uint32_t FirstApplicationAbbreviationId = 4;
	enum class StandardAbbreviationId : char
	{
		// This abbrev ID marks the end of the current block.
		EndBlock = 0,
		// This abbrev ID marks the beginning of a new block.
		EnterSubBlock = 1,
		// This defines a new abbreviation.
		DefineAbbreviation = 2,
		// This ID specifies the definition of an unabbreviated record.
		UnabbreviatedRecord = 3,
	};

	constexpr uint32_t FirstApplicationBlockId = 8;
	enum class StandardBlockId
	{
		BlockInfo = 0,
	};

	enum class BlockRecordId
	{
		SetBlockId = 1,
		BlockName = 2,
		SetRecordName = 3,
	};

	enum class AbbreviationEncoding
	{
		Fixed = 1,
		VBR = 2,
		Array = 3,
		Char6 = 4,
		Blob = 5,
	};

	namespace ClangAST
	{
		enum class BlockId
		{
			AST = FirstApplicationBlockId,
			SourceManager,
			Preprocessor,
			DeclTypes,
			PreprocessorDetail,
			SubModule,
			Comments,
			Control,
			InputFiles,
			Options,
			Extension,
			UnhashedControl,
		};

		// Record codes for each kind of declaration in the Clang AST.
		enum class DeclarationCode
		{
			FirstElement = 51,
			TypeDef = FirstElement,
			TypeAlias,
			Enum,
			Record,
			EnumConstant,
			Function,
			Method,
			ObjC_Interface,
			ObjC_Protocol,
			ObjC_IVar,
			ObjC_AtDefsField,
			ObjC_Category,
			OBJC_CategoryImpl,
			OBJC_Implementation,
			OBJC_CompatibleAlias,
			OBJC_Property,
			OBJC_PropertyImpl,
			Field,
			MS_Property,
			Var,
			ImplicitParam,
			ParmVar,
			Decomposition,
			Binding,
			FileScopeASM,
			Block,
			Captured,
			ContextLexical,
			ContextVisible,
			Label,
			Namespace,
			NamespaceAlias,
			Using,
			UsingPack,
			UsingShadow,
			ConstrutorUsingShadow,
			UsingDirective,
			UnresolvedUsingValue,
			UnresolvedUsingTypename,
			LinkageSpec,
			Export,
			CPP_Record,
			CPP_DeductionGuide,
			CPP_Method,
			CPP_Constructor,
			CPP_Destructor,
			CPP_Conversion,
			AccessSpec,
			Friend,
			FriendTemplate,
			ClassTemplate,
			ClassTemplateSpecialization,
			ClassTemplatePartialSpecialization,
			VarTemplate,
			VarTemplateSpecialization,
			VarTemplatePartialSpecialization,
			FunctionTemplate,
			TemplateTypeParm,
			NonTypeTemplateParm,
			TemplateTemplateParm,
			TypeAliasTemplate,
			Concept,
			StaticAssert,
			CPP_BaseSpecifiers,
			CPP_ConstructorInitializers,
			IndirectField,
			ExpandedNonTypeTemplateParmPack,
			ExpandedTemplateTemplateParmPack,
			ClassScopeFunctionSpecification,
			Import,
			OMP_ThreadPrivate,
			OMP_Requires,
			OMP_Allocate,
			Empty,
			ObjC_TypeParam,
			OMP_CaptureDExpr,
			PragmaComment,
			PagmaDetectMismatch,
			OMP_DeclareMapper,
			OMP_DeclareReduction,
			LastElement = OMP_DeclareReduction,
		};

		// Record codes for each kind of statement or expression.
		enum class StatementCode
		{
			FirstElement = static_cast<uint32_t>(DeclarationCode::LastElement) + 1,
			STMT_STOP = FirstElement,
			STMT_NULL_PTR,
			STMT_REF_PTR,
			STMT_NULL,
			STMT_COMPOUND,
			STMT_CASE,
			STMT_DEFAULT,
			STMT_LABEL,
			STMT_ATTRIBUTED,
			STMT_IF,
			STMT_SWITCH,
			STMT_WHILE,
			STMT_DO,
			STMT_FOR,
			STMT_GOTO,
			STMT_INDIRECT_GOTO,
			STMT_CONTINUE,
			STMT_BREAK,
			STMT_RETURN,
			STMT_DECL,
			STMT_CAPTURED,
			STMT_GCCASM,
			STMT_MSASM,
			EXPR_CONSTANT,
			EXPR_PREDEFINED,
			EXPR_DECL_REF,
			EXPR_INTEGER_LITERAL,
			EXPR_FLOATING_LITERAL,
			EXPR_IMAGINARY_LITERAL,
			EXPR_STRING_LITERAL,
			EXPR_CHARACTER_LITERAL,
			EXPR_PAREN,
			EXPR_PAREN_LIST,
			EXPR_UNARY_OPERATOR,
			EXPR_OFFSETOF,
			EXPR_SIZEOF_ALIGN_OF,
			EXPR_ARRAY_SUBSCRIPT,
			EXPR_CALL,
			EXPR_MEMBER,
			EXPR_BINARY_OPERATOR,
			EXPR_COMPOUND_ASSIGN_OPERATOR,
			EXPR_CONDITIONAL_OPERATOR,
			EXPR_IMPLICIT_CAST,
			EXPR_CSTYLE_CAST,
			EXPR_COMPOUND_LITERAL,
			EXPR_EXT_VECTOR_ELEMENT,
			EXPR_INIT_LIST,
			EXPR_DESIGNATED_INIT,
			EXPR_DESIGNATED_INIT_UPDATE,
			EXPR_NO_INIT,
			EXPR_ARRAY_INIT_LOOP,
			EXPR_ARRAY_INIT_INDEX,
			EXPR_IMPLICIT_VALUE_INIT,
			EXPR_VA_ARG,
			EXPR_ADDR_LABEL,
			EXPR_STMT,
			EXPR_CHOOSE,
			EXPR_GNU_NULL,
			EXPR_SOURCE_LOC,
			EXPR_SHUFFLE_VECTOR,
			EXPR_CONVERT_VECTOR,
			EXPR_BLOCK,
			EXPR_GENERIC_SELECTION,
			EXPR_PSEUDO_OBJECT,
			EXPR_ATOMIC,
			EXPR_OBJC_STRING_LITERAL,
			EXPR_OBJC_BOXED_EXPRESSION,
			EXPR_OBJC_ARRAY_LITERAL,
			EXPR_OBJC_DICTIONARY_LITERAL,
			EXPR_OBJC_ENCODE,
			EXPR_OBJC_SELECTOR_EXPR,
			EXPR_OBJC_PROTOCOL_EXPR,
			EXPR_OBJC_IVAR_REF_EXPR,
			EXPR_OBJC_PROPERTY_REF_EXPR,
			EXPR_OBJC_SUBSCRIPT_REF_EXPR,
			EXPR_OBJC_KVC_REF_EXPR,
			EXPR_OBJC_MESSAGE_EXPR,
			EXPR_OBJC_ISA,
			EXPR_OBJC_INDIRECT_COPY_RESTORE,
			STMT_OBJC_FOR_COLLECTION,
			STMT_OBJC_CATCH,
			STMT_OBJC_FINALLY,
			STMT_OBJC_AT_TRY,
			STMT_OBJC_AT_SYNCHRONIZED,
			STMT_OBJC_AT_THROW,
			STMT_OBJC_AUTORELEASE_POOL,
			EXPR_OBJC_BOOL_LITERAL,
			EXPR_OBJC_AVAILABILITY_CHECK,
			STMT_CXX_CATCH,
			STMT_CXX_TRY,
			STMT_CXX_FOR_RANGE,
			EXPR_CXX_OPERATOR_CALL,
			EXPR_CXX_MEMBER_CALL,
			EXPR_CXX_CONSTRUCT,
			EXPR_CXX_INHERITED_CTOR_INIT,
			EXPR_CXX_TEMPORARY_OBJECT,
			EXPR_CXX_STATIC_CAST,
			EXPR_CXX_DYNAMIC_CAST,
			EXPR_CXX_REINTERPRET_CAST,
			EXPR_CXX_CONST_CAST,
			EXPR_CXX_FUNCTIONAL_CAST,
			EXPR_USER_DEFINED_LITERAL,
			EXPR_CXX_STD_INITIALIZER_LIST,
			EXPR_CXX_BOOL_LITERAL,
			EXPR_CXX_NULL_PTR_LITERAL,
			EXPR_CXX_TYPEID_EXPR,
			EXPR_CXX_TYPEID_TYPE,
			EXPR_CXX_THIS,
			EXPR_CXX_THROW,
			EXPR_CXX_DEFAULT_ARG,
			EXPR_CXX_DEFAULT_INIT,
			EXPR_CXX_BIND_TEMPORARY,
			EXPR_CXX_SCALAR_VALUE_INIT,
			EXPR_CXX_NEW,
			EXPR_CXX_DELETE,
			EXPR_CXX_PSEUDO_DESTRUCTOR,
			EXPR_EXPR_WITH_CLEANUPS,
			EXPR_CXX_DEPENDENT_SCOPE_MEMBER,
			EXPR_CXX_DEPENDENT_SCOPE_DECL_REF,
			EXPR_CXX_UNRESOLVED_CONSTRUCT,
			EXPR_CXX_UNRESOLVED_MEMBER,
			EXPR_CXX_UNRESOLVED_LOOKUP,
			EXPR_CXX_EXPRESSION_TRAIT,
			EXPR_CXX_NOEXCEPT,
			EXPR_OPAQUE_VALUE,
			EXPR_BINARY_CONDITIONAL_OPERATOR,
			EXPR_TYPE_TRAIT,
			EXPR_ARRAY_TYPE_TRAIT,
			EXPR_PACK_EXPANSION,
			EXPR_SIZEOF_PACK,
			EXPR_SUBST_NON_TYPE_TEMPLATE_PARM,
			EXPR_SUBST_NON_TYPE_TEMPLATE_PARM_PACK,
			EXPR_FUNCTION_PARM_PACK,
			EXPR_MATERIALIZE_TEMPORARY,
			EXPR_CXX_FOLD,
			EXPR_CUDA_KERNEL_CALL,
			EXPR_ASTYPE,
			EXPR_CXX_PROPERTY_REF_EXPR,
			EXPR_CXX_PROPERTY_SUBSCRIPT_EXPR,
			EXPR_CXX_UUIDOF_EXPR,
			EXPR_CXX_UUIDOF_TYPE,
			STMT_SEH_LEAVE,
			STMT_SEH_EXCEPT,
			STMT_SEH_FINALLY,
			STMT_SEH_TRY,
			STMT_OMP_PARALLEL_DIRECTIVE,
			STMT_OMP_SIMD_DIRECTIVE,
			STMT_OMP_FOR_DIRECTIVE,
			STMT_OMP_FOR_SIMD_DIRECTIVE,
			STMT_OMP_SECTIONS_DIRECTIVE,
			STMT_OMP_SECTION_DIRECTIVE,
			STMT_OMP_SINGLE_DIRECTIVE,
			STMT_OMP_MASTER_DIRECTIVE,
			STMT_OMP_CRITICAL_DIRECTIVE,
			STMT_OMP_PARALLEL_FOR_DIRECTIVE,
			STMT_OMP_PARALLEL_FOR_SIMD_DIRECTIVE,
			STMT_OMP_PARALLEL_SECTIONS_DIRECTIVE,
			STMT_OMP_TASK_DIRECTIVE,
			STMT_OMP_TASKYIELD_DIRECTIVE,
			STMT_OMP_BARRIER_DIRECTIVE,
			STMT_OMP_TASKWAIT_DIRECTIVE,
			STMT_OMP_FLUSH_DIRECTIVE,
			STMT_OMP_ORDERED_DIRECTIVE,
			STMT_OMP_ATOMIC_DIRECTIVE,
			STMT_OMP_TARGET_DIRECTIVE,
			STMT_OMP_TARGET_DATA_DIRECTIVE,
			STMT_OMP_TARGET_ENTER_DATA_DIRECTIVE,
			STMT_OMP_TARGET_EXIT_DATA_DIRECTIVE,
			STMT_OMP_TARGET_PARALLEL_DIRECTIVE,
			STMT_OMP_TARGET_PARALLEL_FOR_DIRECTIVE,
			STMT_OMP_TEAMS_DIRECTIVE,
			STMT_OMP_TASKGROUP_DIRECTIVE,
			STMT_OMP_CANCELLATION_POINT_DIRECTIVE,
			STMT_OMP_CANCEL_DIRECTIVE,
			STMT_OMP_TASKLOOP_DIRECTIVE,
			STMT_OMP_TASKLOOP_SIMD_DIRECTIVE,
			STMT_OMP_DISTRIBUTE_DIRECTIVE,
			STMT_OMP_TARGET_UPDATE_DIRECTIVE,
			STMT_OMP_DISTRIBUTE_PARALLEL_FOR_DIRECTIVE,
			STMT_OMP_DISTRIBUTE_PARALLEL_FOR_SIMD_DIRECTIVE,
			STMT_OMP_DISTRIBUTE_SIMD_DIRECTIVE,
			STMT_OMP_TARGET_PARALLEL_FOR_SIMD_DIRECTIVE,
			STMT_OMP_TARGET_SIMD_DIRECTIVE,
			STMT_OMP_TEAMS_DISTRIBUTE_DIRECTIVE,
			STMT_OMP_TEAMS_DISTRIBUTE_SIMD_DIRECTIVE,
			STMT_OMP_TEAMS_DISTRIBUTE_PARALLEL_FOR_SIMD_DIRECTIVE,
			STMT_OMP_TEAMS_DISTRIBUTE_PARALLEL_FOR_DIRECTIVE,
			STMT_OMP_TARGET_TEAMS_DIRECTIVE,
			STMT_OMP_TARGET_TEAMS_DISTRIBUTE_DIRECTIVE,
			STMT_OMP_TARGET_TEAMS_DISTRIBUTE_PARALLEL_FOR_DIRECTIVE,
			STMT_OMP_TARGET_TEAMS_DISTRIBUTE_PARALLEL_FOR_SIMD_DIRECTIVE,
			STMT_OMP_TARGET_TEAMS_DISTRIBUTE_SIMD_DIRECTIVE,
			EXPR_OMP_ARRAY_SECTION,
			EXPR_OBJC_BRIDGED_CAST,
			STMT_MS_DEPENDENT_EXISTS,
			EXPR_LAMBDA,
			STMT_COROUTINE_BODY,
			STMT_CORETURN,
			EXPR_COAWAIT,
			EXPR_COYIELD,
			EXPR_DEPENDENT_COAWAIT,
			LastElement = EXPR_DEPENDENT_COAWAIT,
		};

		enum class AccessSpecifier
		{
			Public,
			Protected,
			Private,
			None
		};
	}

	struct UnabbreviatedRecord
	{
		uint32_t Code;
		std::vector<uint64_t> Operands;
	};

	struct RecordInfo
	{
		uint32_t Id;
		std::string Name;
	};

	struct BlockInfo
	{
		uint32_t Id;
		std::string Name;
		std::vector<RecordInfo> Records;
	};

	struct BitCodeAbbreviationOperand
	{
		bool IsLiteral;
		AbbreviationEncoding Encoding;
		uint32_t Value;
	};

	struct BitCodeAbbreviation
	{
		std::vector<BitCodeAbbreviationOperand> Operands;
	};

	struct BitCodeRecord
	{
		uint32_t Code;
		std::vector<uint32_t> Values;
	};

	struct BlockContext
	{
		uint32_t BlockId;
		uint32_t AbbreviationLength;
		std::vector<BitCodeAbbreviation> Abbreviations;
	};

	class LLVMBitCodeParser
	{
	public:
		void Parse(std::istream& stream);

	private:
		BlockContext& GetActiveContext();
		std::string_view GetBlockName(uint32_t blockId);
		std::string_view GetRecordName(uint32_t blockId, uint32_t code);
		void ParseRootBlock();
		void ParseBlockInfoBlock();
		BitCodeAbbreviation ParseDefineAbbreviation();
		UnabbreviatedRecord ParseUnabbreviatedRecord();
		uint64_t ReadSingleOperand(const BitCodeAbbreviationOperand& operand);
		BitCodeRecord ParseRecord(uint32_t abbreviationId);

		// Clang AST Parsing
		void ParseControlBlock();
		void ParseASTBlock();
		void ParseDeclTypesBlock();

		void ParseExternalCPPBaseSpecifiers();
		void ParseDeclarationContext(const BitCodeRecord& record);
		void ParseDeclaration(const BitCodeRecord& record);

	private:
		std::vector<BlockInfo> m_blocks;

		// Active context
		std::unique_ptr<BitReader> m_reader;
		std::stack<BlockContext> m_context;
	};
}
