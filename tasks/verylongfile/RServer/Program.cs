using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http.Headers;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Born2Code.Net;
using log4net;
using log4net.Config;

namespace RServer
{
	public class Program
	{
		private const int DownloadSpeedBytesPerSecond = 100 * 1024 / 8; // 100Kb/sec
		private static ILog log;

		private static void Main(string[] args)
		{
			XmlConfigurator.ConfigureAndWatch(new FileInfo(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log.config.xml")));
			log = LogManager.GetLogger(typeof (Program));
			var port = args.Length > 0 ? int.Parse(args[0]) : 1337;
			var numberOfThreads = args.Length > 1 ? int.Parse(args[1]) : 8;

			InitHintsOffsets();

			try
			{
				var listener = new Listener(port, null, numberOfThreads, OnContextAsync);
				listener.Start();

				log.InfoFormat("Server started listening on port {0}", port);

				while (true)
				{
					Thread.Sleep(Timeout.Infinite);
				}
			}
			catch (Exception e)
			{
				log.Fatal(e);
				throw;
			}
		}

		private static async Task OnContextAsync(HttpListenerContext context)
		{
			var requestId = Guid.NewGuid();
			var query = context.Request.QueryString["query"];
			var remoteEndPoint = context.Request.RemoteEndPoint;
			log.DebugFormat("{0}: received {1} from {2}", requestId, query, remoteEndPoint);
			context.Request.InputStream.Close();

			//var throttledStream = new ThrottledStream(context.Response.OutputStream, DownloadSpeedBytesPerSecond);
			var range = GetRequestedRange(context.Request, requestId);
			await GenerateFileContent(context.Response.OutputStream, range.Item1, range.Item2, requestId);			
		}

		private static Tuple<long?, long?> GetRequestedRange(HttpListenerRequest request, Guid requestId)
		{
			try
			{
				log.Debug(requestId + " " + string.Join(";", request.Headers.AllKeys));
				if (!request.Headers.AllKeys.Contains(HttpRequestHeader.Range.ToString()))
					return new Tuple<long?, long?>(null, null);
				var rangeHeader = request.Headers[HttpRequestHeader.Range.ToString()];


				var res = RangeHeaderValue.Parse(rangeHeader);

				//todo: поддерживаем только один диапазон?
				return new Tuple<long?, long?>(res.Ranges.First().From, res.Ranges.First().To);
			}
			catch (Exception e)
			{
				log.Error(e);
				return new Tuple<long?, long?>(null,null);
			}
		}

		private static async Task GenerateFileContent(Stream stream, long? from, long? to, Guid requestId)
		{
			try
			{

				long startOffset;
				long endOffset;
				if (from == null)
					if (to == null)
						startOffset = 0;
					else
						startOffset = TotalFileLength - to ?? 0;
				else
					startOffset = from ?? 0;

				if (from == null)
					endOffset = TotalFileLength;
				else
					endOffset = to ?? TotalFileLength;

				log.Debug(requestId + " " + string.Format("Requested range: {0} - {1}", startOffset, endOffset));

				for (var i = startOffset; i <= endOffset; i++)
				{
					var onebyte = GetByteByIndex(i);
					if (i % 100000 == 0)
						log.Debug(string.Format("{0} {1} bytes sent", requestId, i));
					await stream.WriteAsync(new[] {onebyte}, 0, 1);
				}

				stream.Close();
			}
			catch (Exception e)
			{
				log.Warn(string.Format("{0} The client probably canceled request: {1}", requestId, e));
			}
		}

		private static byte GetByteByIndex(long i)
		{
			int hintIndex;
			if (AnyHintContentOnByte(i, out hintIndex))
			{
				var bytes = Encoding.UTF8.GetBytes(Hints[hintIndex]);
				return bytes[i - HintOffsets[hintIndex]];
			}
			return HashOf(i);
		}

		private static bool AnyHintContentOnByte(long byteNumber, out int hintIndex)
		{
			hintIndex = -1;
			for (var i = 0; i < Hints.Count; i++)
			{
				if (HintOffsets[i] <= byteNumber && byteNumber < HintOffsets[i] + Encoding.UTF8.GetBytes(Hints[i]).Length)
				{
					hintIndex = i;
					return true;
				}
			}
			return false;
		}

		private static byte HashOf(long byteNum)
		{
			var bytes = BitConverter.GetBytes(byteNum);
			byte oneByte = 0; 
			for (var i=0; i<bytes.Length; i+=3)
			{
				//if (byteNum % i == 0)
					oneByte ^= bytes[i];
			}
			oneByte ^= Stub[byteNum%Stub.Length];
			return oneByte;
		}

		private static void InitHintsOffsets()
		{
			// Нужно растянуть подсказки так, чтобы за час, или даже четыре, их было нереально докачать. 
			// Соответственно, последняя подсказка должна быть дальше, чем:
			// DownloadSpeedBytesPerSecond * 3600 * 3 (bytes)
			// В соответствии с задумкой располагать данные через увеличивающиеся промежутки трэша, то есть: +-+--+----+--------+...
			// получаем, что нужно найти степень двойки >= чем наш последний оффсет:

			const long minimumLastOffset = DownloadSpeedBytesPerSecond*3600 * 5; 
			log.Debug(string.Format("minimumLastOffset: {0}", minimumLastOffset));

			var numberOfOffsets = (int) Math.Ceiling(Math.Log(minimumLastOffset) / Math.Log(2));
			var spacing = Encoding.UTF8.GetBytes(FirstHints[0]).Length;
			var rand = new Random();

			HintOffsets.Add(0);

			for (var i = 0; i < numberOfOffsets - 1; i++)
			{
				var nextOffset = (long) (spacing * 2 * (Math.Pow(2, i) + 1));
				HintOffsets.Add(nextOffset);
				var nextHint = i < 4 ? FirstHints[i] : HintsPool[rand.Next(HintsPool.Length)];
				
				Hints.Add(string.Format(nextHint, nextOffset));
				log.Info(i + ": " +  Hints[i]);
			}

			Hints.Add(Flag);

			TotalFileLength = HintOffsets[numberOfOffsets - 1] + Encoding.UTF8.GetBytes(Hints[numberOfOffsets - 1]).Length;
		}

		private static List<long> HintOffsets = new List<long>();
		private static List<string> Hints = new List<string>();

		private static string[] FirstHints = new []
		{
			"Would you like to play a game? If so - look at {0} offset and we'll start!!!",
			"I knew you would! Otherwise you were not here. Jump to {0}!!!",
			"Ok, let's warm up! Jump to {0}!!!",
			"Hope you're young enough for this game: you'll jump a lot! See {0}!!!",
		};

		private static string Flag = "Congrats! Here is your flag: QCTF_THISWASEASYIFYOUKNOWABOUTHTTPRANGEHEADER!!!";

		private static byte[] Stub = Encoding.UTF8.GetBytes("Россия и Франция, в ходе переговоров лидеров двух стран Владимира Путина и Франсуа Олланда, оказались как никогда близки к достижению договорённости о расторжении дорогостоящего контракта на поставку ВМФ РФ двух вертолётоносцев типа 'Мистраль'.");

		private static string[] HintsPool = new[]
		{
			"Now see {0}!!!",
			"Then see {0}!!!",
			"Please see {0}!!!"
		};

		private static long TotalFileLength;
	}
}