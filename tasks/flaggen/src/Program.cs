using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace flaggen
{
	internal static class Program
	{
		private static void Main()
		{
			var sb = new StringBuilder("QCTF");
			Enumerable.Range(20, 4).Select(Fib).ForEach(i => sb.Append(i.ToString("X")));
			Console.WriteLine(sb);
		}

		private static int Fib(int n)
		{
			if(n <= 1) return n;
			return Fib(n - 1) + Fib(n - 2);
		}
	}

	internal static class EnumerableHelper
	{
		public static void ForEach<T>(this IEnumerable<T> enumerable, Action<T> action)
		{
			foreach(var item in enumerable)
				action(item);
		}
	}
}