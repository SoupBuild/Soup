// <copyright file="WebPageBuilder.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

using System.Globalization;
using System.Text;

namespace Soup.Build.PackageManager;

/// <summary>
/// The webpage builder
/// </summary>
public static class WebPageBuilder
{
	private static readonly CompositeFormat PageFormat = CompositeFormat.Parse(
		"""
		<!DOCTYPE html>
		<html lang="en" class="h-100">
		<head>
			<meta charset="utf-8" />
			<meta name="viewport" content="width=device-width, initial-scale=1">
			<link rel="icon" type="image/x-icon" href="/Shared/favicon.ico">
			<title>{0}</title>
			<link href="https://auth.soupbuild.com/shared/css/app.min.css" rel="stylesheet" />
		</head>
		<body class="d-flex flex-column min-vh-100">
			<header class="sticky-top bg-body">
				<nav class="navbar navbar-expand-lg container">
					<div class="container-fluid">
						<a class="navbar-brand" href="https://www.soupbuild.com">Soup Build</a>
						<button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
							<span class="navbar-toggler-icon"></span>
						</button>
						<div class="collapse navbar-collapse" id="navbarSupportedContent">
						</div>
					</div>
				</nav>
			</header>
			<main role="main" class="pb-3 container">
				{1}
			</main>
			<footer class="mt-auto">
			<div class="container">
				&copy; 2020-2023 - Soup Build
				<ul class="nav">
					<li class="nav-item"><a class="nav-link ps-0" href="https://github.com/SoupBuild/Soup" target="_blank" rel="noopener noreferrer">GitHub</a></li>
				</ul>
			</div>
		</footer>
			<script src="https://auth.soupbuild.com/Shared/js/jquery.slim.min.js"></script>
			<script src="https://auth.soupbuild.com/Shared/js/bootstrap.bundle.min.js"></script>
		</body>
		</html>
		""");

	public static string BuildWebPage(string title, string content)
	{
		var result = string.Format(CultureInfo.InvariantCulture, PageFormat, title, content);

		return result;
	}
}