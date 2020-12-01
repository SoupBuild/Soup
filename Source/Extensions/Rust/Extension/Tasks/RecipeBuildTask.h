// <copyright file="RecipeBuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Rust
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	export class RecipeBuildTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		RecipeBuildTask() :
			_runBeforeList({ "Build" }),
			_runAfterList({ "ResolveTools" })
		{
		}

		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "RecipeBuild";
		}

		/// <summary>
		/// Get the run before list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunBeforeList() const noexcept override final
		{
			return _runBeforeList;
		}

		/// <summary>
		/// Get the run after list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunAfterList() const noexcept override final
		{
			return _runAfterList;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		Soup::Build::ApiCallResult TryExecute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::Utilities::BuildStateWrapper(buildState);

			try
			{
				Execute(buildStateWrapper);
				return Soup::Build::ApiCallResult::Success;
			}
			catch (const std::exception& ex)
			{
				buildStateWrapper.LogError(ex.what());
				return Soup::Build::ApiCallResult::Error;
			}
			catch(...)
			{
				// Unknown error
				return Soup::Build::ApiCallResult::Error;
			}
		}

	private:
		/// <summary>
		/// Internal implementation that can throw
		/// </summary>
		void Execute(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			auto rootTable = buildState.GetActiveState();
			auto recipeTable = rootTable.GetValue("Recipe").AsTable();
			auto buildTable = rootTable.EnsureValue("Build").EnsureTable();

			// Load the input properties
			auto packageRoot = Path(rootTable.GetValue("PackageRoot").AsString().GetValue());
			auto buildFlavor = std::string(rootTable.GetValue("BuildFlavor").AsString().GetValue());

			// Load Recipe properties
			auto name = std::string(recipeTable.GetValue("Name").AsString().GetValue());

			// Add the dependency static library closure to link if targeting an executable or dynamic library
			std::vector<Path> linkLibraries = std::vector<Path>();
			if (recipeTable.HasValue("LinkLibraries"))
			{
				for (auto value : recipeTable.GetValue("LinkLibraries").AsList().CopyAsPathVector())
				{
					// If relative then resolve to working directory
					if (value.HasRoot())
					{
						linkLibraries.push_back(std::move(value));
					}
					else
					{
						linkLibraries.push_back(packageRoot + value);
					}
				}
			}

			// Add the dependency runtime dependencies closure if present
			if (recipeTable.HasValue("RuntimeDependencies"))
			{
				std::vector<Path> runtimeDependencies = std::vector<Path>();
				if (buildTable.HasValue("RuntimeDependencies"))
				{
					runtimeDependencies = buildTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector();
				}

				for (auto value : recipeTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector())
				{
					// If relative then resolve to working directory
					if (value.HasRoot())
					{
						runtimeDependencies.push_back(std::move(value));
					}
					else
					{
						runtimeDependencies.push_back(packageRoot + value);
					}
				}

				buildTable.EnsureValue("RuntimeDependencies").SetValuePathList(runtimeDependencies);
			}

			// Load the extra library paths provided to the build system
			auto libraryFiles = std::vector<Path>();

			// Combine the defines with the default set and the platform
			auto preprocessorDefinitions = std::vector<std::string>();
			if (recipeTable.HasValue("Defines"))
			{
				preprocessorDefinitions = recipeTable.GetValue("Defines").AsList().CopyAsStringVector();
			}

			preprocessorDefinitions.push_back("SOUP_BUILD");
			preprocessorDefinitions.push_back("TRACE");

			// Build up arguments to build this individual recipe
			auto binaryDirectory = rootTable.GetValue("BinaryDirectory").AsString().GetValue();
			auto objectDirectory = rootTable.GetValue("ObjectDirectory").AsString().GetValue();

			// Load the source files if present
			auto sourceFiles = std::vector<std::string>();
			if (recipeTable.HasValue("Source"))
			{
				sourceFiles = recipeTable.GetValue("Source").AsList().CopyAsStringVector();
			}

			// Set the correct optimization level for the requested flavor
			auto enableOptimization = false;
			bool generateSourceDebugInfo = false;
			if (buildFlavor == "debug")
			{
				preprocessorDefinitions.push_back("DEBUG");
				generateSourceDebugInfo = true;
			}
			else if (buildFlavor == "release")
			{
				preprocessorDefinitions.push_back("RELEASE");
				enableOptimization = true;
			}
			else
			{
				buildState.LogError("Unknown build flavor type.");
				throw std::runtime_error("Unknown build flavors type.");
			}

			//////////////////////////////////////
			// DOT NET CORE 3.1 hard coded for now
			//////////////////////////////////////

			// Set the platform definitions 
			preprocessorDefinitions.push_back("NETCOREAPP");
			preprocessorDefinitions.push_back("NETCOREAPP3_1");

			// Set the platform libraries
			// TODO: Find this path...
			auto dotNetCore31Root = Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/3.1.0/ref/netcoreapp3.1/");
			auto platformLibraries = std::vector<Path>({
				Path("Microsoft.Rust.dll"),
				Path("Microsoft.VisualBasic.Core.dll"),
				Path("Microsoft.VisualBasic.dll"),
				Path("Microsoft.Win32.Primitives.dll"),
				Path("mscorlib.dll"),
				Path("netstandard.dll"),
				Path("System.AppContext.dll"),
				Path("System.Buffers.dll"),
				Path("System.Collections.Concurrent.dll"),
				Path("System.Collections.dll"),
				Path("System.Collections.Immutable.dll"),
				Path("System.Collections.NonGeneric.dll"),
				Path("System.Collections.Specialized.dll"),
				Path("System.ComponentModel.Annotations.dll"),
				Path("System.ComponentModel.DataAnnotations.dll"),
				Path("System.ComponentModel.dll"),
				Path("System.ComponentModel.EventBasedAsync.dll"),
				Path("System.ComponentModel.Primitives.dll"),
				Path("System.ComponentModel.TypeConverter.dll"),
				Path("System.Configuration.dll"),
				Path("System.Console.dll"),
				Path("System.Core.dll"),
				Path("System.Data.Common.dll"),
				Path("System.Data.DataSetExtensions.dll"),
				Path("System.Data.dll"),
				Path("System.Diagnostics.Contracts.dll"),
				Path("System.Diagnostics.Debug.dll"),
				Path("System.Diagnostics.DiagnosticSource.dll"),
				Path("System.Diagnostics.FileVersionInfo.dll"),
				Path("System.Diagnostics.Process.dll"),
				Path("System.Diagnostics.StackTrace.dll"),
				Path("System.Diagnostics.TextWriterTraceListener.dll"),
				Path("System.Diagnostics.Tools.dll"),
				Path("System.Diagnostics.TraceSource.dll"),
				Path("System.Diagnostics.Tracing.dll"),
				Path("System.dll"),
				Path("System.Drawing.dll"),
				Path("System.Drawing.Primitives.dll"),
				Path("System.Dynamic.Runtime.dll"),
				Path("System.Globalization.Calendars.dll"),
				Path("System.Globalization.dll"),
				Path("System.Globalization.Extensions.dll"),
				Path("System.IO.Compression.Brotli.dll"),
				Path("System.IO.Compression.dll"),
				Path("System.IO.Compression.FileSystem.dll"),
				Path("System.IO.Compression.ZipFile.dll"),
				Path("System.IO.dll"),
				Path("System.IO.FileSystem.dll"),
				Path("System.IO.FileSystem.DriveInfo.dll"),
				Path("System.IO.FileSystem.Primitives.dll"),
				Path("System.IO.FileSystem.Watcher.dll"),
				Path("System.IO.IsolatedStorage.dll"),
				Path("System.IO.MemoryMappedFiles.dll"),
				Path("System.IO.Pipes.dll"),
				Path("System.IO.UnmanagedMemoryStream.dll"),
				Path("System.Linq.dll"),
				Path("System.Linq.Expressions.dll"),
				Path("System.Linq.Parallel.dll"),
				Path("System.Linq.Queryable.dll"),
				Path("System.Memory.dll"),
				Path("System.Net.dll"),
				Path("System.Net.Http.dll"),
				Path("System.Net.HttpListener.dll"),
				Path("System.Net.Mail.dll"),
				Path("System.Net.NameResolution.dll"),
				Path("System.Net.NetworkInformation.dll"),
				Path("System.Net.Ping.dll"),
				Path("System.Net.Primitives.dll"),
				Path("System.Net.Requests.dll"),
				Path("System.Net.Security.dll"),
				Path("System.Net.ServicePoint.dll"),
				Path("System.Net.Sockets.dll"),
				Path("System.Net.WebClient.dll"),
				Path("System.Net.WebHeaderCollection.dll"),
				Path("System.Net.WebProxy.dll"),
				Path("System.Net.WebSockets.Client.dll"),
				Path("System.Net.WebSockets.dll"),
				Path("System.Numerics.dll"),
				Path("System.Numerics.Vectors.dll"),
				Path("System.ObjectModel.dll"),
				Path("System.Reflection.DispatchProxy.dll"),
				Path("System.Reflection.dll"),
				Path("System.Reflection.Emit.dll"),
				Path("System.Reflection.Emit.ILGeneration.dll"),
				Path("System.Reflection.Emit.Lightweight.dll"),
				Path("System.Reflection.Extensions.dll"),
				Path("System.Reflection.Metadata.dll"),
				Path("System.Reflection.Primitives.dll"),
				Path("System.Reflection.TypeExtensions.dll"),
				Path("System.Resources.Reader.dll"),
				Path("System.Resources.ResourceManager.dll"),
				Path("System.Resources.Writer.dll"),
				Path("System.Runtime.CompilerServices.Unsafe.dll"),
				Path("System.Runtime.CompilerServices.VisualC.dll"),
				Path("System.Runtime.dll"),
				Path("System.Runtime.Extensions.dll"),
				Path("System.Runtime.Handles.dll"),
				Path("System.Runtime.InteropServices.dll"),
				Path("System.Runtime.InteropServices.RuntimeInformation.dll"),
				Path("System.Runtime.InteropServices.WindowsRuntime.dll"),
				Path("System.Runtime.Intrinsics.dll"),
				Path("System.Runtime.Loader.dll"),
				Path("System.Runtime.Numerics.dll"),
				Path("System.Runtime.Serialization.dll"),
				Path("System.Runtime.Serialization.Formatters.dll"),
				Path("System.Runtime.Serialization.Json.dll"),
				Path("System.Runtime.Serialization.Primitives.dll"),
				Path("System.Runtime.Serialization.Xml.dll"),
				Path("System.Security.Claims.dll"),
				Path("System.Security.Cryptography.Algorithms.dll"),
				Path("System.Security.Cryptography.Csp.dll"),
				Path("System.Security.Cryptography.Encoding.dll"),
				Path("System.Security.Cryptography.Primitives.dll"),
				Path("System.Security.Cryptography.X509Certificates.dll"),
				Path("System.Security.dll"),
				Path("System.Security.Principal.dll"),
				Path("System.Security.SecureString.dll"),
				Path("System.ServiceModel.Web.dll"),
				Path("System.ServiceProcess.dll"),
				Path("System.Text.Encoding.CodePages.dll"),
				Path("System.Text.Encoding.dll"),
				Path("System.Text.Encoding.Extensions.dll"),
				Path("System.Text.Encodings.Web.dll"),
				Path("System.Text.Json.dll"),
				Path("System.Text.RegularExpressions.dll"),
				Path("System.Threading.Channels.dll"),
				Path("System.Threading.dll"),
				Path("System.Threading.Overlapped.dll"),
				Path("System.Threading.Tasks.Dataflow.dll"),
				Path("System.Threading.Tasks.dll"),
				Path("System.Threading.Tasks.Extensions.dll"),
				Path("System.Threading.Tasks.Parallel.dll"),
				Path("System.Threading.Thread.dll"),
				Path("System.Threading.ThreadPool.dll"),
				Path("System.Threading.Timer.dll"),
				Path("System.Transactions.dll"),
				Path("System.Transactions.Local.dll"),
				Path("System.ValueTuple.dll"),
				Path("System.Web.dll"),
				Path("System.Web.HttpUtility.dll"),
				Path("System.Windows.dll"),
				Path("System.Xml.dll"),
				Path("System.Xml.Linq.dll"),
				Path("System.Xml.ReaderWriter.dll"),
				Path("System.Xml.Serialization.dll"),
				Path("System.Xml.XDocument.dll"),
				Path("System.Xml.XmlDocument.dll"),
				Path("System.Xml.XmlSerializer.dll"),
				Path("System.Xml.XPath.dll"),
				Path("System.Xml.XPath.XDocument.dll"),
				Path("WindowsBase.dll"),
			});
			for (auto& library : platformLibraries)
			{
				libraryFiles.push_back(dotNetCore31Root + library);
			}

			// Pass along the build parameters to the build task
			buildTable.EnsureValue("TargetName").SetValueString(name);
			buildTable.EnsureValue("WorkingDirectory").SetValueString(packageRoot.ToString());
			buildTable.EnsureValue("ObjectDirectory").SetValueString(objectDirectory);
			buildTable.EnsureValue("BinaryDirectory").SetValueString(binaryDirectory);
			buildTable.EnsureValue("EnableOptimization").SetValueBoolean(enableOptimization);
			buildTable.EnsureValue("GenerateSourceDebugInfo").SetValueBoolean(generateSourceDebugInfo);

			buildTable.EnsureValue("PreprocessorDefinitions").EnsureList().Append(preprocessorDefinitions);
			buildTable.EnsureValue("LibraryFiles").EnsureList().Append(libraryFiles);
			buildTable.EnsureValue("Source").EnsureList().Append(sourceFiles);

			// Convert the recipe type to the required build type
			auto targetType = Soup::Rust::Compiler::BuildTargetType::Library;
			if (recipeTable.HasValue("Type"))
			{
				targetType = ParseType(recipeTable.GetValue("Type").AsString().GetValue());
			}

			buildTable.EnsureValue("TargetType").SetValueInteger(static_cast<int64_t>(targetType));
		}

		static Soup::Rust::Compiler::BuildTargetType ParseType(std::string_view value)
		{
			if (value == "Executable")
				return Soup::Rust::Compiler::BuildTargetType::Executable;
			else if (value == "Library")
				return Soup::Rust::Compiler::BuildTargetType::Library;
			else
				throw std::runtime_error("Unknown target type value.");
		}

	private:
		Soup::Build::Utilities::ReadOnlyStringList _runBeforeList;
		Soup::Build::Utilities::ReadOnlyStringList _runAfterList;
	};
}
