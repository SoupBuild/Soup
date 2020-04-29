// <copyright file="TLSClient.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

#include "BasicCredentialsManager.h"

namespace Opal::Network
{
	typedef SOCKET socket_type;

	inline socket_type invalid_socket() { return INVALID_SOCKET; }

	inline int close(int fd)
	{
		return ::closesocket(fd);
	}

	inline int send(int s, const uint8_t* buf, size_t len, int flags)
	{
		return ::send(s, reinterpret_cast<const char*>(buf), static_cast<int>(len), flags);
	}

	inline int read(int s, void* buf, size_t len)
	{
		return ::recv(s, reinterpret_cast<char*>(buf), static_cast<int>(len), 0);
	}

	class TLSClient final : public Botan::TLS::Callbacks
	{
	public:
		TLSClient()
		{
			// TODO init_sockets();
		}

		~TLSClient()
		{
			// TODO stop_sockets();
		}

		void DoIt()
		{
			// TODO client cert auth
			const std::string host = "www.soupbuild.com";
			const uint16_t port = 443;
			const std::string transport = "tcp";
			const std::string next_protos = "";
			const bool use_system_cert_store = true;
			const std::string trusted_CAs = "";

			auto session_mgr = std::make_unique<Botan::TLS::Session_Manager_In_Memory>(rng());

			auto policy = std::make_unique<Botan::TLS::Policy>();

			const bool use_tcp = (transport == "tcp");

			const std::vector<std::string> protocols_to_offer = Botan::split_on(next_protos, ',');

			Botan::TLS::Protocol_Version version = Botan::TLS::Protocol_Version::TLS_V12;

			if (policy->acceptable_protocol_version(version) == false)
			{
				throw std::runtime_error("The policy specified does not allow the requested TLS version");
			}

			struct sockaddr_storage addrbuf;
			std::string hostname;
			if(!host.empty() &&
				inet_pton(AF_INET, host.c_str(), &addrbuf) != 1 &&
				inet_pton(AF_INET6, host.c_str(), &addrbuf) != 1)
			{
				hostname = host;
			}

			m_sockfd = connect_to_host(host, port, use_tcp);

			BasicCredentialsManager creds(use_system_cert_store, trusted_CAs);

			Botan::TLS::Client client(
				*this,
				*session_mgr,
				creds,
				*policy,
				rng(),
				Botan::TLS::Server_Information(hostname, port),
				version,
				protocols_to_offer);

			bool first_active = true;

			while (!client.is_closed())
			{
				fd_set readfds;
				FD_ZERO(&readfds);
				FD_SET(m_sockfd, &readfds);

				if(client.is_active())
				{
					// FD_SET(STDIN_FILENO, &readfds);
					if (first_active && !protocols_to_offer.empty())
					{
						std::string app = client.application_protocol();
						if (app != "")
						{
							Log::HighPriority("Server choose protocol: " + client.application_protocol());
						}

						first_active = false;
					}
				}

				struct timeval timeout = { 1, 0 };

				::select(static_cast<int>(m_sockfd + 1), &readfds, nullptr, nullptr, &timeout);

				if (FD_ISSET(m_sockfd, &readfds))
				{
					uint8_t buf[4 * 1024] = { 0 };

					auto got = read(m_sockfd, buf, sizeof(buf));

					if (got == 0)
					{
						Log::HighPriority("EOF on socket");
						break;
					}
					else if (got == -1)
					{
						Log::HighPriority("Socket error: " + std::to_string(errno));
						continue;
					}

					client.received_data(buf, got);
				}

				// if (FD_ISSET(STDIN_FILENO, &readfds))
				// {
				//	 uint8_t buf[1024] = { 0 };
				//	 ssize_t got = read(STDIN_FILENO, buf, sizeof(buf));

				//	 if (got == 0)
				//	 {
				//		 Log::HighPriority("EOF on stdin");
				//		 client.close();
				//		 break;
				//	 }
				//	 else if (got == -1)
				//	 {
				//		 Log::HighPriority("Stdin error: " + errno + " " + err_to_string(errno));
				//		 continue;
				//	 }

				//	 if(got == 2 && buf[1] == '\n')
				//	 {
				//		 char cmd = buf[0];

				//		 if(cmd == 'R' || cmd == 'r')
				//		 {
				//			 Log::HighPriority("Client initiated renegotiation");
				//			 client.renegotiate(cmd == 'R');
				//		 }
				//		 else if(cmd == 'Q')
				//		 {
				//			 Log::HighPriority("Client initiated close");
				//			 client.close();
				//		 }
				//	 }
				//	 else
				//	 {
				//		 client.send(buf, got);
				//	 }
				// }

				if (client.timeout_check())
				{
					Log::HighPriority("Timeout detected");
				}
			}

			close(m_sockfd);
		 }

