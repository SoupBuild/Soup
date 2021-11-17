// <copyright file="MainPageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Threading.Tasks;
using System.Windows.Input;
using Microsoft.Maui.Controls;
using Microsoft.Maui.Essentials;
using Microsoft.Toolkit.Mvvm.Input;

namespace SoupView.ViewModels
{
    public class MainPageModel : ObservableProperty
    {
        private string message = "test";
        public string Message
        {
            get { return message; }
            private set
            {
                if (value != message)
                {
                    message = value;
                    OnPropertyChanged();
                }
            }
        }

        public IAsyncRelayCommand LoadCommand => new AsyncRelayCommand<string>(LoadProjectAsync);
        
        private async Task<string> LoadProjectAsync(string item)
        {
            try
            {
                var result = await FilePicker.PickAsync();
                if (result != null)
                {
                    ////Text = $"File Name: {result.FileName}";
                    ////if (result.FileName.EndsWith("jpg", StringComparison.OrdinalIgnoreCase) ||
                    ////    result.FileName.EndsWith("png", StringComparison.OrdinalIgnoreCase))
                    ////{
                    ////    var stream = await result.OpenReadAsync();
                    ////    Image = ImageSource.FromStream(() => stream);
                    ////}
                }

                return string.Empty;
            }
            catch (Exception)
            {
                // The user canceled or something went wrong
            }

            return null;
        }
    }
}
