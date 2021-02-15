

namespace Soup.Build
{
	public interface IBuildState
	{
		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		IValueTable ActiveState { get; }

		/// <summary>
		/// Get a reference to the shared state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		IValueTable SharedState { get; }

		/// <summary>
		/// Create a build operation
		/// </summary>
		void CreateOperation(
			string title,
			string executable,
			string arguments,
			string workingDirectory,
			string[] declaredInput,
			string[] declaredOutput);
	}
}
