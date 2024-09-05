// <copyright file="PathTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Xunit;

namespace Opal.UnitTests;

public class PathUnitTests
{
	[Fact]
	public void DefaultInitializer()
	{
		var uut = new Path();
		Assert.False(uut.HasRoot);
		Assert.False(uut.HasFileName);
		Assert.Equal("", uut.FileName);
		Assert.False(uut.HasFileStem);
		Assert.Equal("", uut.FileStem);
		Assert.False(uut.HasFileExtension);
		Assert.Equal("", uut.FileExtension);
		Assert.Equal("./", uut.ToString());
		Assert.Equal(".\\", uut.ToAlternateString());
	}

	[Fact]
	public void Empty()
	{
		var uut = new Path("");
		Assert.False(uut.HasRoot);
		Assert.False(uut.HasFileName);
		Assert.Equal("", uut.FileName);
		Assert.False(uut.HasFileStem);
		Assert.Equal("", uut.FileStem);
		Assert.False(uut.HasFileExtension);
		Assert.Equal("", uut.FileExtension);
		Assert.Equal("./", uut.ToString());
		Assert.Equal(".\\", uut.ToAlternateString());
	}

	[Fact]
	public void RelativePath_Simple()
	{
		var uut = new Path("./");
		Assert.False(uut.HasRoot);
		Assert.False(uut.HasFileName);
		Assert.Equal("", uut.FileName);
		Assert.False(uut.HasFileStem);
		Assert.Equal("", uut.FileStem);
		Assert.False(uut.HasFileExtension);
		Assert.Equal("", uut.FileExtension);
		Assert.Equal("./", uut.ToString());
		Assert.Equal(".\\", uut.ToAlternateString());
	}

	[Fact]
	public void RelativePath_Parent()
	{
		var uut = new Path("../");
		Assert.False(uut.HasRoot);
		Assert.False(uut.HasFileName);
		Assert.Equal("", uut.FileName);
		Assert.False(uut.HasFileStem);
		Assert.Equal("", uut.FileStem);
		Assert.False(uut.HasFileExtension);
		Assert.Equal("", uut.FileExtension);
		Assert.Equal("../", uut.ToString());
		Assert.Equal("..\\", uut.ToAlternateString());
	}

	[Fact]
	public void RelativePath_Complex()
	{
		var uut = new Path("./myfolder/anotherFolder/file.txt");
		Assert.False(uut.HasRoot);
		Assert.True(uut.HasFileName);
		Assert.Equal("file.txt", uut.FileName);
		Assert.True(uut.HasFileStem);
		Assert.Equal("file", uut.FileStem);
		Assert.True(uut.HasFileExtension);
		Assert.Equal(".txt", uut.FileExtension);
		Assert.Equal("./myfolder/anotherFolder/file.txt", uut.ToString());
		Assert.Equal(".\\myfolder\\anotherFolder\\file.txt", uut.ToAlternateString());
	}

	[Fact]
	public void LinuxRoot()
	{
		var uut = new Path("/");
		Assert.True(uut.HasRoot);
		Assert.Equal("", uut.Root);
		Assert.False(uut.HasFileName);
		Assert.Equal("", uut.FileName);
		Assert.False(uut.HasFileStem);
		Assert.Equal("", uut.FileStem);
		Assert.False(uut.HasFileExtension);
		Assert.Equal("", uut.FileExtension);
		Assert.Equal("/", uut.ToString());
		Assert.Equal("\\", uut.ToAlternateString());
	}

	[Fact]
	public void SimpleAbsolutePath()
	{
		var uut = new Path("C:/myfolder/anotherFolder/file.txt");
		Assert.True(uut.HasRoot, "Verify is root.");
		Assert.Equal("C:", uut.Root);
		Assert.True(uut.HasFileName);
		Assert.Equal("file.txt", uut.FileName);
		Assert.True(uut.HasFileStem);
		Assert.Equal("file", uut.FileStem);
		Assert.True(uut.HasFileExtension);
		Assert.Equal(".txt", uut.FileExtension);
		Assert.Equal("C:/myfolder/anotherFolder/file.txt", uut.ToString());
	}

	[Fact]
	public void AlternativeDirectoriesPath()
	{
		var uut = new Path("C:\\myfolder/anotherFolder\\file.txt");
		Assert.True(uut.HasRoot, "Verify is root.");
		Assert.Equal("C:", uut.Root);
		Assert.True(uut.HasFileName);
		Assert.Equal("file.txt", uut.FileName);
		Assert.True(uut.HasFileStem);
		Assert.Equal("file", uut.FileStem);
		Assert.True(uut.HasFileExtension);
		Assert.Equal(".txt", uut.FileExtension);
		Assert.Equal("C:/myfolder/anotherFolder/file.txt", uut.ToString());
	}

	[Fact]
	public void RemoveEmptyDirectoryInside()
	{
		var uut = new Path("C:/myfolder//file.txt");
		Assert.Equal("C:/myfolder/file.txt", uut.ToString());
	}

	[Fact]
	public void RemoveParentDirectoryInside()
	{
		var uut = new Path("C:/myfolder/../file.txt");
		Assert.Equal("C:/file.txt", uut.ToString());
	}

