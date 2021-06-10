// <copyright file="ResolveToolsTask.cs"),company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Soup.Build.CSharp
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	public class ResolveToolsTask : IBuildTask
	{
		private IBuildState _buildState;

		/// <summary>
		/// Get the run before list
		/// </summary>
		public static IReadOnlyList<string> RunBeforeList => new List<string>()
		{
		};

		/// <summary>
		/// Get the run after list
		/// </summary>
		public static IReadOnlyList<string> RunAfterList => new List<string>()
		{
		};

		public ResolveToolsTask(IBuildState buildState)
		{
			_buildState = buildState;

			if (!LifetimeManager.Has<IProcessManager>())
			{
				LifetimeManager.RegisterSingleton<IProcessManager, RuntimeProcessManager>();
			}
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
			var state = _buildState.ActiveState;
			var parameters = state["Parameters"].AsTable();
			var buildTable = state.EnsureValueTable("Build");

			var systemName = parameters["System"].AsString();
			var architectureName = parameters["Architecture"].AsString();

			if (systemName != "win32")
				throw new InvalidOperationException("Win32 is the only supported system... so far.");

			// Check if skip platform includes was specified
			bool skipPlatform = false;
			if (state.TryGetValue("SkipPlatform", out var skipPlatformValue))
			{
				skipPlatform = skipPlatformValue.AsBoolean();
			}

			// Find the location of the Windows SDK
			var visualStudioInstallRoot = FindVSInstallRoot();
			_buildState.LogTrace(TraceLevel.Information, "Using VS Installation: " + visualStudioInstallRoot.ToString());

			// Calculate the final Roslyn binaries folder
			var roslynFolder =
				visualStudioInstallRoot + new Path("/MSBuild/Current/Bin/Roslyn/");

			var cscToolPath = roslynFolder + new Path("csc.exe");

			// Save the build properties
			state["MSVS.InstallRoot"] = new Value(visualStudioInstallRoot.ToString());
			state["Roslyn.BinRoot"] = new Value(roslynFolder.ToString());
			state["Roslyn.CscToolPath"] = new Value(cscToolPath.ToString());

			// Save the platform libraries
			state["PlatformLibraries"] = new Value();
			var linkDependencies = new List<Path>();
			if (buildTable.TryGetValue("LinkDependencies", out var linkLibrariesValue))
			{
				linkDependencies = linkLibrariesValue.AsList().Select(value => new Path(value.AsString())).ToList();
			}

			linkDependencies.AddRange(GetPlatformLibraries());
			buildTable["LinkDependencies"] = new Value(new ValueList(
				linkDependencies.Select(value => new Value(value.ToString()))));
		}

		private Path FindVSInstallRoot()
		{
			// Find a copy of visual studio that has the required VisualCompiler
			var executablePath = new Path("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe");
			var workingDirectory = new Path("./");
			var argumentList = new List<string>()
			{
				"-latest",
				"-products",
				"*",
				"-requires",
				"Microsoft.VisualStudio.Component.Roslyn.Compiler",
				"-property",
				"installationPath",
			};

			// Check if we should include pre-release versions
			bool includePrerelease = true;
			if (includePrerelease)
			{
				argumentList.Add("-prerelease");
			}

			// Execute the requested target
			var arguments = CombineArguments(argumentList);
			_buildState.LogTrace(TraceLevel.Debug, executablePath.ToString() + " " + arguments);
			var process = LifetimeManager.Get<IProcessManager>().CreateProcess(
				executablePath,
				arguments,
				workingDirectory);
			process.Start();
			process.WaitForExit();

			var stdOut = process.GetStandardOutput();
			var stdErr = process.GetStandardError();
			var exitCode = process.GetExitCode();

			if (!string.IsNullOrEmpty(stdErr))
			{
				_buildState.LogTrace(TraceLevel.Error, stdErr);
				throw new InvalidOperationException("VSWhere failed.");
			}

			if (exitCode != 0)
			{
				// TODO: Return error code
				_buildState.LogTrace(TraceLevel.Error, "FAILED");
				throw new InvalidOperationException("VSWhere failed.");
			}

			// The first line is the path
			using (var reader = new System.IO.StringReader(stdOut))
			{
				var path = reader.ReadLine();
				if (path is null)
				{
					_buildState.LogTrace(TraceLevel.Error, "Failed to parse vswhere output.");
					throw new InvalidOperationException("Failed to parse vswhere output.");
				}

				return new Path(path);
			}
		}

		private IEnumerable<Path> GetPlatformLibraries()
		{
			// Set the platform libraries
			var path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/5.0.0/ref/net5.0/");
			var platformLibraries = new List<Path>()
			{
				new Path("Microsoft.CSharp.dll"),
				new Path("Microsoft.VisualBasic.Core.dll"),
				new Path("Microsoft.VisualBasic.dll"),
				new Path("Microsoft.Win32.Primitives.dll"),
				new Path("mscorlib.dll"),
				new Path("netstandard.dll"),
				new Path("System.AppContext.dll"),
				new Path("System.Buffers.dll"),
				new Path("System.Collections.Concurrent.dll"),
				new Path("System.Collections.dll"),
				new Path("System.Collections.Immutable.dll"),
				new Path("System.Collections.NonGeneric.dll"),
				new Path("System.Collections.Specialized.dll"),
				new Path("System.ComponentModel.Annotations.dll"),
				new Path("System.ComponentModel.DataAnnotations.dll"),
				new Path("System.ComponentModel.dll"),
				new Path("System.ComponentModel.EventBasedAsync.dll"),
				new Path("System.ComponentModel.Primitives.dll"),
				new Path("System.ComponentModel.TypeConverter.dll"),
				new Path("System.Configuration.dll"),
				new Path("System.Console.dll"),
				new Path("System.Core.dll"),
				new Path("System.Data.Common.dll"),
				new Path("System.Data.DataSetExtensions.dll"),
				new Path("System.Data.dll"),
				new Path("System.Diagnostics.Contracts.dll"),
				new Path("System.Diagnostics.Debug.dll"),
				new Path("System.Diagnostics.DiagnosticSource.dll"),
				new Path("System.Diagnostics.FileVersionInfo.dll"),
				new Path("System.Diagnostics.Process.dll"),
				new Path("System.Diagnostics.StackTrace.dll"),
				new Path("System.Diagnostics.TextWriterTraceListener.dll"),
				new Path("System.Diagnostics.Tools.dll"),
				new Path("System.Diagnostics.TraceSource.dll"),
				new Path("System.Diagnostics.Tracing.dll"),
				new Path("System.dll"),
				new Path("System.Drawing.dll"),
				new Path("System.Drawing.Primitives.dll"),
				new Path("System.Dynamic.Runtime.dll"),
				new Path("System.Formats.Asn1.dll"),
				new Path("System.Globalization.Calendars.dll"),
				new Path("System.Globalization.dll"),
				new Path("System.Globalization.Extensions.dll"),
				new Path("System.IO.Compression.Brotli.dll"),
				new Path("System.IO.Compression.dll"),
				new Path("System.IO.Compression.FileSystem.dll"),
				new Path("System.IO.Compression.ZipFile.dll"),
				new Path("System.IO.dll"),
				new Path("System.IO.FileSystem.dll"),
				new Path("System.IO.FileSystem.DriveInfo.dll"),
				new Path("System.IO.FileSystem.Primitives.dll"),
				new Path("System.IO.FileSystem.Watcher.dll"),
				new Path("System.IO.IsolatedStorage.dll"),
				new Path("System.IO.MemoryMappedFiles.dll"),
				new Path("System.IO.Pipes.dll"),
				new Path("System.IO.UnmanagedMemoryStream.dll"),
				new Path("System.Linq.dll"),
				new Path("System.Linq.Expressions.dll"),
				new Path("System.Linq.Parallel.dll"),
				new Path("System.Linq.Queryable.dll"),
				new Path("System.Memory.dll"),
				new Path("System.Net.dll"),
				new Path("System.Net.Http.dll"),
				new Path("System.Net.Http.Json.dll"),
				new Path("System.Net.HttpListener.dll"),
				new Path("System.Net.Mail.dll"),
				new Path("System.Net.NameResolution.dll"),
				new Path("System.Net.NetworkInformation.dll"),
				new Path("System.Net.Ping.dll"),
				new Path("System.Net.Primitives.dll"),
				new Path("System.Net.Requests.dll"),
				new Path("System.Net.Security.dll"),
				new Path("System.Net.ServicePoint.dll"),
				new Path("System.Net.Sockets.dll"),
				new Path("System.Net.WebClient.dll"),
				new Path("System.Net.WebHeaderCollection.dll"),
				new Path("System.Net.WebProxy.dll"),
				new Path("System.Net.WebSockets.Client.dll"),
				new Path("System.Net.WebSockets.dll"),
				new Path("System.Numerics.dll"),
				new Path("System.Numerics.Vectors.dll"),
				new Path("System.ObjectModel.dll"),
				new Path("System.Reflection.DispatchProxy.dll"),
				new Path("System.Reflection.dll"),
				new Path("System.Reflection.Emit.dll"),
				new Path("System.Reflection.Emit.ILGeneration.dll"),
				new Path("System.Reflection.Emit.Lightweight.dll"),
				new Path("System.Reflection.Extensions.dll"),
				new Path("System.Reflection.Metadata.dll"),
				new Path("System.Reflection.Primitives.dll"),
				new Path("System.Reflection.TypeExtensions.dll"),
				new Path("System.Resources.Reader.dll"),
				new Path("System.Resources.ResourceManager.dll"),
				new Path("System.Resources.Writer.dll"),
				new Path("System.Runtime.CompilerServices.Unsafe.dll"),
				new Path("System.Runtime.CompilerServices.VisualC.dll"),
				new Path("System.Runtime.dll"),
				new Path("System.Runtime.Extensions.dll"),
				new Path("System.Runtime.Handles.dll"),
				new Path("System.Runtime.InteropServices.dll"),
				new Path("System.Runtime.InteropServices.RuntimeInformation.dll"),
				new Path("System.Runtime.Intrinsics.dll"),
				new Path("System.Runtime.Loader.dll"),
				new Path("System.Runtime.Numerics.dll"),
				new Path("System.Runtime.Serialization.dll"),
				new Path("System.Runtime.Serialization.Formatters.dll"),
				new Path("System.Runtime.Serialization.Json.dll"),
				new Path("System.Runtime.Serialization.Primitives.dll"),
				new Path("System.Runtime.Serialization.Xml.dll"),
				new Path("System.Security.Claims.dll"),
				new Path("System.Security.Cryptography.Algorithms.dll"),
				new Path("System.Security.Cryptography.Csp.dll"),
				new Path("System.Security.Cryptography.Encoding.dll"),
				new Path("System.Security.Cryptography.Primitives.dll"),
				new Path("System.Security.Cryptography.X509Certificates.dll"),
				new Path("System.Security.dll"),
				new Path("System.Security.Principal.dll"),
				new Path("System.Security.SecureString.dll"),
				new Path("System.ServiceModel.Web.dll"),
				new Path("System.ServiceProcess.dll"),
				new Path("System.Text.Encoding.CodePages.dll"),
				new Path("System.Text.Encoding.dll"),
				new Path("System.Text.Encoding.Extensions.dll"),
				new Path("System.Text.Encodings.Web.dll"),
				new Path("System.Text.Json.dll"),
				new Path("System.Text.RegularExpressions.dll"),
				new Path("System.Threading.Channels.dll"),
				new Path("System.Threading.dll"),
				new Path("System.Threading.Overlapped.dll"),
				new Path("System.Threading.Tasks.Dataflow.dll"),
				new Path("System.Threading.Tasks.dll"),
				new Path("System.Threading.Tasks.Extensions.dll"),
				new Path("System.Threading.Tasks.Parallel.dll"),
				new Path("System.Threading.Thread.dll"),
				new Path("System.Threading.ThreadPool.dll"),
				new Path("System.Threading.Timer.dll"),
				new Path("System.Transactions.dll"),
				new Path("System.Transactions.Local.dll"),
				new Path("System.ValueTuple.dll"),
				new Path("System.Web.dll"),
				new Path("System.Web.HttpUtility.dll"),
				new Path("System.Windows.dll"),
				new Path("System.Xml.dll"),
				new Path("System.Xml.Linq.dll"),
				new Path("System.Xml.ReaderWriter.dll"),
				new Path("System.Xml.Serialization.dll"),
				new Path("System.Xml.XDocument.dll"),
				new Path("System.Xml.XmlDocument.dll"),
				new Path("System.Xml.XmlSerializer.dll"),
				new Path("System.Xml.XPath.dll"),
				new Path("System.Xml.XPath.XDocument.dll"),
				new Path("WindowsBase.dll"),
			};

			return platformLibraries.Select(value => path + value);
		}

		private static string CombineArguments(IList<string> args)
		{
			var argumentString = new StringBuilder();
			bool isFirst = true;
			foreach (var arg in args)
			{
				if (!isFirst)
					argumentString.Append(" ");

				argumentString.Append(arg);
				isFirst = false;
			}

			return argumentString.ToString();
		}
	}
}
