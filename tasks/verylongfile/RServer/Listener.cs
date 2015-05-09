using System;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using log4net;

namespace RServer
{
	public class Listener
	{
		private HttpListener listener;

		public Listener(int port, string suffix, int numberOfThreads, Func<HttpListenerContext, Task> callbackAsync)
		{
			ThreadPool.SetMinThreads(numberOfThreads, numberOfThreads);
			CallbackAsync = callbackAsync;
			listener = new HttpListener();
			listener.Prefixes.Add(string.Format("http://+:{0}{1}/", port, suffix != null ? "/" + suffix.TrimStart('/') : ""));
		}

		public void Start()
		{
			listener.Start();
			StartListen();
		}

		public async void StartListen()
		{
			while (true)
			{
				try
				{
					var context = await listener.GetContextAsync();

					Task.Run(
						async () =>
						{
							var ctx = context;
							try
							{
								await CallbackAsync(ctx);
							}
							catch (Exception e)
							{
								log.Error(e);
							}
							finally
							{
								ctx.Response.Close();
							}
						}
					);
				}
				catch (Exception e)
				{
					log.Error(e);
				}
			}
		}

		private Func<HttpListenerContext, Task> CallbackAsync { get; set; }

		private static readonly ILog log = LogManager.GetLogger(typeof(Program));
	}
}