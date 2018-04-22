// <copyright company="Soup" file="VSInstallationCategory.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	internal class VSInstallationCategory
	{
		public string BuildBranch { get; set; }

		public string BuildVersion { get; set; }

		public string Id { get; set; }

		public string LocalBuild { get; set; }

		public string ManifestName { get; set; }

		public string ManifestType { get; set; }

		public string ProductDisplayVersion { get; set; }

		public string ProductLine { get; set; }

		public string ProductLineVersion { get; set; }

		public string ProductMilestone { get; set; }

		public string ProductMilestoneIsPreRelease { get; set; }

		public string ProductName { get; set; }

		public string ProductPatchVersion { get; set; }

		public string ProductPreReleaseMilestoneSuffix { get; set; }

		public string ProductRelease { get; set; }

		public string ProductSemanticVersion { get; set; }
	}
}
