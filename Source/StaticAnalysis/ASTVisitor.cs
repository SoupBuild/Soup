
using Antlr4.Runtime.Misc;
using Soup.StaticAnalysis.AST;

namespace Soup.StaticAnalysis
{
    /// <summary>
    /// Abstract syntax tree visitor
    /// </summary>
    public class ASTVisitor : CppParserBaseVisitor<Node>
    {
        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typedefName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypedefName([NotNull] CppParser.TypedefNameContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceName([NotNull] CppParser.NamespaceNameContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceAlias()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceAlias([NotNull] CppParser.NamespaceAliasContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.className()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassName([NotNull] CppParser.ClassNameContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumName([NotNull] CppParser.EnumNameContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateName([NotNull] CppParser.TemplateNameContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.translationUnit()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTranslationUnit([NotNull] CppParser.TranslationUnitContext context)
        {
            var result = new TranslationUnit();

            // Check for the optional declaration sequences
            var declarationSequence = context.declarationSequence();
            if (declarationSequence != null)
            {
                result.Declarations = (DeclarationSequence)Visit(declarationSequence);
            }

            return result;
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.primaryExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPrimaryExpression([NotNull] CppParser.PrimaryExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.identifierExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitIdentifierExpression([NotNull] CppParser.IdentifierExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.unqualifiedIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUnqualifiedIdentifier([NotNull] CppParser.UnqualifiedIdentifierContext context)
        {
            var identifier = context.Identifier();
            if (identifier != null)
            {
                return new Identifier()
                {
                    Value = identifier.GetText(),
                };
            }

            return base.VisitUnqualifiedIdentifier(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.qualifiedIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitQualifiedIdentifier([NotNull] CppParser.QualifiedIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.nestedNameSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNestedNameSpecifier([NotNull] CppParser.NestedNameSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.lambdaExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLambdaExpression([NotNull] CppParser.LambdaExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.lambdaIntroducer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLambdaIntroducer([NotNull] CppParser.LambdaIntroducerContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.lambdaDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLambdaDeclarator([NotNull] CppParser.LambdaDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.lambdaCapture()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLambdaCapture([NotNull] CppParser.LambdaCaptureContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.captureDefault()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCaptureDefault([NotNull] CppParser.CaptureDefaultContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.captureList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCaptureList([NotNull] CppParser.CaptureListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.capture()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCapture([NotNull] CppParser.CaptureContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.simpleCapture()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSimpleCapture([NotNull] CppParser.SimpleCaptureContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerCapture()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerCapture([NotNull] CppParser.InitializerCaptureContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.foldExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFoldExpression([NotNull] CppParser.FoldExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.foldOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFoldOperator([NotNull] CppParser.FoldOperatorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.postfixExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPostfixExpression([NotNull] CppParser.PostfixExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.expressionList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExpressionList([NotNull] CppParser.ExpressionListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pseudoDestructorName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPseudoDestructorName([NotNull] CppParser.PseudoDestructorNameContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.unaryExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUnaryExpression([NotNull] CppParser.UnaryExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.unaryOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUnaryOperator([NotNull] CppParser.UnaryOperatorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewExpression([NotNull] CppParser.NewExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newPlacement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewPlacement([NotNull] CppParser.NewPlacementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newTypeIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewTypeIdentifier([NotNull] CppParser.NewTypeIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewDeclarator([NotNull] CppParser.NewDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noPointerNewDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoPointerNewDeclarator([NotNull] CppParser.NoPointerNewDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewInitializer([NotNull] CppParser.NewInitializerContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.deleteExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeleteExpression([NotNull] CppParser.DeleteExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noExceptionExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoExceptionExpression([NotNull] CppParser.NoExceptionExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.castExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCastExpression([NotNull] CppParser.CastExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerManipulationExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerManipulationExpression([NotNull] CppParser.PointerManipulationExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.multiplicativeExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMultiplicativeExpression([NotNull] CppParser.MultiplicativeExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.additiveExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAdditiveExpression([NotNull] CppParser.AdditiveExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.shiftExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitShiftExpression([NotNull] CppParser.ShiftExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.relationalExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitRelationalExpression([NotNull] CppParser.RelationalExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.equalityExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEqualityExpression([NotNull] CppParser.EqualityExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.andExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAndExpression([NotNull] CppParser.AndExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.exclusiveOrExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExclusiveOrExpression([NotNull] CppParser.ExclusiveOrExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.inclusiveOrExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInclusiveOrExpression([NotNull] CppParser.InclusiveOrExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.logicalAndExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLogicalAndExpression([NotNull] CppParser.LogicalAndExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.logicalOrExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLogicalOrExpression([NotNull] CppParser.LogicalOrExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.conditionalExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConditionalExpression([NotNull] CppParser.ConditionalExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.throwExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitThrowExpression([NotNull] CppParser.ThrowExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.assignmentExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAssignmentExpression([NotNull] CppParser.AssignmentExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.assignmentOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAssignmentOperator([NotNull] CppParser.AssignmentOperatorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.expression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExpression([NotNull] CppParser.ExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.constantExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConstantExpression([NotNull] CppParser.ConstantExpressionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.statement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitStatement([NotNull] CppParser.StatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerStatement([NotNull] CppParser.InitializerStatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.condition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCondition([NotNull] CppParser.ConditionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.labeledStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLabeledStatement([NotNull] CppParser.LabeledStatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.expressionStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExpressionStatement([NotNull] CppParser.ExpressionStatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.compoundStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCompoundStatement([NotNull] CppParser.CompoundStatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.statementSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitStatementSequence([NotNull] CppParser.StatementSequenceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.selectionStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSelectionStatement([NotNull] CppParser.SelectionStatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.iterationStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitIterationStatement([NotNull] CppParser.IterationStatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.forInitializerStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitForInitializerStatement([NotNull] CppParser.ForInitializerStatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.forRangeDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitForRangeDeclaration([NotNull] CppParser.ForRangeDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.forRangeInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitForRangeInitializer([NotNull] CppParser.ForRangeInitializerContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.jumpStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitJumpStatement([NotNull] CppParser.JumpStatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarationStatement()"/>.
        /// </summary>
        public override Node VisitDeclarationStatement([NotNull] CppParser.DeclarationStatementContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarationSequence()"/>.
        /// </summary>
        public override Node VisitDeclarationSequence([NotNull] CppParser.DeclarationSequenceContext context)
        {
            // Handle the recursive rule
            DeclarationSequence sequence;
            var childSequence = context.declarationSequence();
            if (childSequence != null)
            {
                sequence = (DeclarationSequence)Visit(childSequence);
            }
            else
            {
                sequence = new DeclarationSequence();
            }

            // Handle the new item
            var declaration = context.declaration();
            sequence.Declarations.Add((Declaration)Visit(declaration));

            return sequence;
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclaration([NotNull] CppParser.DeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.blockDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBlockDeclaration([NotNull] CppParser.BlockDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noDeclarationSpecifierFunctionDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoDeclarationSpecifierFunctionDeclaration([NotNull] CppParser.NoDeclarationSpecifierFunctionDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.aliasDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAliasDeclaration([NotNull] CppParser.AliasDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.simpleDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSimpleDeclaration([NotNull] CppParser.SimpleDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.staticAssertDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitStaticAssertDeclaration([NotNull] CppParser.StaticAssertDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.emptyDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEmptyDeclaration([NotNull] CppParser.EmptyDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeDeclaration([NotNull] CppParser.AttributeDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarationSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclarationSpecifier([NotNull] CppParser.DeclarationSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarationSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclarationSpecifierSequence([NotNull] CppParser.DeclarationSpecifierSequenceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.storageClassSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitStorageClassSpecifier([NotNull] CppParser.StorageClassSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.functionSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFunctionSpecifier([NotNull] CppParser.FunctionSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeSpecifier([NotNull] CppParser.TypeSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeSpecifierSequence([NotNull] CppParser.TypeSpecifierSequenceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.definingTypeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDefiningTypeSpecifier([NotNull] CppParser.DefiningTypeSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.definingTypeSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDefiningTypeSpecifierSequence([NotNull] CppParser.DefiningTypeSpecifierSequenceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.simpleTypeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSimpleTypeSpecifier([NotNull] CppParser.SimpleTypeSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeName([NotNull] CppParser.TypeNameContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarationTypeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclarationTypeSpecifier([NotNull] CppParser.DeclarationTypeSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.elaboratedTypeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitElaboratedTypeSpecifier([NotNull] CppParser.ElaboratedTypeSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumSpecifier([NotNull] CppParser.EnumSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumHead()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumHead([NotNull] CppParser.EnumHeadContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumHeadName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumHeadName([NotNull] CppParser.EnumHeadNameContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.opaqueEnumDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitOpaqueEnumDeclaration([NotNull] CppParser.OpaqueEnumDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumKey()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumKey([NotNull] CppParser.EnumKeyContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumBase()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumBase([NotNull] CppParser.EnumBaseContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumeratorList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumeratorList([NotNull] CppParser.EnumeratorListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumeratorDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumeratorDefinition([NotNull] CppParser.EnumeratorDefinitionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumerator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumerator([NotNull] CppParser.EnumeratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceDefinition([NotNull] CppParser.NamespaceDefinitionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namedNamespaceDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamedNamespaceDefinition([NotNull] CppParser.NamedNamespaceDefinitionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.unnamedNamespaceDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUnnamedNamespaceDefinition([NotNull] CppParser.UnnamedNamespaceDefinitionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.nestedNamespaceDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNestedNamespaceDefinition([NotNull] CppParser.NestedNamespaceDefinitionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enclosingNamespaceSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnclosingNamespaceSpecifier([NotNull] CppParser.EnclosingNamespaceSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceBody()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceBody([NotNull] CppParser.NamespaceBodyContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceAliasDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceAliasDefinition([NotNull] CppParser.NamespaceAliasDefinitionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.qualifiedNamespaceSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitQualifiedNamespaceSpecifier([NotNull] CppParser.QualifiedNamespaceSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.usingDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUsingDeclaration([NotNull] CppParser.UsingDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.usingDeclaratorList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUsingDeclaratorList([NotNull] CppParser.UsingDeclaratorListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.usingDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUsingDeclarator([NotNull] CppParser.UsingDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.usingDirective()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUsingDirective([NotNull] CppParser.UsingDirectiveContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.asmDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAsmDefinition([NotNull] CppParser.AsmDefinitionContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.linkageSpecification()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLinkageSpecification([NotNull] CppParser.LinkageSpecificationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeSpecifierSequence([NotNull] CppParser.AttributeSpecifierSequenceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeSpecifier([NotNull] CppParser.AttributeSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.alignmentSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAlignmentSpecifier([NotNull] CppParser.AlignmentSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeUsingPrefix()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeUsingPrefix([NotNull] CppParser.AttributeUsingPrefixContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeList([NotNull] CppParser.AttributeListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attribute()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttribute([NotNull] CppParser.AttributeContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeToken()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeToken([NotNull] CppParser.AttributeTokenContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeScopedToken()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeScopedToken([NotNull] CppParser.AttributeScopedTokenContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeNamespace()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeNamespace([NotNull] CppParser.AttributeNamespaceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeArgumentClause()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeArgumentClause([NotNull] CppParser.AttributeArgumentClauseContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.balancedTokenSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBalancedTokenSequence([NotNull] CppParser.BalancedTokenSequenceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.balancedToken()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBalancedToken([NotNull] CppParser.BalancedTokenContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerDeclaratorList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerDeclaratorList([NotNull] CppParser.InitializerDeclaratorListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerDeclarator([NotNull] CppParser.InitializerDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclarator([NotNull] CppParser.DeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerDeclarator([NotNull] CppParser.PointerDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noPointerDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoPointerDeclarator([NotNull] CppParser.NoPointerDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.parametersAndQualifiers()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitParametersAndQualifiers([NotNull] CppParser.ParametersAndQualifiersContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.trailingReturnType()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTrailingReturnType([NotNull] CppParser.TrailingReturnTypeContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerOperator([NotNull] CppParser.PointerOperatorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.constVolatileQualifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConstVolatileQualifierSequence([NotNull] CppParser.ConstVolatileQualifierSequenceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.constVolatileQualifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConstVolatileQualifier([NotNull] CppParser.ConstVolatileQualifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.referenceQualifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitReferenceQualifier([NotNull] CppParser.ReferenceQualifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declaratorIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclaratorIdentifier([NotNull] CppParser.DeclaratorIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeIdentifier([NotNull] CppParser.TypeIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.definingTypeIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDefiningTypeIdentifier([NotNull] CppParser.DefiningTypeIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.abstractDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAbstractDeclarator([NotNull] CppParser.AbstractDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerAbstractDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerAbstractDeclarator([NotNull] CppParser.PointerAbstractDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noPointerAbstractDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoPointerAbstractDeclarator([NotNull] CppParser.NoPointerAbstractDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.abstractPackDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAbstractPackDeclarator([NotNull] CppParser.AbstractPackDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noPointerAbstractPackDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoPointerAbstractPackDeclarator([NotNull] CppParser.NoPointerAbstractPackDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.parameterDeclarationClause()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitParameterDeclarationClause([NotNull] CppParser.ParameterDeclarationClauseContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.parameterDeclarationList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitParameterDeclarationList([NotNull] CppParser.ParameterDeclarationListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.parameterDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitParameterDeclaration([NotNull] CppParser.ParameterDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.functionDefinition()"/>.
        /// </summary>
        public override Node VisitFunctionDefinition([NotNull] CppParser.FunctionDefinitionContext context)
        {
            Identifier name = null;
            var declarator = Visit(context.declarator());
            if (declarator is Identifier identifier)
            {
                name = identifier;
            }

            var body = (CompoundStatement)Visit(context.functionBody());

            return new FunctionDefinition()
            {
                Name = name,
                Body = body,
            };
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.functionBody()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFunctionBody([NotNull] CppParser.FunctionBodyContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializer([NotNull] CppParser.InitializerContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.braceOrEqualInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBraceOrEqualInitializer([NotNull] CppParser.BraceOrEqualInitializerContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerClause()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerClause([NotNull] CppParser.InitializerClauseContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerList([NotNull] CppParser.InitializerListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.bracedInitializerList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBracedInitializerList([NotNull] CppParser.BracedInitializerListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.expressionOrBracedInitializerList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExpressionOrBracedInitializerList([NotNull] CppParser.ExpressionOrBracedInitializerListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassSpecifier([NotNull] CppParser.ClassSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classHead()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassHead([NotNull] CppParser.ClassHeadContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classHeadName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassHeadName([NotNull] CppParser.ClassHeadNameContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classVirtualSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassVirtualSpecifier([NotNull] CppParser.ClassVirtualSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classKey()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassKey([NotNull] CppParser.ClassKeyContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberSpecification()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberSpecification([NotNull] CppParser.MemberSpecificationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberDeclaration([NotNull] CppParser.MemberDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberDeclaratorList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberDeclaratorList([NotNull] CppParser.MemberDeclaratorListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberDeclarator([NotNull] CppParser.MemberDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.virtualSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitVirtualSpecifierSequence([NotNull] CppParser.VirtualSpecifierSequenceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.virtualSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitVirtualSpecifier([NotNull] CppParser.VirtualSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pureSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPureSpecifier([NotNull] CppParser.PureSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.baseClause()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBaseClause([NotNull] CppParser.BaseClauseContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.baseSpecifierList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBaseSpecifierList([NotNull] CppParser.BaseSpecifierListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.baseSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBaseSpecifier([NotNull] CppParser.BaseSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classOrDecltype()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassOrDecltype([NotNull] CppParser.ClassOrDecltypeContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.accessSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAccessSpecifier([NotNull] CppParser.AccessSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.conversionFunctionIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConversionFunctionIdentifier([NotNull] CppParser.ConversionFunctionIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.conversionTypeIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConversionTypeIdentifier([NotNull] CppParser.ConversionTypeIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.conversionDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConversionDeclarator([NotNull] CppParser.ConversionDeclaratorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.constructorInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConstructorInitializer([NotNull] CppParser.ConstructorInitializerContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberInitializerList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberInitializerList([NotNull] CppParser.MemberInitializerListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberInitializer([NotNull] CppParser.MemberInitializerContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberInitializerIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberInitializerIdentifier([NotNull] CppParser.MemberInitializerIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.operatorFunctionIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitOperatorFunctionIdentifier([NotNull] CppParser.OperatorFunctionIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.anyOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAnyOperator([NotNull] CppParser.AnyOperatorContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.literalOperatorIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLiteralOperatorIdentifier([NotNull] CppParser.LiteralOperatorIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateDeclaration([NotNull] CppParser.TemplateDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateParameterList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateParameterList([NotNull] CppParser.TemplateParameterListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateParameter()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateParameter([NotNull] CppParser.TemplateParameterContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeParameter()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeParameter([NotNull] CppParser.TypeParameterContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeParameterKey()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeParameterKey([NotNull] CppParser.TypeParameterKeyContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.simpleTemplateIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSimpleTemplateIdentifier([NotNull] CppParser.SimpleTemplateIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateIdentifier([NotNull] CppParser.TemplateIdentifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateArgumentList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateArgumentList([NotNull] CppParser.TemplateArgumentListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateArgument()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateArgument([NotNull] CppParser.TemplateArgumentContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typenameSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypenameSpecifier([NotNull] CppParser.TypenameSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.explicitInstantiation()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExplicitInstantiation([NotNull] CppParser.ExplicitInstantiationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.explicitSpecialization()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExplicitSpecialization([NotNull] CppParser.ExplicitSpecializationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.deductionGuide()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeductionGuide([NotNull] CppParser.DeductionGuideContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.tryBlock()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTryBlock([NotNull] CppParser.TryBlockContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.functionTryBlock()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFunctionTryBlock([NotNull] CppParser.FunctionTryBlockContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.handlerSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitHandlerSequence([NotNull] CppParser.HandlerSequenceContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.handler()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitHandler([NotNull] CppParser.HandlerContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.exceptionDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExceptionDeclaration([NotNull] CppParser.ExceptionDeclarationContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noExceptionSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoExceptionSpecifier([NotNull] CppParser.NoExceptionSpecifierContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.identifierList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitIdentifierList([NotNull] CppParser.IdentifierListContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.literal()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLiteral([NotNull] CppParser.LiteralContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.integerLiteral()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitIntegerLiteral([NotNull] CppParser.IntegerLiteralContext context)
        {
            // TODO : Log error when failed
            return new IntegerLiteral()
            {
                Value = context.GetText(),
            };
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.booleanLiteral()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBooleanLiteral([NotNull] CppParser.BooleanLiteralContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerLiteral()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerLiteral([NotNull] CppParser.PointerLiteralContext context)
        {
            return VisitChildren(context);
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.userDefinedLiteral()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUserDefinedLiteral([NotNull] CppParser.UserDefinedLiteralContext context)
        {
            return VisitChildren(context);
        }
    }
}
