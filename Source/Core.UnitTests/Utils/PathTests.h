// <copyright file="PathTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class PathTests
    {
    public:
        [[Fact]]
        void DefaultInitializer()
        {
            auto uut = Path();
            Assert::IsFalse(uut.HasRoot(), "Verify is no root.");
            Assert::AreEqual("", uut.GetRoot(), "Verify root matches.");
            Assert::IsFalse(uut.HasFileName(), "Verify has no filename.");
            Assert::AreEqual("", uut.GetFileName(), "Verify file name matches.");
            Assert::IsFalse(uut.HasFileStem(), "Verify has no file stem.");
            Assert::AreEqual("", uut.GetFileStem(), "Verify file stem matches.");
            Assert::IsFalse(uut.HasFileExtension(), "Verify has no file extension.");
            Assert::AreEqual("", uut.GetFileExtension(), "Verify file extension matches.");
            Assert::AreEqual("", uut.ToString(), "Verify string value matches.");
        }

        [[Fact]]
        void SimpleRelativePath()
        {
            auto uut = Path("myfolder/anotherfolder/file.txt");
            Assert::IsFalse(uut.HasRoot(), "Verify is no root.");
            Assert::AreEqual("", uut.GetRoot(), "Verify root matches.");
            Assert::IsTrue(uut.HasFileName(), "Verify has filename.");
            Assert::AreEqual("file.txt", uut.GetFileName(), "Verify file name matches.");
            Assert::IsTrue(uut.HasFileStem(), "Verify has file stem.");
            Assert::AreEqual("file", uut.GetFileStem(), "Verify file stem matches.");
            Assert::IsTrue(uut.HasFileExtension(), "Verify has file extension.");
            Assert::AreEqual(".txt", uut.GetFileExtension(), "Verify file extension matches.");
            Assert::AreEqual("myfolder/anotherfolder/file.txt", uut.ToString(), "Verify string value matches.");
        }

        [[Fact]]
        void SimpleAbsolutePath()
        {
            auto uut = Path("C:/myfolder/anotherfolder/file.txt");
            Assert::IsTrue(uut.HasRoot(), "Verify is root.");
            Assert::AreEqual("C:", uut.GetRoot(), "Verify root matches.");
            Assert::IsTrue(uut.HasFileName(), "Verify has filename.");
            Assert::AreEqual("file.txt", uut.GetFileName(), "Verify file name matches.");
            Assert::IsTrue(uut.HasFileStem(), "Verify has file stem.");
            Assert::AreEqual("file", uut.GetFileStem(), "Verify file stem matches.");
            Assert::IsTrue(uut.HasFileExtension(), "Verify has file extension.");
            Assert::AreEqual(".txt", uut.GetFileExtension(), "Verify file extension matches.");
            Assert::AreEqual("C:/myfolder/anotherfolder/file.txt", uut.ToString(), "Verify string value matches.");
        }
    };
}
