// <copyright file="PackageReferenceTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class PackageReferenceTests
    {
    public:
        [[Fact]]
        void InitialzeNameVersion()
        {
            auto uut = PackageReference(
                "MyPackage",
                SemanticVersion(1, 2, 3));

            Assert::AreEqual("MyPackage", uut.GetName(), "Verify name matches expected.");
            Assert::AreEqual(SemanticVersion(1, 2, 3), uut.GetVersion(), "Verify version matches expected.");
            Assert::AreEqual("", uut.GetPath(), "Verify path matches expected.");
        }

        [[Fact]]
        void InitialzePath()
        {
            auto uut = PackageReference("../MyPackage");

            Assert::AreEqual("", uut.GetName(), "Verify name matches expected.");
            Assert::AreEqual(SemanticVersion(), uut.GetVersion(), "Verify version matches expected.");
            Assert::AreEqual("../MyPackage", uut.GetPath(), "Verify path matches expected.");
        }

        [[Fact]]
        void OperatorEqualNameVersion()
        {
            auto uut = PackageReference(
                "MyPackage",
                SemanticVersion(1, 2, 3));

            Assert::AreEqual(
                PackageReference(
                    "MyPackage",
                    SemanticVersion(1, 2, 3)),
                uut,
                "Verify are equal.");
        }

        [[Fact]]
        void OperatorEqualPath()
        {
            auto uut = PackageReference("../MyPackage");

            Assert::AreEqual(
                PackageReference("../MyPackage"),
                uut,
                "Verify are equal.");
        }

        [[Fact]]
        void OperatorNotEqualName()
        {
            auto uut = PackageReference(
                "MyPackage",
                SemanticVersion(1, 2, 3));

            Assert::AreNotEqual(
                PackageReference(
                    "MyPackage2",
                    SemanticVersion(1, 2, 3)),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualVersion()
        {
            auto uut = PackageReference(
                "MyPackage",
                SemanticVersion(1, 2, 3));

            Assert::AreNotEqual(
                PackageReference(
                    "MyPackage",
                    SemanticVersion(11, 2, 3)),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualPath()
        {
            auto uut = PackageReference("../MyPackage");

            Assert::AreNotEqual(
                PackageReference("../MyPackage2"),
                uut,
                "Verify are not equal.");
        }

        [[Theory]]
        [[InlineData("Name@1.2.3", "Name", Soup::SemanticVersion(1, 2, 3))]]
        void ParseNameVersionValues(std::string value, std::string name, SemanticVersion version)
        {
            auto uut = PackageReference::Parse(value);
            Assert::AreEqual(
                PackageReference(name, version),
                uut,
                "Verify matches expected values.");
        }

        [[Theory]]
        [[InlineData("../Path")]]
        void ParsePathValues(std::string value)
        {
            auto uut = PackageReference::Parse(value);
            Assert::AreEqual(
                PackageReference(value),
                uut,
                "Verify matches expected values.");
        }

        [[Theory]]
        [[InlineData("Package@1.2.3", true)]] // Success
        [[InlineData("Package@2", false)]] // Fails invalid version
        void TryParseValues(std::string value, bool expectedResult)
        {
            PackageReference uut;
            auto result = PackageReference::TryParse(value, uut);
            Assert::AreEqual(
                expectedResult,
                result,
                "Verify matches expected result.");
        }

        [[Theory]]
        [[InlineData("Name", Soup::SemanticVersion(1, 2, 3), "Name@1.2.3")]]
        void ToStringNameVersionValues(std::string name, SemanticVersion version, std::string expected)
        {
            auto uut = PackageReference(name, version);
            auto value = uut.ToString();
            Assert::AreEqual(
                expected,
                value,
                "Verify matches expected value.");
        }

        [[Theory]]
        [[InlineData("../Path")]]
        void ToStringPathValues(std::string path)
        {
            auto uut = PackageReference(path);
            auto value = uut.ToString();
            Assert::AreEqual(
                path,
                value,
                "Verify matches expected value.");
        }
    };
}
