using System;
// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	public static class Singleton<T>
	{
		public static T Instance { get; set; }
	}
}
