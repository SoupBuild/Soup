// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
	[Serializable]
	public class ClCompile
	{
		public ClCompile()
		{
		}

		[XmlElement]
		public string Optimization { get; set; }

		[XmlElement]
		public bool? FunctionLevelLinking { get; set; }
		public bool FunctionLevelLinkingSpecified => FunctionLevelLinking.HasValue;

		[XmlElement]
		public bool? IntrinsicFunctions { get; set; }
		public bool IntrinsicFunctionsSpecified => IntrinsicFunctions.HasValue;

		[XmlElement]
		public string PrecompiledHeader { get; set; }
			
		[XmlElement]
		public string WarningLevel { get; set; }

		[XmlElement]
		public bool? SDLCheck { get; set; }
		public bool SDLCheckSpecified => SDLCheck.HasValue;

		[XmlElement]
		public string PreprocessorDefinitions { get; set; }

		[XmlElement]
		public bool? ConformanceMode { get; set; }
		public bool ConformanceModeSpecified => ConformanceMode.HasValue;

		[XmlElement]
		public AdditionalIncludeDirectories AdditionalIncludeDirectories { get; set; }
	}
}
