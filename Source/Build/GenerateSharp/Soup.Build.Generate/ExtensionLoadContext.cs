using System;
using System.Reflection;
using System.Runtime.Loader;

namespace Soup.Build.Generate
{
	class ExtensionLoadContext : AssemblyLoadContext
	{
		private AssemblyDependencyResolver _resolver;

		public ExtensionLoadContext(string assemblyPath)
		{
			_resolver = new AssemblyDependencyResolver(assemblyPath);
		}

		protected override Assembly? Load(AssemblyName assemblyName)
		{
			var assemblyPath = _resolver.ResolveAssemblyToPath(assemblyName);
			if (assemblyPath != null)
			{
				return LoadFromAssemblyPath(assemblyPath);
			}

			return null;
		}

		protected override IntPtr LoadUnmanagedDll(string unmanagedDllName)
		{
			var libraryPath = _resolver.ResolveUnmanagedDllToPath(unmanagedDllName);
			if (libraryPath != null)
			{
				return LoadUnmanagedDllFromPath(libraryPath);
			}

			return IntPtr.Zero;
		}
	}
}