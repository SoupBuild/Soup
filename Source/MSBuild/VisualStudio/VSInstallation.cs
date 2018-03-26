// <copyright company="Soup" file="VSInstallation.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.VisualStudioBuild
{
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
