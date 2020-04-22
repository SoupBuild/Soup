// <copyright file="MockHttpClient.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IHttpClient.h"

namespace Opal::Network
{
	/// <summary>
	/// A mock Http Client
	/// </summary>
	export class MockHttpClient : public IHttpClient
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockHttpClient'/> class.
		/// </summary>
		MockHttpClient(std::string host, int port) :
			_host(std::move(host)),
			_port(port),
			_requests(),
			_getResponses(),
			_postResponses(),
			_putResponses()
		{
		}

		/// <summary>
		/// Get the requests
		/// </summary>
		const std::vector<std::string>& GetRequests() const
		{
			return _requests;
		}

		/// <summary>
		/// Get the host
		/// </summary>
		const std::string& GetHost() const
		{
			return _host;
		}

		/// <summary>
		/// Get the port
		/// </summary>
		int GetPort() const
		{
			return _port;
		}

		/// <summary>
		/// Set the response for given request
		/// </summary>
		void AddGetResponse(std::string url, HttpResponse response)
		{
			auto findExistingQueue = _getResponses.find(url);
			if (findExistingQueue != _getResponses.end())
			{
				findExistingQueue->second.push(std::move(response));
			}
			else
			{
				auto insertResult = _getResponses.emplace(url, std::queue<HttpResponse>());
				insertResult.first->second.push(std::move(response));
			}
		}

		/// <summary>
		/// Set the response for given request
		/// </summary>
		void AddPostResponse(std::string url, HttpResponse response)
		{
			auto findExistingQueue = _postResponses.find(url);
			if (findExistingQueue != _postResponses.end())
			{
				findExistingQueue->second.push(std::move(response));
			}
			else
			{
				auto insertResult = _postResponses.emplace(url, std::queue<HttpResponse>());
				insertResult.first->second.push(std::move(response));
			}
		}

		/// <summary>
		/// Set the response for given request
		/// </summary>
		void AddPutResponse(std::string url, HttpResponse response)
		{
			auto findExistingQueue = _putResponses.find(url);
			if (findExistingQueue != _putResponses.end())
			{
				findExistingQueue->second.push(std::move(response));
			}
			else
			{
				auto insertResult = _putResponses.emplace(url, std::queue<HttpResponse>());
				insertResult.first->second.push(std::move(response));
			}
		}

		/// <summary>
		/// Set the authentication token
		/// </summary>
		void SetAuthenticationToken(std::string_view scheme, std::string_view token) override final
		{
			auto message = std::stringstream();
			message << "SetAuthenticationToken: " << scheme << ":" << token;
			_requests.push_back(message.str());
		}

		/// <summary>
		/// Perform an Http Get request
		/// </summary>
		HttpResponse Get(std::string_view request) override final
		{
			auto message = std::stringstream();
			message << "Get: " << request;
			_requests.push_back(message.str());

			auto searchClient = _getResponses.find(std::string(request));
			if (searchClient != _getResponses.end())
			{
				auto& responseQueue = searchClient->second;
				if (responseQueue.empty())
				{
					throw std::runtime_error("Ran out of Get responses.");
				}
				else
				{
					auto result = std::move(responseQueue.front());
					responseQueue.pop();
					return result;
				}
			}
			else
			{
				// The response was not set, return not found
				return HttpResponse(HttpStatusCode::NotFound);
			}
		}

		/// <summary>
		/// Perform an Http Post request
		/// </summary>
		HttpResponse Post(
			std::string_view request,
			std::string_view contentType,
			std::istream& content) override final
		{
			auto message = std::stringstream();
			message << "Post: " << request;
			message << " [" << contentType << "]";
			_requests.push_back(message.str());

			auto searchClient = _postResponses.find(std::string(request));
			if (searchClient != _postResponses.end())
			{
				auto& responseQueue = searchClient->second;
				if (responseQueue.empty())
				{
					throw std::runtime_error("Ran out of Post responses.");
				}
				else
				{
					auto result = std::move(responseQueue.front());
					responseQueue.pop();
					return result;
				}
			}
			else
			{
				// The response was not set, return not found
				return HttpResponse(HttpStatusCode::NotFound);
			}
		}

		/// <summary>
		/// Perform an Http Put request
		/// </summary>
		HttpResponse Put(
			std::string_view request,
			std::string_view contentType,
			std::istream& content) override final
		{
			auto message = std::stringstream();
			message << "Put: " << request;
			message << " [" << contentType << "]";
			_requests.push_back(message.str());

			auto searchClient = _putResponses.find(std::string(request));
			if (searchClient != _putResponses.end())
			{
				auto& responseQueue = searchClient->second;
				if (responseQueue.empty())
				{
					throw std::runtime_error("Ran out of Put responses.");
				}
				else
				{
					auto result = std::move(responseQueue.front());
					responseQueue.pop();
					return result;
				}
			}
			else
			{
				// The response was not set, return not found
				return HttpResponse(HttpStatusCode::NotFound);
			}
		}

	private:
		std::string _host;
		int _port;
		std::vector<std::string> _requests;
		std::map<std::string, std::queue<HttpResponse>> _getResponses;
		std::map<std::string, std::queue<HttpResponse>> _postResponses;
		std::map<std::string, std::queue<HttpResponse>> _putResponses;
	};
}
