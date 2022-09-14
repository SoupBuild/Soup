// <copyright file="SMLManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Antlr4.Runtime;
using Antlr4.Runtime.Misc;
using System;
using System.Text;
using System.Threading.Tasks;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The recipe SML
	/// </summary>
	public static class SMLManager
	{
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		public static SMLTable Deserialize(string content)
		{
			var inputStream = new CodePointCharStream(content);
			var lexer = new SMLLexer(inputStream);
			var commonTokenStream = new CommonTokenStream(lexer);
			var parser = new SMLParser(commonTokenStream);
			parser.ErrorHandler = new BailErrorStrategy();

			try
			{
				var document = parser.document();

				var visitor = new SMLValueTableVisitor();
				var result = visitor.Visit(document).AsTable();
				return result;
			}
			catch (ParseCanceledException ex)
			{
				throw new InvalidOperationException($"Failed to parser SML file: {ex.Message}");
			}
		}

		/// <summary>
		/// Save the document syntax to the root file
		/// </summary>
		public static async Task SerializeAsync(SMLTable table, System.IO.Stream stream)
		{
			// Write out the entire root table
			using var writer = new System.IO.StreamWriter(stream, null, -1, true);

			await SerializeAsync(table, writer);
		}

		private static async Task SerializeAsync(SMLTable table, System.IO.StreamWriter writer)
		{
			await writer.WriteAsync('{');

			foreach (var value in table.GetValue())
			{
				await writer.WriteAsync(value.Key);
				await writer.WriteAsync(" = ");
				await SerializeAsync(value.Value, writer);
			}

			await writer.WriteAsync('}');
		}

		private static async Task SerializeAsync(SMLArray array, System.IO.StreamWriter writer)
		{
			await writer.WriteAsync('[');

			foreach (var value in array.GetValue())
			{
				await SerializeAsync(value, writer);
				await writer.WriteAsync(", ");
			}

			await writer.WriteAsync(']');
		}

		private static async Task SerializeAsync(SMLValue value, System.IO.StreamWriter writer)
		{
			switch (value.Type)
			{
				case SMLValueType.Empty:
					break;
				case SMLValueType.Boolean:
					await writer.WriteAsync(value.AsBoolean().ToString());
					break;
				case SMLValueType.Integer:
					await writer.WriteAsync(value.AsInteger().ToString());
					break;
				case SMLValueType.Float:
					await writer.WriteAsync(value.AsFloat().ToString());
					break;
				case SMLValueType.String:
					await writer.WriteAsync('"');
					await writer.WriteAsync(value.AsString());
					await writer.WriteAsync('"');
					break;
				case SMLValueType.Table:
					await SerializeAsync(value.AsTable(), writer);
					break;
				case SMLValueType.Array:
					await SerializeAsync(value.AsArray(), writer);
					break;
				default:
					throw new InvalidOperationException("Unknown SMLValueType");
			}
		}
	}
}
