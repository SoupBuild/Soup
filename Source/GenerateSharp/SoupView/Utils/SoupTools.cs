
using System.Runtime.InteropServices;

namespace Soup.View;

public static class SoupTools
{
	[DllImport("SoupTools", CallingConvention = CallingConvention.Cdecl)]
	public static extern string GetName();

	[DllImport("SoupTools", CallingConvention = CallingConvention.Cdecl)]
	public static extern int AddStuff(int a, int b);
}