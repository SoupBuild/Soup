// <copyright file="PackageManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class PackageManagerTests
	{
	public:
		[[Fact]]
		void InstallPackageReference_MissingRecipe_Throws()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto packageName = "TheirPackage";
			Assert::ThrowsRuntimeError([&packageName]() {
				PackageManager::InstallPackageReference(packageName);
			});

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: Recipe.toml",
					"INFO: Recipe file does not exist.",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void InstallPackageReference_Latest_Success()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener =  ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test listener
			auto testNetworkManager = std::make_shared<Network::MockNetworkManager>();
			auto scopedNetworkManager = Network::ScopedNetworkManagerRegister(testNetworkManager);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Version = "1.2.3"
				)")));

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"api.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testHttpClient);

			// Setup the expected http requests
			auto packageResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({
					"name": "TheirPackage",
					"latest": "2.2.2"
				})");
			testHttpClient->AddGetResponse("/v1/packages/TheirPackage", packageResult);

			auto packageContent = std::vector<unsigned char>(
			{
				0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C, 0x00, 0x04, 0x4A, 0xD5, 0xC7, 0xEB, 0x42, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xBD, 0x4C, 0x8C, 
				0x01, 0x00, 0x3D, 0x4E, 0x61, 0x6D, 0x65, 0x20, 0x3D, 0x20, 0x22, 0x4D, 0x79, 0x50, 0x72, 0x6F, 
				0x6A, 0x65, 0x63, 0x74, 0x22, 0x0D, 0x0A, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x20, 0x3D, 
				0x20, 0x22, 0x31, 0x2E, 0x32, 0x2E, 0x33, 0x22, 0x0D, 0x0A, 0x50, 0x75, 0x62, 0x6C, 0x69, 0x63, 
				0x20, 0x3D, 0x20, 0x22, 0x4D, 0x6F, 0x64, 0x75, 0x6C, 0x65, 0x2E, 0x63, 0x70, 0x70, 0x22, 0x0D, 
				0x0A, 0x00, 0x01, 0x04, 0x06, 0x00, 0x01, 0x09, 0x42, 0x00, 0x07, 0x0B, 0x01, 0x00, 0x01, 0x21, 
				0x21, 0x01, 0x00, 0x0C, 0x3E, 0x00, 0x08, 0x0A, 0x01, 0x90, 0x59, 0x1A, 0xB9, 0x00, 0x00, 0x05, 
				0x01, 0x19, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 
				0x19, 0x00, 0x52, 0x00, 0x65, 0x00, 0x63, 0x00, 0x69, 0x00, 0x70, 0x00, 0x65, 0x00, 0x2E, 0x00, 
				0x74, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x19, 0x02, 0x00, 0x00, 0x14, 0x0A, 
				0x01, 0x00, 0x5F, 0xFB, 0xB6, 0x43, 0xE8, 0xED, 0xD5, 0x01, 0x15, 0x06, 0x01, 0x00, 0x20, 0x00, 
				0x00, 0x00, 0x00, 0x00,
			});
			auto packageContentString = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				std::string(packageContent.begin(), packageContent.end()));
			testHttpClient->AddGetResponse("/v1/packages/TheirPackage/v2.2.2/download", packageContentString);

			auto packageName = "TheirPackage";
			PackageManager::InstallPackageReference(packageName);

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: Recipe.toml",
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"HIGH: Install Package: TheirPackage",
					"DIAG: /v1/packages/TheirPackage",
					"HIGH: Latest Version: 2.2.2",
					"HIGH: Downloading package",
					"DIAG: /v1/packages/TheirPackage/v2.2.2/download",
					"INFO: ExtractStart: 62",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 62",
					"INFO: ExtractGetStream: C:/Users/Me/.soup/packages/.staging/2.2.2/Recipe.toml",
					"INFO: ExtractOnOperationStart",
					"INFO: ExtractOperationCompleted",
					"INFO: ExtractProgress: 62",
					"INFO: ExtractDone",
					"INFO: Deleting staging directory",
					"INFO: Adding reference to recipe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Recipe.toml",
					"OpenReadBinary: Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage/2.2.2",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/2.2.2",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"Exists: C:/Users/Me/.soup/packages/.staging/2.2.2/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/2.2.2/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/2.2.2/Recipe.toml",
					"SetLastWriteTime: C:/Users/Me/.soup/packages/.staging/2.2.2/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage",
					"CreateDirectory: C:/Users/Me/.soup/packages/TheirPackage",
					"Rename: [C:/Users/Me/.soup/packages/.staging/2.2.2] -> [C:/Users/Me/.soup/packages/TheirPackage/2.2.2]",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
					"OpenWrite: Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: api.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /v1/packages/TheirPackage",
					"Get: /v1/packages/TheirPackage/v2.2.2/download",
				}),
				testHttpClient->GetRequests(),
				"Verify http requests match expected.");

			// Verify the contents of the recipe file
			std::string expectedFinalRecipe = 
