// <copyright file="RuntimeOutputFile.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System
{
    using global::System;
    using global::System.IO;

    /// <summary>
    /// The standard library output file implementation.
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
        /// Get the file stream.
        /// </summary>
        public Stream GetOutStream()
        {
            return this.stream;
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            this.Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!this.isDisposed)
            {
                if (disposing)
                {
                    this.stream.Dispose();
                }

                this.isDisposed = true;
            }
        }
    }
}
