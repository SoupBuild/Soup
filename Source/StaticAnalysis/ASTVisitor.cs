
using Antlr4.Runtime.Misc;
using Soup.StaticAnalysis.AST;
using System;

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
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceName([NotNull] CppParser.NamespaceNameContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceAlias()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceAlias([NotNull] CppParser.NamespaceAliasContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.className()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassName([NotNull] CppParser.ClassNameContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumName([NotNull] CppParser.EnumNameContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateName([NotNull] CppParser.TemplateNameContext context)
        {
            throw new NotImplementedException();
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
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.identifierExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitIdentifierExpression([NotNull] CppParser.IdentifierExpressionContext context)
        {
            return new Identifier()
            {
                Value = context.GetText(),
            };
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
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.nestedNameSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNestedNameSpecifier([NotNull] CppParser.NestedNameSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.lambdaExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLambdaExpression([NotNull] CppParser.LambdaExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.lambdaIntroducer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLambdaIntroducer([NotNull] CppParser.LambdaIntroducerContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.lambdaDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLambdaDeclarator([NotNull] CppParser.LambdaDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.lambdaCapture()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLambdaCapture([NotNull] CppParser.LambdaCaptureContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.captureDefault()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCaptureDefault([NotNull] CppParser.CaptureDefaultContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.captureList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCaptureList([NotNull] CppParser.CaptureListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.capture()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCapture([NotNull] CppParser.CaptureContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.simpleCapture()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSimpleCapture([NotNull] CppParser.SimpleCaptureContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerCapture()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerCapture([NotNull] CppParser.InitializerCaptureContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.foldExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFoldExpression([NotNull] CppParser.FoldExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.foldOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFoldOperator([NotNull] CppParser.FoldOperatorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.postfixExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPostfixExpression([NotNull] CppParser.PostfixExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.expressionList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExpressionList([NotNull] CppParser.ExpressionListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pseudoDestructorName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPseudoDestructorName([NotNull] CppParser.PseudoDestructorNameContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.unaryExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUnaryExpression([NotNull] CppParser.UnaryExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.unaryOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUnaryOperator([NotNull] CppParser.UnaryOperatorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewExpression([NotNull] CppParser.NewExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newPlacement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewPlacement([NotNull] CppParser.NewPlacementContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newTypeIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewTypeIdentifier([NotNull] CppParser.NewTypeIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewDeclarator([NotNull] CppParser.NewDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noPointerNewDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoPointerNewDeclarator([NotNull] CppParser.NoPointerNewDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.newInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNewInitializer([NotNull] CppParser.NewInitializerContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.deleteExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeleteExpression([NotNull] CppParser.DeleteExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noExceptionExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoExceptionExpression([NotNull] CppParser.NoExceptionExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.castExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCastExpression([NotNull] CppParser.CastExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerManipulationExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerManipulationExpression([NotNull] CppParser.PointerManipulationExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.multiplicativeExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMultiplicativeExpression([NotNull] CppParser.MultiplicativeExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.additiveExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAdditiveExpression([NotNull] CppParser.AdditiveExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.shiftExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitShiftExpression([NotNull] CppParser.ShiftExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.relationalExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitRelationalExpression([NotNull] CppParser.RelationalExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.equalityExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEqualityExpression([NotNull] CppParser.EqualityExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.andExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAndExpression([NotNull] CppParser.AndExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.exclusiveOrExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExclusiveOrExpression([NotNull] CppParser.ExclusiveOrExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.inclusiveOrExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInclusiveOrExpression([NotNull] CppParser.InclusiveOrExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.logicalAndExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLogicalAndExpression([NotNull] CppParser.LogicalAndExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.logicalOrExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLogicalOrExpression([NotNull] CppParser.LogicalOrExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.conditionalExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConditionalExpression([NotNull] CppParser.ConditionalExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.throwExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitThrowExpression([NotNull] CppParser.ThrowExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.assignmentExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAssignmentExpression([NotNull] CppParser.AssignmentExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.assignmentOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAssignmentOperator([NotNull] CppParser.AssignmentOperatorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.expression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExpression([NotNull] CppParser.ExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.constantExpression()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConstantExpression([NotNull] CppParser.ConstantExpressionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerStatement([NotNull] CppParser.InitializerStatementContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.condition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCondition([NotNull] CppParser.ConditionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.labeledStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLabeledStatement([NotNull] CppParser.LabeledStatementContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.expressionStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExpressionStatement([NotNull] CppParser.ExpressionStatementContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.compoundStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitCompoundStatement([NotNull] CppParser.CompoundStatementContext context)
        {
            // Check for optional sequence
            CompoundStatement result = null;
            var sequence = context.statementSequence();
            if (sequence != null)
            {
                result = (CompoundStatement)Visit(sequence);
            }
            else
            {
                result = new CompoundStatement();
            }

            // TODO: Reference the source braces
            return result;
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.statementSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitStatementSequence([NotNull] CppParser.StatementSequenceContext context)
        {
            // Handle the recursive rule
            CompoundStatement sequence;
            var childSequence = context.statementSequence();
            if (childSequence != null)
            {
                sequence = (CompoundStatement)Visit(childSequence);
            }
            else
            {
                sequence = new CompoundStatement();
            }

            // Handle the new item
            var statement = (Statement)Visit(context.statement());
            sequence.Statements.Add(statement);

            return sequence;
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.selectionStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSelectionStatement([NotNull] CppParser.SelectionStatementContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.iterationStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitIterationStatement([NotNull] CppParser.IterationStatementContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.forInitializerStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitForInitializerStatement([NotNull] CppParser.ForInitializerStatementContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.forRangeDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitForRangeDeclaration([NotNull] CppParser.ForRangeDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.forRangeInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitForRangeInitializer([NotNull] CppParser.ForRangeInitializerContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.jumpStatement()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitJumpStatement([NotNull] CppParser.JumpStatementContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarationStatement()"/>.
        /// </summary>
        public override Node VisitDeclarationStatement([NotNull] CppParser.DeclarationStatementContext context)
        {
            throw new NotImplementedException();
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
        /// Visit a parse tree produced by <see cref="CppParser.blockDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBlockDeclaration([NotNull] CppParser.BlockDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noDeclarationSpecifierFunctionDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoDeclarationSpecifierFunctionDeclaration([NotNull] CppParser.NoDeclarationSpecifierFunctionDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.aliasDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAliasDeclaration([NotNull] CppParser.AliasDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.simpleDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSimpleDeclaration([NotNull] CppParser.SimpleDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.staticAssertDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitStaticAssertDeclaration([NotNull] CppParser.StaticAssertDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.emptyDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEmptyDeclaration([NotNull] CppParser.EmptyDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeDeclaration([NotNull] CppParser.AttributeDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarationSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclarationSpecifier([NotNull] CppParser.DeclarationSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarationSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclarationSpecifierSequence([NotNull] CppParser.DeclarationSpecifierSequenceContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.storageClassSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitStorageClassSpecifier([NotNull] CppParser.StorageClassSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.functionSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFunctionSpecifier([NotNull] CppParser.FunctionSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeSpecifier([NotNull] CppParser.TypeSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeSpecifierSequence([NotNull] CppParser.TypeSpecifierSequenceContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.definingTypeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDefiningTypeSpecifier([NotNull] CppParser.DefiningTypeSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.definingTypeSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDefiningTypeSpecifierSequence([NotNull] CppParser.DefiningTypeSpecifierSequenceContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.simpleTypeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSimpleTypeSpecifier([NotNull] CppParser.SimpleTypeSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeName([NotNull] CppParser.TypeNameContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarationTypeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclarationTypeSpecifier([NotNull] CppParser.DeclarationTypeSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.elaboratedTypeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitElaboratedTypeSpecifier([NotNull] CppParser.ElaboratedTypeSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumSpecifier([NotNull] CppParser.EnumSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumHead()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumHead([NotNull] CppParser.EnumHeadContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumHeadName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumHeadName([NotNull] CppParser.EnumHeadNameContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.opaqueEnumDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitOpaqueEnumDeclaration([NotNull] CppParser.OpaqueEnumDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumKey()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumKey([NotNull] CppParser.EnumKeyContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumBase()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumBase([NotNull] CppParser.EnumBaseContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumeratorList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumeratorList([NotNull] CppParser.EnumeratorListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumeratorDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumeratorDefinition([NotNull] CppParser.EnumeratorDefinitionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enumerator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnumerator([NotNull] CppParser.EnumeratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceDefinition([NotNull] CppParser.NamespaceDefinitionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namedNamespaceDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamedNamespaceDefinition([NotNull] CppParser.NamedNamespaceDefinitionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.unnamedNamespaceDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUnnamedNamespaceDefinition([NotNull] CppParser.UnnamedNamespaceDefinitionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.nestedNamespaceDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNestedNamespaceDefinition([NotNull] CppParser.NestedNamespaceDefinitionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.enclosingNamespaceSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitEnclosingNamespaceSpecifier([NotNull] CppParser.EnclosingNamespaceSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceBody()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceBody([NotNull] CppParser.NamespaceBodyContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.namespaceAliasDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNamespaceAliasDefinition([NotNull] CppParser.NamespaceAliasDefinitionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.qualifiedNamespaceSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitQualifiedNamespaceSpecifier([NotNull] CppParser.QualifiedNamespaceSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.usingDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUsingDeclaration([NotNull] CppParser.UsingDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.usingDeclaratorList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUsingDeclaratorList([NotNull] CppParser.UsingDeclaratorListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.usingDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUsingDeclarator([NotNull] CppParser.UsingDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.usingDirective()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUsingDirective([NotNull] CppParser.UsingDirectiveContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.asmDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAsmDefinition([NotNull] CppParser.AsmDefinitionContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.linkageSpecification()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLinkageSpecification([NotNull] CppParser.LinkageSpecificationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeSpecifierSequence([NotNull] CppParser.AttributeSpecifierSequenceContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeSpecifier([NotNull] CppParser.AttributeSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.alignmentSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAlignmentSpecifier([NotNull] CppParser.AlignmentSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeUsingPrefix()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeUsingPrefix([NotNull] CppParser.AttributeUsingPrefixContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeList([NotNull] CppParser.AttributeListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attribute()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttribute([NotNull] CppParser.AttributeContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeToken()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeToken([NotNull] CppParser.AttributeTokenContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeScopedToken()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeScopedToken([NotNull] CppParser.AttributeScopedTokenContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeNamespace()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeNamespace([NotNull] CppParser.AttributeNamespaceContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.attributeArgumentClause()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAttributeArgumentClause([NotNull] CppParser.AttributeArgumentClauseContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.balancedTokenSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBalancedTokenSequence([NotNull] CppParser.BalancedTokenSequenceContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.balancedToken()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBalancedToken([NotNull] CppParser.BalancedTokenContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerDeclaratorList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerDeclaratorList([NotNull] CppParser.InitializerDeclaratorListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerDeclarator([NotNull] CppParser.InitializerDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.declarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeclarator([NotNull] CppParser.DeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerDeclarator([NotNull] CppParser.PointerDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noPointerDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoPointerDeclarator([NotNull] CppParser.NoPointerDeclaratorContext context)
        {
            // Check if it was an identifier declarator
            var identifierExpression = context.identifierExpression();
            if (identifierExpression != null)
            {
                return Visit(identifierExpression);
            }

            throw new InvalidOperationException("Unknown sub type.");
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.parametersAndQualifiers()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitParametersAndQualifiers([NotNull] CppParser.ParametersAndQualifiersContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.trailingReturnType()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTrailingReturnType([NotNull] CppParser.TrailingReturnTypeContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerOperator([NotNull] CppParser.PointerOperatorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.constVolatileQualifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConstVolatileQualifierSequence([NotNull] CppParser.ConstVolatileQualifierSequenceContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.constVolatileQualifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConstVolatileQualifier([NotNull] CppParser.ConstVolatileQualifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.referenceQualifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitReferenceQualifier([NotNull] CppParser.ReferenceQualifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeIdentifier([NotNull] CppParser.TypeIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.definingTypeIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDefiningTypeIdentifier([NotNull] CppParser.DefiningTypeIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.abstractDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAbstractDeclarator([NotNull] CppParser.AbstractDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerAbstractDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerAbstractDeclarator([NotNull] CppParser.PointerAbstractDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noPointerAbstractDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoPointerAbstractDeclarator([NotNull] CppParser.NoPointerAbstractDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.abstractPackDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAbstractPackDeclarator([NotNull] CppParser.AbstractPackDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noPointerAbstractPackDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoPointerAbstractPackDeclarator([NotNull] CppParser.NoPointerAbstractPackDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.parameterDeclarationClause()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitParameterDeclarationClause([NotNull] CppParser.ParameterDeclarationClauseContext context)
        {
            // TODO Items
            return new ParameterList();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.parameterDeclarationList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitParameterDeclarationList([NotNull] CppParser.ParameterDeclarationListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.parameterDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitParameterDeclaration([NotNull] CppParser.ParameterDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.functionDefinition()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFunctionDefinition([NotNull] CppParser.FunctionDefinitionContext context)
        {
            // Check for optional return type
            Node returnType = null;
            if (context.declarationSpecifierSequence() != null)
            {
                returnType = null; // TODO : Visit(context.declarationSpecifierSequence());
            }

            // Analyze the declarator
            var declaratorContext = context.functionDeclarator();
            var identifier = (Identifier)Visit(declaratorContext.identifierExpression());
            var parameterList = (ParameterList)Visit(declaratorContext.functionParameters());
            // TODO Qualifiers
            // TODO Trailiing return type

            var body = (CompoundStatement)Visit(context.functionBody());

            return new FunctionDefinition()
            {
                ReturnType = returnType,
                Identifier = identifier,
                ParameterList = parameterList,
                Body = body,
            };
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.regularFunctionBody()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitRegularFunctionBody([NotNull] CppParser.RegularFunctionBodyContext context)
        {
            return new RegularFunctionBody()
            {
                Statements = (CompoundStatement)Visit(context.compoundStatement()),
            };
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializer([NotNull] CppParser.InitializerContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.braceOrEqualInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBraceOrEqualInitializer([NotNull] CppParser.BraceOrEqualInitializerContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerClause()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerClause([NotNull] CppParser.InitializerClauseContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.initializerList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitInitializerList([NotNull] CppParser.InitializerListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.bracedInitializerList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBracedInitializerList([NotNull] CppParser.BracedInitializerListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.expressionOrBracedInitializerList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExpressionOrBracedInitializerList([NotNull] CppParser.ExpressionOrBracedInitializerListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassSpecifier([NotNull] CppParser.ClassSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classHead()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassHead([NotNull] CppParser.ClassHeadContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classHeadName()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassHeadName([NotNull] CppParser.ClassHeadNameContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classVirtualSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassVirtualSpecifier([NotNull] CppParser.ClassVirtualSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classKey()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassKey([NotNull] CppParser.ClassKeyContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberSpecification()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberSpecification([NotNull] CppParser.MemberSpecificationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberDeclaration([NotNull] CppParser.MemberDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberDeclaratorList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberDeclaratorList([NotNull] CppParser.MemberDeclaratorListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberDeclarator([NotNull] CppParser.MemberDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.virtualSpecifierSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitVirtualSpecifierSequence([NotNull] CppParser.VirtualSpecifierSequenceContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.virtualSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitVirtualSpecifier([NotNull] CppParser.VirtualSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pureSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPureSpecifier([NotNull] CppParser.PureSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.baseClause()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBaseClause([NotNull] CppParser.BaseClauseContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.baseSpecifierList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBaseSpecifierList([NotNull] CppParser.BaseSpecifierListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.baseSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitBaseSpecifier([NotNull] CppParser.BaseSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.classOrDecltype()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitClassOrDecltype([NotNull] CppParser.ClassOrDecltypeContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.accessSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAccessSpecifier([NotNull] CppParser.AccessSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.conversionFunctionIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConversionFunctionIdentifier([NotNull] CppParser.ConversionFunctionIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.conversionTypeIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConversionTypeIdentifier([NotNull] CppParser.ConversionTypeIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.conversionDeclarator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConversionDeclarator([NotNull] CppParser.ConversionDeclaratorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.constructorInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitConstructorInitializer([NotNull] CppParser.ConstructorInitializerContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberInitializerList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberInitializerList([NotNull] CppParser.MemberInitializerListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberInitializer()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberInitializer([NotNull] CppParser.MemberInitializerContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.memberInitializerIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitMemberInitializerIdentifier([NotNull] CppParser.MemberInitializerIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.operatorFunctionIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitOperatorFunctionIdentifier([NotNull] CppParser.OperatorFunctionIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.anyOperator()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitAnyOperator([NotNull] CppParser.AnyOperatorContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.literalOperatorIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLiteralOperatorIdentifier([NotNull] CppParser.LiteralOperatorIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateDeclaration([NotNull] CppParser.TemplateDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateParameterList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateParameterList([NotNull] CppParser.TemplateParameterListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateParameter()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateParameter([NotNull] CppParser.TemplateParameterContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeParameter()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeParameter([NotNull] CppParser.TypeParameterContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typeParameterKey()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypeParameterKey([NotNull] CppParser.TypeParameterKeyContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.simpleTemplateIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitSimpleTemplateIdentifier([NotNull] CppParser.SimpleTemplateIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateIdentifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateIdentifier([NotNull] CppParser.TemplateIdentifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateArgumentList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateArgumentList([NotNull] CppParser.TemplateArgumentListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.templateArgument()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTemplateArgument([NotNull] CppParser.TemplateArgumentContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.typenameSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTypenameSpecifier([NotNull] CppParser.TypenameSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.explicitInstantiation()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExplicitInstantiation([NotNull] CppParser.ExplicitInstantiationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.explicitSpecialization()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExplicitSpecialization([NotNull] CppParser.ExplicitSpecializationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.deductionGuide()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitDeductionGuide([NotNull] CppParser.DeductionGuideContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.tryBlock()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitTryBlock([NotNull] CppParser.TryBlockContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.functionTryBlock()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitFunctionTryBlock([NotNull] CppParser.FunctionTryBlockContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.handlerSequence()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitHandlerSequence([NotNull] CppParser.HandlerSequenceContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.handler()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitHandler([NotNull] CppParser.HandlerContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.exceptionDeclaration()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitExceptionDeclaration([NotNull] CppParser.ExceptionDeclarationContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.noExceptionSpecifier()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitNoExceptionSpecifier([NotNull] CppParser.NoExceptionSpecifierContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.identifierList()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitIdentifierList([NotNull] CppParser.IdentifierListContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.literal()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitLiteral([NotNull] CppParser.LiteralContext context)
        {
            throw new NotImplementedException();
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
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.pointerLiteral()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitPointerLiteral([NotNull] CppParser.PointerLiteralContext context)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Visit a parse tree produced by <see cref="CppParser.userDefinedLiteral()"/>.
        /// </summary>
        /// <param name="context">The parse tree.</param>
        /// <return>The visitor result.</return>
        public override Node VisitUserDefinedLiteral([NotNull] CppParser.UserDefinedLiteralContext context)
        {
            throw new NotImplementedException();
        }
    }
}
