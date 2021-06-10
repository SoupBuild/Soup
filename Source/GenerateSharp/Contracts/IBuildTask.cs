

using System.Collections.Generic;

namespace Soup.Build
{
	public interface IBuildTask
	{
		/// <summary>
		/// Execute the build task
		/// </summary>
		void Execute();
	}
}