R"(Name = "MyPackage"
Version = "1.2.3"
Dependencies = [
"TheirPackage@2.2.2",
]
)";
			auto& mockRecipeFile = fileSystem->GetMockFile(Path("Recipe.toml"));
			Assert::AreEqual(expectedFinalRecipe, mockRecipeFile->Content.str(), "Verify recipe file contents.");
		}

		[[Fact]]
		void PublishPackage_Success()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto consoleManager = std::make_shared<IO::MockConsoleManager>();
			auto scopedConsoleManager = IO::ScopedConsoleManagerRegister(consoleManager);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test listener
			auto testNetworkManager = std::make_shared<Network::MockNetworkManager>();
			auto scopedNetworkManager = Network::ScopedNetworkManagerRegister(testNetworkManager);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Version = "1.2.3"
				)")));

			// Create the required http client
			auto testAuthHttpClient = std::make_shared<Network::MockHttpClient>(
				"auth.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testAuthHttpClient);

			auto testApiHttpClient = std::make_shared<Network::MockHttpClient>(
				"api.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testApiHttpClient);

			// Setup the expected http requests
			auto openIdConfigResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({"issuer":"https://auth.soupbuild.com","jwks_uri":"https://auth.soupbuild.com/.well-known/openid-configuration/jwks","authorization_endpoint":"https://auth.soupbuild.com/connect/authorize","token_endpoint":"https://auth.soupbuild.com/connect/token","userinfo_endpoint":"https://auth.soupbuild.com/connect/userinfo","end_session_endpoint":"https://auth.soupbuild.com/connect/endsession","check_session_iframe":"https://auth.soupbuild.com/connect/checksession","revocation_endpoint":"https://auth.soupbuild.com/connect/revocation","introspection_endpoint":"https://auth.soupbuild.com/connect/introspect","device_authorization_endpoint":"https://auth.soupbuild.com/connect/deviceauthorization","frontchannel_logout_supported":true,"frontchannel_logout_session_supported":true,"backchannel_logout_supported":true,"backchannel_logout_session_supported":true,"scopes_supported":["openid","profile","soup_api","offline_access"],"claims_supported":["sub","name","family_name","given_name","middle_name","nickname","preferred_username","profile","picture","website","gender","birthdate","zoneinfo","locale","updated_at"],"grant_types_supported":["authorization_code","client_credentials","refresh_token","implicit","password","urn:ietf:params:oauth:grant-type:device_code"],"response_types_supported":["code","token","id_token","id_token token","code id_token","code token","code id_token token"],"response_modes_supported":["form_post","query","fragment"],"token_endpoint_auth_methods_supported":["client_secret_basic","client_secret_post"],"id_token_signing_alg_values_supported":["RS256"],"subject_types_supported":["public"],"code_challenge_methods_supported":["plain","S256"],"request_parameter_supported":true})");
			testAuthHttpClient->AddGetResponse("/.well-known/openid-configuration", openIdConfigResult);
			
			auto connectTokenResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({"access_token":"SUPER_TOKEN","expires_in":3600,"token_type":"Bearer","scope":"soup_api"})");
			testAuthHttpClient->AddPostResponse("/connect/token", connectTokenResult);

			auto packageResult = Network::HttpResponse(
				Network::HttpStatusCode::Created,
				"");
			testApiHttpClient->AddPutResponse("/v1/packages/MyPackage/v1.2.3", packageResult);

			auto packageName = "TheirPackage";
			PackageManager::PublishPackage();

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Publish Project: {recipe.Name}@{recipe.Version}",
					"DIAG: Load Recipe: Recipe.toml",
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"INFO: UpdateStart: 0",
					"INFO: UpdateProgress: 0.000000",
					"INFO: UpdateDone",
					"INFO: Request Authentication Token",
					"HIGH: UserName:",
					"HIGH: Password:",
					"DIAG: /.well-known/openid-configuration",
					"DIAG: /connect/token",
					"INFO: Publish package",
					"DIAG: /v1/packages/MyPackage/v1.2.3",
					"INFO: Package version created",
					"INFO: Cleanup staging directory",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetStandardInput",
					"ReadLine: ",
					"ReadPassword: ",
				}),
				consoleManager->GetRequests(),
				"Verify console manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Recipe.toml",
					"OpenReadBinary: Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"GetDirectoryChildren: ./",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/MyPackage.7z",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/MyPackage.7z",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: auth.soupbuild.com:443",
					"CreateClient: auth.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /.well-known/openid-configuration",
					"Post: /connect/token [application/x-www-form-urlencoded]",
				}),
				testAuthHttpClient->GetRequests(),
				"Verify http requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"SetAuthenticationToken: Bearer:SUPER_TOKEN",
					"Put: /v1/packages/MyPackage/v1.2.3 [application/x-7z-compressed]",
				}),
				testApiHttpClient->GetRequests(),
				"Verify http requests match expected.");
		}

		[[Fact]]
		void PublishPackage_PackageMissing_Success()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto consoleManager = std::make_shared<IO::MockConsoleManager>();
			auto scopedConsoleManager = IO::ScopedConsoleManagerRegister(consoleManager);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test listener
			auto testNetworkManager = std::make_shared<Network::MockNetworkManager>();
			auto scopedNetworkManager = Network::ScopedNetworkManagerRegister(testNetworkManager);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Version = "1.2.3"
				)")));

			// Create the required http client
			auto testAuthHttpClient = std::make_shared<Network::MockHttpClient>(
				"auth.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testAuthHttpClient);

			auto testApiHttpClient = std::make_shared<Network::MockHttpClient>(
				"api.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testApiHttpClient);

			// Setup the expected http requests
			auto openIdConfigResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({"issuer":"https://auth.soupbuild.com","jwks_uri":"https://auth.soupbuild.com/.well-known/openid-configuration/jwks","authorization_endpoint":"https://auth.soupbuild.com/connect/authorize","token_endpoint":"https://auth.soupbuild.com/connect/token","userinfo_endpoint":"https://auth.soupbuild.com/connect/userinfo","end_session_endpoint":"https://auth.soupbuild.com/connect/endsession","check_session_iframe":"https://auth.soupbuild.com/connect/checksession","revocation_endpoint":"https://auth.soupbuild.com/connect/revocation","introspection_endpoint":"https://auth.soupbuild.com/connect/introspect","device_authorization_endpoint":"https://auth.soupbuild.com/connect/deviceauthorization","frontchannel_logout_supported":true,"frontchannel_logout_session_supported":true,"backchannel_logout_supported":true,"backchannel_logout_session_supported":true,"scopes_supported":["openid","profile","soup_api","offline_access"],"claims_supported":["sub","name","family_name","given_name","middle_name","nickname","preferred_username","profile","picture","website","gender","birthdate","zoneinfo","locale","updated_at"],"grant_types_supported":["authorization_code","client_credentials","refresh_token","implicit","password","urn:ietf:params:oauth:grant-type:device_code"],"response_types_supported":["code","token","id_token","id_token token","code id_token","code token","code id_token token"],"response_modes_supported":["form_post","query","fragment"],"token_endpoint_auth_methods_supported":["client_secret_basic","client_secret_post"],"id_token_signing_alg_values_supported":["RS256"],"subject_types_supported":["public"],"code_challenge_methods_supported":["plain","S256"],"request_parameter_supported":true})");
			testAuthHttpClient->AddGetResponse("/.well-known/openid-configuration", openIdConfigResult);
			
			auto connectTokenResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({"access_token":"SUPER_TOKEN","expires_in":3600,"token_type":"Bearer","scope":"soup_api"})");
			testAuthHttpClient->AddPostResponse("/connect/token", connectTokenResult);

			auto publishPackageResultMissing = Network::HttpResponse(
				Network::HttpStatusCode::NotFound,
				"");
			testApiHttpClient->AddPutResponse("/v1/packages/MyPackage/v1.2.3", publishPackageResultMissing);
			
			auto createPackageResult = Network::HttpResponse(
				Network::HttpStatusCode::Created,
				R"({"name":"MyPackage"})");
			testApiHttpClient->AddPutResponse("/v1/packages/MyPackage", createPackageResult);

			auto publishPackageResultSuccess = Network::HttpResponse(
				Network::HttpStatusCode::Created,
				"");
			testApiHttpClient->AddPutResponse("/v1/packages/MyPackage/v1.2.3", publishPackageResultSuccess);

			auto packageName = "TheirPackage";
			PackageManager::PublishPackage();

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Publish Project: {recipe.Name}@{recipe.Version}",
					"DIAG: Load Recipe: Recipe.toml",
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"INFO: UpdateStart: 0",
					"INFO: UpdateProgress: 0.000000",
					"INFO: UpdateDone",
					"INFO: Request Authentication Token",
					"HIGH: UserName:",
					"HIGH: Password:",
					"DIAG: /.well-known/openid-configuration",
					"DIAG: /connect/token",
					"INFO: Publish package",
					"DIAG: /v1/packages/MyPackage/v1.2.3",
					"HIGH: The provided package name does not exist",
					"HIGH: Create package",
					"DIAG: /v1/packages/MyPackage",
					"HIGH: Retry publish package",
					"DIAG: /v1/packages/MyPackage/v1.2.3",
					"INFO: Package version created",
					"INFO: Cleanup staging directory",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetStandardInput",
					"ReadLine: ",
					"ReadPassword: ",
				}),
				consoleManager->GetRequests(),
				"Verify console manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Recipe.toml",
					"OpenReadBinary: Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"GetDirectoryChildren: ./",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/MyPackage.7z",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/MyPackage.7z",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: auth.soupbuild.com:443",
					"CreateClient: auth.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /.well-known/openid-configuration",
					"Post: /connect/token [application/x-www-form-urlencoded]",
				}),
				testAuthHttpClient->GetRequests(),
				"Verify http requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"SetAuthenticationToken: Bearer:SUPER_TOKEN",
					"Put: /v1/packages/MyPackage/v1.2.3 [application/x-7z-compressed]",
					"SetAuthenticationToken: Bearer:SUPER_TOKEN",
					"Put: /v1/packages/MyPackage [application/json]",
					"SetAuthenticationToken: Bearer:SUPER_TOKEN",
					"Put: /v1/packages/MyPackage/v1.2.3 [application/x-7z-compressed]",
				}),
				testApiHttpClient->GetRequests(),
				"Verify http requests match expected.");
		}
	};
}
