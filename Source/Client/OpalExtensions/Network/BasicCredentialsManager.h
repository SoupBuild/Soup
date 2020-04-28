// <copyright file="HttpLibClient.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IHttpClient.h"

namespace Opal::Network
{
	/// <summary>
	/// The Http Lib Client
	/// </summary>
	class BasicCredentialsManager : public Botan::Credentials_Manager
	{
	public:
		BasicCredentialsManager(
			bool use_system_store,
			const std::string& ca_path)
		{
			if (!ca_path.empty())
			{
				m_certstores.push_back(std::make_shared<Botan::Certificate_Store_In_Memory>(ca_path));
			}
			
			if (use_system_store)
			{
				m_certstores.push_back(std::make_shared<Botan::System_Certificate_Store>());
			}
		}

		std::vector<Botan::Certificate_Store*> trusted_certificate_authorities(
			const std::string& type,
			const std::string& /*hostname*/) override
		{
			std::vector<Botan::Certificate_Store*> v;

			// don't ask for client certs
			if (type == "tls-server")
			{
				return v;
			}

			for (auto const& cs : m_certstores)
			{
				v.push_back(cs.get());
			}

			return v;
		}

		std::vector<Botan::X509_Certificate> cert_chain(
			const std::vector<std::string>& algos,
			const std::string& type,
			const std::string& hostname) override
		{
			for (auto const& i : m_creds)
			{
				if (std::find(algos.begin(), algos.end(), i.key->algo_name()) == algos.end())
				{
					continue;
				}

				if (hostname != "" && !i.certs[0].matches_dns_name(hostname))
				{
					continue;
				}

				return i.certs;
			}

			return std::vector<Botan::X509_Certificate>();
		}

		Botan::Private_Key* private_key_for(
			const Botan::X509_Certificate& cert,
			const std::string& /*type*/,
			const std::string& /*context*/) override
		{
			for (auto const& i : m_creds)
			{
				if (cert == i.certs[0])
				{
					return i.key.get();
				}
			}

			return nullptr;
		}

	private:
		struct Certificate_Info
		{
			std::vector<Botan::X509_Certificate> certs;
			std::shared_ptr<Botan::Private_Key> key;
		};

		std::vector<Certificate_Info> m_creds;
		std::vector<std::shared_ptr<Botan::Certificate_Store>> m_certstores;
	};
}
