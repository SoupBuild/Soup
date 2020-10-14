// <copyright file="OpenIdConfigurationModel.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Api
{
    /// <summary>
    /// A class representing the OpenId Configuration
    /// </summary>
    export class OpenIdConfigurationModel
    {
    public:
        OpenIdConfigurationModel()
        {
        }

        OpenIdConfigurationModel(
            std::string issuer,
            std::string jwksUri,
            std::string authorizationEndpoint,
            std::string tokenEndpoint,
            std::string userinfoEndpoint,
            std::string sessionEndpoint,
            std::string checkSessionIframe,
            std::string revocationEndpoint,
            std::string introspectionEndpoint,
            std::string deviceAuthorizationEndpoint,
            bool frontchannelLogoutSupported,
            bool frontchannelLogoutSessionSupported,
            bool backchannelLogoutSupported,
            bool backchannelLogoutSessionSupported,
            std::vector<std::string> scopesSupported,
            std::vector<std::string> claimsSupported,
            std::vector<std::string> grantTypesSupported,
            std::vector<std::string> responseTypesSupported,
            std::vector<std::string> responseModesSupported,
            std::vector<std::string> tokenEndpointAuthmethodsSupported,
            std::vector<std::string> idTokenSigningAlgValuesSupported,
            std::vector<std::string> subjectTypesSupported,
            std::vector<std::string> codeChallengeMethodsSupported,
            bool requestParameterSupported) :
            Issuer(std::move(issuer)),
            JwksUri(std::move(jwksUri)),
            AuthorizationEndpoint(std::move(authorizationEndpoint)),
            TokenEndpoint(std::move(tokenEndpoint)),
            UserinfoEndpoint(std::move(userinfoEndpoint)),
            SessionEndpoint(std::move(sessionEndpoint)),
            CheckSessionIframe(std::move(checkSessionIframe)),
            RevocationEndpoint(std::move(revocationEndpoint)),
            IntrospectionEndpoint(std::move(introspectionEndpoint)),
            DeviceAuthorizationEndpoint(std::move(deviceAuthorizationEndpoint)),
            FrontchannelLogoutSupported(frontchannelLogoutSupported),
            FrontchannelLogoutSessionSupported(frontchannelLogoutSessionSupported),
            BackchannelLogoutSupported(backchannelLogoutSupported),
            BackchannelLogoutSessionSupported(backchannelLogoutSessionSupported),
            ScopesSupported(std::move(scopesSupported)),
            ClaimsSupported(std::move(claimsSupported)),
            GrantTypesSupported(std::move(grantTypesSupported)),
            ResponseTypesSupported(std::move(responseTypesSupported)),
            ResponseModesSupported(std::move(responseModesSupported)),
            TokenEndpointAuthmethodsSupported(std::move(tokenEndpointAuthmethodsSupported)),
            IdTokenSigningAlgValuesSupported(std::move(idTokenSigningAlgValuesSupported)),
            SubjectTypesSupported(std::move(subjectTypesSupported)),
            CodeChallengeMethodsSupported(std::move(codeChallengeMethodsSupported)),
            RequestParameterSupported(requestParameterSupported)
        {
        }

        std::string Issuer;
        std::string JwksUri;
        std::string AuthorizationEndpoint;
        std::string TokenEndpoint;
        std::string UserinfoEndpoint;
        std::string SessionEndpoint;
        std::string CheckSessionIframe;
        std::string RevocationEndpoint;
        std::string IntrospectionEndpoint;
        std::string DeviceAuthorizationEndpoint;
        bool FrontchannelLogoutSupported;
        bool FrontchannelLogoutSessionSupported;
        bool BackchannelLogoutSupported;
        bool BackchannelLogoutSessionSupported;
        std::vector<std::string> ScopesSupported;
        std::vector<std::string> ClaimsSupported;
        std::vector<std::string> GrantTypesSupported;
        std::vector<std::string> ResponseTypesSupported;
        std::vector<std::string> ResponseModesSupported;
        std::vector<std::string> TokenEndpointAuthmethodsSupported;
        std::vector<std::string> IdTokenSigningAlgValuesSupported;
        std::vector<std::string> SubjectTypesSupported;
        std::vector<std::string> CodeChallengeMethodsSupported;
        bool RequestParameterSupported;
    };
}
