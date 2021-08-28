// <copyright file="STLInputFile.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.IO;

namespace Opal.System
{

    /// <summary>
    /// The standard library input file implementation
    /// </summary>
    class RuntimeInputFile : IInputFile
    {
        private FileStream stream;
        private bool isDisposed;

        public RuntimeInputFile(FileStream stream)
        {
            this.isDisposed = false;
            this.stream = stream;
        }

        /// <summary>
        /// Gets the file stream
        /// </summary>
        public Stream GetInStream()
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
    }
}
