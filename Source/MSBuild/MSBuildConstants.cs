// <copyright company="Soup" file="MSBuildConstants.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	/// <summary>
	/// The constants used during msbuild related tasks
	/// </summary>
	internal static class MSBuildConstants
	{
		public static string PackageIncludeFileName => "Include.props";

		public static string PackagePropertiesFileName => "Package.props";

		public static string VS2017ProjectName => "Build.vcxproj";
	}
}
