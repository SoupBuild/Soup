// <copyright file="SemanticVersionTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once

namespace Soup::UnitTests
{
    class SemanticVersionTests
    {
    public:
        [[Fact]]
        void Defaultinitializer()
        {
            auto uut = SemanticVersion();
            Assert::AreEqual(0, uut.GetMajor(), "Major version must match.");
            Assert::AreEqual(0, uut.GetMinor(), "Minor version must match.");
            Assert::AreEqual(0, uut.GetPatch(), "Patch version must match.");
        }

        [[Theory]]
        [[InlineData(1, 2, 3)]]
        [[InlineData(3, 2, 1)]]
        [[InlineData(1, 1, 1)]]
        [[InlineData(100, 200, 300)]]
        [[InlineData(1, 2, 333)]]
        [[InlineData(1, 222, 3)]]
        [[InlineData(111, 2, 3)]]
        void InitializeValues(int major, int minor, int patch)
        {
            auto uut = SemanticVersion(major, minor, patch);
            Assert::AreEqual(major, uut.GetMajor(), "Major version must match.");
            Assert::AreEqual(minor, uut.GetMinor(), "Minor version must match.");
            Assert::AreEqual(patch, uut.GetPatch(), "Patch version must match.");
        }

        [[Fact]]
        void OperatorEqual()
        {
            auto uut = SemanticVersion(1, 2, 3);

            Assert::AreEqual(
                SemanticVersion(1, 2, 3),
                uut,
                "Verify are equal.");
        }

        [[Fact]]
        void OperatorNotEqualMajor()
        {
            auto uut = SemanticVersion(1, 2, 3);

            Assert::AreNotEqual(
                SemanticVersion(11, 2, 3),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualMinor()
        {
            auto uut = SemanticVersion(1, 2, 3);

            Assert::AreNotEqual(
                SemanticVersion(1, 22, 3),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualPatch()
        {
            auto uut = SemanticVersion(1, 2, 3);

            Assert::AreNotEqual(
                SemanticVersion(1, 2, 33),
                uut,
                "Verify are not equal.");
        }

        [[Theory]]
        [[InlineData("1.2.3", 1, 2, 3)]]
        [[InlineData("3.2.1", 3, 2, 1)]]
        [[InlineData("1.1.1", 1, 1, 1)]]
        [[InlineData("100.200.300", 100, 200, 300)]]
        [[InlineData("1.2.333", 1, 2, 333)]]
        [[InlineData("1.222.3", 1, 222, 3)]]
        [[InlineData("111.2.3", 111, 2, 3)]]
        [[InlineData("0.0.0", 0, 0, 0)]]
        void ParseValues(std::string value, int major, int minor, int patch)
        {
            auto uut = SemanticVersion::Parse(value);
            Assert::AreEqual(
                SemanticVersion(major, minor, patch),
                uut,
                "Verify matches expected values.");
        }

        [[Theory]]
        [[InlineData("1.2.3", true, 1, 2, 3)]] // Success
        [[InlineData("0.0.0", true, 0, 0, 0)]] // All zeros success
        [[InlineData("", false, 0, 0, 0)]] // Empty fails
        [[InlineData("1", false, 0, 0, 0)]] // Major only fails
        [[InlineData("1.2", false, 0, 0, 0)]] // Major/Minor only fails
        void TryParseValues(std::string value, bool expectedResult, int major, int minor, int patch)
        {
            SemanticVersion uut;
            auto result = SemanticVersion::TryParse(value, uut);
            Assert::AreEqual(
                expectedResult,
                result,
                "Verify matches expected result.");
            Assert::AreEqual(
                SemanticVersion(major, minor, patch),
                uut,
                "Verify matches expected values.");
        }
    };
}
