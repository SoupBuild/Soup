
using System.Runtime.InteropServices;

namespace Soup.View;

public static class SoupTools
{
	[DllImport("SoupTools", CallingConvention = CallingConvention.Cdecl)]
	public static extern string LoadBuildGraph(string workingDirectory);

	[DllImport("SoupTools", CallingConvention = CallingConvention.Cdecl)]
	public static extern int AddStuff(int a, int b);
}