	[Fact]
	public void RemoveTwoParentDirectoryInside()
	{
		var uut = new Path("C:/myfolder/myfolder2/../../file.txt");
		Assert.Equal("C:/file.txt", uut.ToString());
	}

	[Fact]
	public void LeaveParentDirectoryAtStart()
	{
		var uut = new Path("../file.txt");
		Assert.Equal("../file.txt", uut.ToString());
	}

	[Fact]
	public void CurrentDirectoryAtStart()
	{
		var uut = new Path("./file.txt");
		Assert.Equal("./file.txt", uut.ToString());
	}

	[Fact]
	public void CurrentDirectoryAtStartAlternate()
	{
		var uut = new Path(".\\../file.txt");
		Assert.Equal("../file.txt", uut.ToString());
	}

	[Fact]
	public void Concatenate_Simple()
	{
		var path1 = new Path("C:/MyRootFolder");
		var path2 = new Path("./MyFolder/MyFile.txt");
		var uut = path1 + path2;

		Assert.Equal("C:/MyRootFolder/MyFolder/MyFile.txt", uut.ToString());
	}

	[Fact]
	public void Concatenate_Empty()
	{
		var path1 = new Path("C:/MyRootFolder");
		var path2 = new Path("");
		var uut = path1 + path2;

		// Changes the assumed file into a folder
		Assert.Equal("C:/MyRootFolder/", uut.ToString());
	}

	[Fact]
	public void Concatenate_RootFile()
	{
		var path1 = new Path("C:");
		var path2 = new Path("./MyFile.txt");
		var uut = path1 + path2;

		Assert.Equal("C:/MyFile.txt", uut.ToString());
	}

	[Fact]
	public void Concatenate_RootFolder()
	{
		var path1 = new Path("C:");
		var path2 = new Path("./MyFolder/");
		var uut = path1 + path2;

		Assert.Equal("C:/MyFolder/", uut.ToString());
	}

	[Fact]
	public void Concatenate_UpDirectory()
	{
		var path1 = new Path("C:/MyRootFolder");
		var path2 = new Path("../NewRoot/MyFile.txt");
		var uut = path1 + path2;

		Assert.Equal("C:/NewRoot/MyFile.txt", uut.ToString());
	}

	[Fact]
	public void Concatenate_UpDirectoryBeginning()
	{
		var path1 = new Path("../MyRootFolder");
		var path2 = new Path("../NewRoot/MyFile.txt");
		var uut = path1 + path2;

		Assert.Equal("../NewRoot/MyFile.txt", uut.ToString());
	}

	[Fact]
	public void SetFileExtension_Replace()
	{
		var uut = new Path("../MyFile.txt");
		uut.SetFileExtension("awe");

		Assert.Equal("../MyFile.awe", uut.ToString());
	}

	[Fact]
	public void SetFileExtension_Replace_Rooted()
	{
		var uut = new Path("C:/MyFolder/MyFile.txt");
		uut.SetFileExtension("awe");

		Assert.Equal("C:/MyFolder/MyFile.awe", uut.ToString());
	}

	[Fact]
	public void SetFileExtension_Add()
	{
		var uut = new Path("../MyFile");
		uut.SetFileExtension("awe");

		Assert.Equal("../MyFile.awe", uut.ToString());
	}

	[Fact]
	public void GetRelativeTo_Empty()
	{
		var uut = new Path("File.txt");
		var basePath = new Path("");

		var result = uut.GetRelativeTo(basePath);

		Assert.Equal("./File.txt", result.ToString());
	}

	[Fact]
	public void GetRelativeTo_SingleRelative()
	{
		var uut = new Path("./Folder/File.txt");
		var basePath = new Path("./Folder/");

		var result = uut.GetRelativeTo(basePath);

		Assert.Equal("./File.txt", result.ToString());
	}

	[Fact]
	public void GetRelativeTo_UpParentRelative()
	{
		var uut = new Path("../Folder/Target");
		var basePath = new Path("../Folder");

		var result = uut.GetRelativeTo(basePath);

		Assert.Equal("./Target", result.ToString());
	}

	[Fact]
	public void GetRelativeTo_MismatchRelative()
	{
		var uut = new Path("./Folder1/File.txt");
		var basePath = new Path("./Folder2/");

		var result = uut.GetRelativeTo(basePath);

		Assert.Equal("../Folder1/File.txt", result.ToString());
	}

	[Fact]
	public void GetRelativeTo_Rooted_DifferentRoot()
	{
		var uut = new Path("C:/Folder1/File.txt");
		var basePath = new Path("D:/Folder2/");

		var result = uut.GetRelativeTo(basePath);

		Assert.Equal("C:/Folder1/File.txt", result.ToString());
	}

	[Fact]
	public void GetRelativeTo_Rooted_SingleFolder()
	{
		var uut = new Path("C:/Folder1/File.txt");
		var basePath = new Path("C:/Folder1/");

		var result = uut.GetRelativeTo(basePath);

		Assert.Equal("./File.txt", result.ToString());
	}
}