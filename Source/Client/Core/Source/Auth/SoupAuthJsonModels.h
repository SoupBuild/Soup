// <copyright file="SoupAuthJsonModels.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Models/OpenIdConfigurationModel.h"
#include "Models/ClientCredentialsTokenModel.h"

namespace Soup::Api
{
	/// <summary>
	/// The soup auth models json serialize manager
	/// </summary>
	export class SoupAuthJsonModels
	{
	private:
		static constexpr const char* Property_Issuer = "issuer";
		static constexpr const char* Property_Jwks_Uri = "jwks_uri";
		static constexpr const char* Property_Authorization_Endpoint = "authorization_endpoint";
		static constexpr const char* Property_Token_Endpoint = "token_endpoint";
		static constexpr const char* Property_Userinfo_Endpoint = "userinfo_endpoint";
		static constexpr const char* Property_End_Session_Endpoint = "end_session_endpoint";
		static constexpr const char* Property_Check_Session_Iframe = "check_session_iframe";
		static constexpr const char* Property_Revocation_Endpoint = "revocation_endpoint";
		static constexpr const char* Property_Introspection_Endpoint = "introspection_endpoint";
		static constexpr const char* Property_Device_Authorization_Endpoint = "device_authorization_endpoint";
		static constexpr const char* Property_Frontchannel_Logout_Supported = "frontchannel_logout_supported";
		static constexpr const char* Property_Frontchannel_Logout_Session_Supported = "frontchannel_logout_session_supported";
		static constexpr const char* Property_Backchannel_Logout_Supported = "backchannel_logout_supported";
		static constexpr const char* Property_Backchannel_Logout_Session_Supported = "backchannel_logout_session_supported";
		static constexpr const char* Property_Scopes_Supported = "scopes_supported";
		static constexpr const char* Property_Claims_Supported = "claims_supported";
		static constexpr const char* Property_Grant_Types_Supported = "grant_types_supported";
		static constexpr const char* Property_Response_Types_Supported = "response_types_supported";
		static constexpr const char* Property_Response_Modes_Supported = "response_modes_supported";
		static constexpr const char* Property_Token_Endpoint_Auth_Methods_Supported = "token_endpoint_auth_methods_supported";
		static constexpr const char* Property_Id_Token_Signing_Alg_Values_Supported = "id_token_signing_alg_values_supported";
		static constexpr const char* Property_Subject_Types_Supported = "subject_types_supported";
		static constexpr const char* Property_Code_Challenge_Methods_Supported = "code_challenge_methods_supported";
		static constexpr const char* Property_Request_Parameter_Supported = "request_parameter_supported";
		static constexpr const char* Property_Access_Token = "access_token";
		static constexpr const char* Property_Expires_In = "expires_in";
		static constexpr const char* Property_Token_Type = "token_type";
		static constexpr const char* Property_Scope = "scope";

	public:
		static OpenIdConfigurationModel ParseOpenIdConfigurationResult(const std::string& content)
		{
			// Read the contents of the content
			std::string error = "";
			auto jsonRoot = json11::Json::parse(content, error);
			if (jsonRoot.is_null())
			{
				auto message = "Failed to parse the package result json: " + error;
				throw std::runtime_error(std::move(message));
			}
			else
			{
				if (!jsonRoot.is_object())
					throw std::runtime_error("Root json was not an object.");

				return ParseOpenIdConfigurationResult(jsonRoot);
			}
		}

		static ClientCredentialsTokenModel ParseClientCredentialsTokenModel(const std::string& content)
		{
			// Read the contents of the content
			std::string error = "";
			auto jsonRoot = json11::Json::parse(content, error);
			if (jsonRoot.is_null())
			{
				auto message = "Failed to parse the package result json: " + error;
				throw std::runtime_error(std::move(message));
			}
			else
			{
				if (!jsonRoot.is_object())
					throw std::runtime_error("Root json was not an object.");

				return ParseClientCredentialsTokenModel(jsonRoot);
			}
		}

