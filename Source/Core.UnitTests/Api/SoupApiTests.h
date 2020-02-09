// <copyright file="SoupApiTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Api::UnitTests
{
	class SoupApiTests
	{
	public:
		[[Fact]]
		void GetPackage_NotFound()
		{
			// Register the network listener
			auto testNetworkManager = std::make_shared<Network::MockNetworkManager>();
			auto scopedNetworkManager = Network::ScopedNetworkManagerRegister(testNetworkManager);

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"localhost",
				7071);
			testNetworkManager->RegisterClient(testHttpClient);

			auto uut = SoupApi();

			auto packageName = "MissingPackage";
			Assert::ThrowsRuntimeError([&uut, &packageName]() {
				auto result = uut.GetPackage(packageName);
			});

			// Verify expected network manager requests
			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: localhost:7071",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			// Verify expected http requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /api/v1/packages/MissingPackage",
				}),
				testHttpClient->GetRequests(),
				"Verify http requests match expected.");
		}

		[[Fact]]
		void GetPackage_Success()
		{
			// Register the test listener
			auto testNetworkManager = std::make_shared<Network::MockNetworkManager>();
			auto scopedNetworkManager = Network::ScopedNetworkManagerRegister(testNetworkManager);

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"localhost",
				7071);
			testNetworkManager->RegisterClient(testHttpClient);

			// Setup the expected http requests
			auto packageResult = std::string(
				R"({
					"name": "MyPackage"
				})");
			testHttpClient->SetResponse("/api/v1/packages/MyPackage", packageResult);

			auto uut = SoupApi();

			auto packageName = "MyPackage";
			auto result = uut.GetPackage(packageName);

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: localhost:7071",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /api/v1/packages/MyPackage",
				}),
				testHttpClient->GetRequests(),
				"Verify http requests match expected.");

			Assert::AreEqual(
				PackageResultModel("MyPackage"),
				result,
				"Verify final package matches expected");
		}
	};
}
