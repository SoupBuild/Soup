// <copyright file="RuntimeOutputFile.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.IO;

namespace Opal.System
{

    /// <summary>
    /// The standard library output file implementation
    /// </summary>
    internal class RuntimeOutputFile : IOutputFile
    {
        private FileStream stream;
        private bool isDisposed;

        public RuntimeOutputFile(FileStream stream)
        {
            this.isDisposed = false;
            this.stream = stream;
        }

        /// <summary>
        /// Get the file stream
        /// </summary>
        public Stream GetOutStream()
        {
            return this.stream;
        }


        protected virtual void Dispose(bool disposing)
        {
            if (!isDisposed)
            {
                if (disposing)
                {
                    this.stream.Dispose();
                }

                isDisposed = true;
            }
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    };
}
