// <copyright file="FileParameter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.IO;

namespace Soup.Build.Api.Client
{
	public class FileParameter
	{
		public FileParameter(Stream data)
			: this(data, null, null)
		{
		}

		public FileParameter(Stream data, string? fileName)
			: this(data, fileName, null)
		{
		}

		public FileParameter(Stream data, string? fileName, string? contentType)
		{
			Data = data;
			FileName = fileName;
			ContentType = contentType;
		}

		public Stream Data { get; private set; }

		public string? FileName { get; private set; }

		public string? ContentType { get; private set; }
	}
}