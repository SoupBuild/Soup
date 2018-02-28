// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.VisualStudioBuild
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

	internal class VSInstallationProperties
	{
		public string CampaignId { get; set; }
		public string ChannelManifestId { get; set; }
		public string Nickname { get; set; }
		public string SetupEngineFilePath { get; set; }
	}


	internal class VSInstallation
	{
		public string InstanceId { get; set; }
		public string InstallDate { get; set; }
		public string InstallationName { get; set; }
		public string InstallationPath { get; set; }
		public string InstallationVersion { get; set; }
		public string ProductId { get; set; }
		public string ProductPath { get; set; }
		public bool IsPrerelease { get; set; }
		public string DisplayName { get; set; }
		public string Description { get; set; }
		public string ChannelId { get; set; }
		public string ChannelPath { get; set; }
		public string ChannelUri { get; set; }
		public string EnginePath { get; set; }
		public string ReleaseNotes { get; set; }
		public string ThirdPartyNotices { get; set; }
		public VSInstallationCategory Catalog { get; set; }
		public VSInstallationProperties Properties { get; set; }
	}
}
