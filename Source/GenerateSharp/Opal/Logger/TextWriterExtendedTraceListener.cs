// <copyright file="TextWriterExtendedTraceListener.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal
{
	using System;
	using System.IO;
	using System.Text;

	/// <summary>
	/// A clone of the <see cref="System.Diagnostics.TextWriterTraceListener"/> that utilizes the <see cref="ExtendedTraceListener"/>
	/// </summary>
	public class TextWriterExtendedTraceListener : ExtendedTraceListener
	{
		private TextWriter _writer;
		private string _fileName = null;

		/// <summary>
		/// Initializes a new instance of the <see cref='TextWriterExtendedTraceListener'/> class with
		/// <see cref='System.IO.TextWriter'/> as the output recipient.
		/// </summary>
		public TextWriterExtendedTraceListener()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='TextWriterExtendedTraceListener'/> class, using the
		/// stream as the recipient of the debugging and tracing output.
		/// </summary>
		public TextWriterExtendedTraceListener(Stream stream)
			: this(stream, string.Empty)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='TextWriterExtendedTraceListener'/> class with the
		/// specified name and using the stream as the recipient of the debugging and tracing output.
		/// </summary>
		public TextWriterExtendedTraceListener(Stream stream, string name)
			: base(name)
		{
			_writer = new StreamWriter(
				stream ?? throw new ArgumentNullException(nameof(stream)));
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='TextWriterExtendedTraceListener'/> class using the
		/// specified writer as recipient of the tracing or debugging output.
		/// </summary>
		public TextWriterExtendedTraceListener(TextWriter writer)
			: this(writer, string.Empty)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='TextWriterExtendedTraceListener'/> class with the
		/// specified name and using the specified writer as recipient of the tracing or
		/// debugging output.
		/// </summary>
		public TextWriterExtendedTraceListener(TextWriter writer, string name)
			: base(name)
		{
			_writer = writer ?? throw new ArgumentNullException(nameof(writer));
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='TextWriterExtendedTraceListener'/> class
		/// </summary>
		public TextWriterExtendedTraceListener(string fileName)
		{
			_fileName = fileName;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='TextWriterExtendedTraceListener'/> class
		/// </summary>
		public TextWriterExtendedTraceListener(string fileName, string name)
			: base(name)
		{
			_fileName = fileName;
		}

		/// <summary>
		/// Gets or sets the text writer that receives the tracing
		/// or debugging output.
		/// </summary>
		public TextWriter Writer
		{
			get
			{
				EnsureWriter();
				return _writer;
			}

			set
			{
				_writer = value;
			}
		}

		/// <summary>
		/// Closes the <see cref='TextWriterExtendedTraceListener.Writer'/> so that it no longer
		/// receives tracing or debugging output.
		/// </summary>
		public override void Close()
		{
			if (_writer != null)
			{
				try
				{
					_writer.Close();
				}
				catch (ObjectDisposedException)
				{
				}
			}

			_writer = null;
		}

		/// <summary>
		/// Flushes the output buffer for the <see cref='TextWriterExtendedTraceListener.Writer'/>.
		/// </summary>
		public override void Flush()
		{
			if (!EnsureWriter())
			{
				return;
			}

			try
			{
				_writer.Flush();
			}
			catch (ObjectDisposedException)
			{
			}
		}

		/// <summary>
		/// Writes a message
		/// to this instance's <see cref='TextWriterExtendedTraceListener.Writer'/>.
		/// </summary>
		public override void Write(string message)
		{
			if (!EnsureWriter())
			{
				return;
			}

			if (NeedIndent)
			{
				WriteIndent();
			}

			try
			{
				_writer.Write(message);
			}
			catch (ObjectDisposedException)
			{
			}
		}

		/// <summary>
		/// Writes a message
		/// to this instance's <see cref='TextWriterExtendedTraceListener.Writer'/> followed by a line terminator. The
		/// default line terminator is a carriage return followed by a line feed (\r\n).
		/// </summary>
		public override void WriteLine(string message)
		{
			if (!EnsureWriter())
			{
				return;
			}

			if (NeedIndent)
			{
				WriteIndent();
			}

			try
			{
				_writer.WriteLine(message);
				NeedIndent = true;
			}
			catch (ObjectDisposedException)
			{
			}
		}

		/// <summary>
		/// Dispose
		/// </summary>
		protected override void Dispose(bool disposing)
		{
			try
			{
				if (disposing)
				{
					Close();
				}
				else
				{
					// clean up resources
					if (_writer != null)
					{
						try
						{
							_writer.Close();
						}
						catch (ObjectDisposedException)
						{
						}
					}

					_writer = null;
				}
			}
			finally
			{
				base.Dispose(disposing);
			}
		}

		private static Encoding GetEncodingWithFallback(Encoding encoding)
		{
			// Clone it and set the "?" replacement fallback
			Encoding fallbackEncoding = (Encoding)encoding.Clone();
			fallbackEncoding.EncoderFallback = EncoderFallback.ReplacementFallback;
			fallbackEncoding.DecoderFallback = DecoderFallback.ReplacementFallback;

			return fallbackEncoding;
		}

		/// <summary>
		/// This uses a machine resource, scoped by the fileName variable.
		/// </summary>
		private bool EnsureWriter()
		{
			bool ret = true;

			if (_writer == null)
			{
				ret = false;

				if (_fileName == null)
				{
					return ret;
				}

				// StreamWriter by default uses UTF8Encoding which will throw on invalid encoding errors.
				// This can cause the internal StreamWriter's state to be irrecoverable. It is bad for tracing
				// APIs to throw on encoding errors. Instead, we should provide a "?" replacement fallback
				// encoding to substitute illegal chars. For ex, In case of high surrogate character
				// D800-DBFF without a following low surrogate character DC00-DFFF
				// NOTE: We also need to use an encoding that does't emit BOM whic is StreamWriter's default
				Encoding noBOMwithFallback = GetEncodingWithFallback(new UTF8Encoding(false));

				// To support multiple appdomains/instances tracing to the same file,
				// we will try to open the given file for append but if we encounter
				// IO errors, we will prefix the file name with a unique GUID value
				// and try one more time
				string fullPath = Path.GetFullPath(_fileName);
				string dirPath = Path.GetDirectoryName(fullPath);
				string fileNameOnly = Path.GetFileName(fullPath);

				for (int i = 0; i < 2; i++)
				{
					try
					{
						_writer = new StreamWriter(fullPath, true, noBOMwithFallback, 4096);
						ret = true;
						break;
					}
					catch (IOException)
					{
						fileNameOnly = Guid.NewGuid().ToString() + fileNameOnly;
						fullPath = Path.Combine(dirPath, fileNameOnly);
						continue;
					}
					catch (UnauthorizedAccessException)
					{
						// ERROR_ACCESS_DENIED, mostly ACL issues
						break;
					}
					catch (Exception)
					{
						break;
					}
				}

				if (!ret)
				{
					// Disable tracing to this listener. Every Write will be nop.
					// We need to think of a central way to deal with the listener
					// init errors in the future. The default should be that we eat
					// up any errors from listener and optionally notify the user
					_fileName = null;
				}
			}

			return ret;
		}
	}
}