	private:
		static OpenIdConfigurationModel ParseOpenIdConfigurationResult(const json11::Json& value)
		{
			auto issuer = value[Property_Issuer].string_value();
			auto jwksUri = value[Property_Jwks_Uri].string_value();
			auto authorizationEndpoint = value[Property_Authorization_Endpoint].string_value();
			auto tokenEndpoint = value[Property_Token_Endpoint].string_value();
			auto userinfoEndpoint = value[Property_Userinfo_Endpoint].string_value();
			auto sessionEndpoint = value[Property_End_Session_Endpoint].string_value();
			auto checkSessionIframe = value[Property_Check_Session_Iframe].string_value();
			auto revocationEndpoint = value[Property_Revocation_Endpoint].string_value();
			auto introspectionEndpoint = value[Property_Introspection_Endpoint].string_value();
			auto deviceAuthorizationEndpoint = value[Property_Device_Authorization_Endpoint].string_value();
			auto frontchannelLogoutSupported = value[Property_Frontchannel_Logout_Supported].bool_value();
			auto frontchannelLogoutSessionSupported = value[Property_Frontchannel_Logout_Session_Supported].bool_value();
			auto backchannelLogoutSupported = value[Property_Backchannel_Logout_Supported].bool_value();
			auto backchannelLogoutSessionSupported = value[Property_Backchannel_Logout_Session_Supported].bool_value();
			auto scopesSupported = ParseStringArray(value[Property_Scopes_Supported].array_items());
			auto claimsSupported = ParseStringArray(value[Property_Claims_Supported].array_items());
			auto grantTypesSupported = ParseStringArray(value[Property_Grant_Types_Supported].array_items());
			auto responseTypesSupported = ParseStringArray(value[Property_Response_Types_Supported].array_items());
			auto responseModesSupported = ParseStringArray(value[Property_Response_Modes_Supported].array_items());
			auto tokenEndpointAuthMethodsSupported = ParseStringArray(value[Property_Token_Endpoint_Auth_Methods_Supported].array_items());
			auto idTokenSigningAlgValuesSupported = ParseStringArray(value[Property_Id_Token_Signing_Alg_Values_Supported].array_items());
			auto subjectTypesSupported = ParseStringArray(value[Property_Subject_Types_Supported].array_items());
			auto codeChallengeMethodsSupported = ParseStringArray(value[Property_Code_Challenge_Methods_Supported].array_items());
			auto requestParameterSupported = value[Property_Request_Parameter_Supported].bool_value();

			return OpenIdConfigurationModel(
				std::move(issuer),
				std::move(jwksUri),
				std::move(authorizationEndpoint),
				std::move(tokenEndpoint),
				std::move(userinfoEndpoint),
				std::move(sessionEndpoint),
				std::move(checkSessionIframe),
				std::move(revocationEndpoint),
				std::move(introspectionEndpoint),
				std::move(deviceAuthorizationEndpoint),
				std::move(frontchannelLogoutSupported),
				std::move(frontchannelLogoutSessionSupported),
				std::move(backchannelLogoutSupported),
				std::move(backchannelLogoutSessionSupported),
				std::move(scopesSupported),
				std::move(claimsSupported),
				std::move(grantTypesSupported),
				std::move(responseTypesSupported),
				std::move(responseModesSupported),
				std::move(tokenEndpointAuthMethodsSupported),
				std::move(idTokenSigningAlgValuesSupported),
				std::move(subjectTypesSupported),
				std::move(codeChallengeMethodsSupported),
				std::move(requestParameterSupported));
		}

		static ClientCredentialsTokenModel ParseClientCredentialsTokenModel(const json11::Json& value)
		{
			auto accessToken = value[Property_Access_Token].string_value();
			auto expiresIn = value[Property_Expires_In].number_value();
			auto tokenType = value[Property_Token_Type].string_value();
			auto scope = value[Property_Scope].string_value();

			return ClientCredentialsTokenModel(
				std::move(accessToken),
				expiresIn,
				std::move(tokenType),
				std::move(scope));
		}

	private:
		static std::vector<std::string> ParseStringArray(const json11::Json::array& items)
		{
			std::vector<std::string> result;
			result.reserve(items.size());
			for (size_t i = 0; i < items.size(); i++)
			{
				result.push_back(items[i].string_value());
			}

			return result;
		}
	};
}