	private:
		Botan::RandomNumberGenerator& rng()
		{
			if (m_rng == nullptr)
			{
				m_rng = std::make_unique<Botan::System_RNG>();
			}

			return *m_rng.get();
		}

		socket_type connect_to_host(const std::string& host, uint16_t port, bool tcp)
		{
			addrinfo hints;
			Botan::clear_mem(&hints, 1);
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = tcp ? SOCK_STREAM : SOCK_DGRAM;
			addrinfo* res, *rp = nullptr;

			if (::getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res) != 0)
			{
				throw std::runtime_error("getaddrinfo failed for " + host);
			}

			socket_type fd = 0;

			for (rp = res; rp != nullptr; rp = rp->ai_next)
			{
				fd = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

				if (fd == invalid_socket())
				{
					continue;
				}

				if (::connect(fd, rp->ai_addr, static_cast<socklen_t>(rp->ai_addrlen)) != 0)
				{
					close(fd);
					continue;
				}

				break;
			}

			::freeaddrinfo(res);

			if (rp == nullptr) // no address succeeded
			{
				throw std::runtime_error("connect failed");
			}

			return fd;
		}

		void tls_verify_cert_chain(
			const std::vector<Botan::X509_Certificate>& cert_chain,
			const std::vector<std::shared_ptr<const Botan::OCSP::Response>>& ocsp,
			const std::vector<Botan::Certificate_Store*>& trusted_roots,
			Botan::Usage_Type usage,
			const std::string& hostname,
			const Botan::TLS::Policy& policy) override
		{
			if (cert_chain.empty())
			{
				throw Botan::Invalid_Argument("Certificate chain was empty");
			}

			Botan::Path_Validation_Restrictions restrictions(
				false, // TODO: policy.require_cert_revocation_info(),
				policy.minimum_signature_strength());

			auto ocsp_timeout = std::chrono::milliseconds(1000);

			Botan::Path_Validation_Result result = Botan::x509_path_validate(
				cert_chain,
				restrictions,
				trusted_roots,
				hostname,
				usage,
				std::chrono::system_clock::now(),
				ocsp_timeout,
				ocsp);

			Log::HighPriority("Certificate validation status: " + result.result_string());
			if (result.successful_validation())
			{
				auto status = result.all_statuses();

				if (status.size() > 0 && status[0].count(Botan::Certificate_Status_Code::OCSP_RESPONSE_GOOD))
				{
					Log::HighPriority("Valid OCSP response for this server");
				}
			}
		}

		bool tls_session_established(const Botan::TLS::Session& session) override
		{
			Log::HighPriority("Handshake complete, " + session.version().to_string()
				+ " using " + session.ciphersuite().to_string());

		//	 if (!session.session_id().empty())
		//	 {
		//		 output() << "Session ID " << Botan::hex_encode(session.session_id()) << "\n";
		//	 }

		//	 if (!session.session_ticket().empty())
		//	 {
		//		 output() << "Session ticket " << Botan::hex_encode(session.session_ticket()) << "\n";
		//	 }

		//	 if (flag_set("print-certs"))
		//	 {
		//		 const std::vector<Botan::X509_Certificate>& certs = session.peer_certs();

		//		 for (size_t i = 0; i != certs.size(); ++i)
		//		 {
		//			 output() << "Certificate " << i + 1 << "/" << certs.size() << "\n";
		//			 output() << certs[i].to_string();
		//			 output() << certs[i].PEM_encode();
		//		 }
		//	 }

			return true;
		}

		// static void dgram_socket_write(int sockfd, const uint8_t buf[], size_t length)
		// {
		//	 int r = ::send(sockfd, buf, length, MSG_NOSIGNAL);

		//	 if (r == -1)
		//	 {
		//		 throw CLI_Error("Socket write failed errno=" + std::to_string(errno));
		//	 }
		// }

		void tls_emit_data(const uint8_t buf[], size_t length) override
		{
			size_t offset = 0;

			while (length)
			{
				ssize_t sent = send(m_sockfd, buf + offset, length, 0);

				if (sent == -1)
				{
					if (errno == EINTR)
					{
						sent = 0;
					}
					else
					{
						throw std::runtime_error("Socket write failed errno=" + std::to_string(errno));
					}
				}

				offset += sent;
				length -= sent;
			}
		}

		void tls_alert(Botan::TLS::Alert alert) override
		{
			Log::Error("Alert: " + alert.type_string());
		}

		void tls_record_received(uint64_t /*seq_no*/, const uint8_t buf[], size_t buf_size) override
		{
			for (size_t i = 0; i != buf_size; ++i)
			{
				// output() << buf[i];
			}
		 }

	private:
		std::unique_ptr<Botan::RandomNumberGenerator> m_rng;
		socket_type m_sockfd = invalid_socket();
	};
}
