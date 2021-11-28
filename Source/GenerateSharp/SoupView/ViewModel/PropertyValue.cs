// <copyright file="PropertyValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace SoupView.ViewModel
{
    public class PropertyValue
    {
        public PropertyValue(string name, string value)
        {
            this.Name = name;
            this.Value = value;
        }

        public string Name { get; set; } = string.Empty;
        public string Value { get; set; } = string.Empty;
    }
}
