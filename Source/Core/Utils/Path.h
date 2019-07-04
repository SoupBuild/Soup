// <copyright file="SemanticVersion.hs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once

namespace Soup
{
    /// <summary>
    /// A container for a path string
    /// </summary>
    export class Path
    {
    private:
        static constexpr char DirectorySeparator = '/';
        static constexpr char LetterDriveSpecifier = ':';
        static constexpr char FileExtensionSeparator = '.';
        static constexpr std::string_view ParentDirectory = "..";

    public:
        static Path GetCurrentDirectory()
        {
            auto current = std::filesystem::current_path();
            return Path(current.string());
        }

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Path"/> class.
        /// </summary>
        Path() :
            _root(),
            _directories(),
            _filename()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Path"/> class.
        /// </summary>
        Path(std::string_view value) :
            _root(),
            _directories(),
            _filename()
        {
            ParsePath(value);
            NormalizeDirectories();
        }

        /// <summary>
        /// Gets a value indicating whether the path has a root
        /// </summary>
        bool HasRoot() const
        {
            return !_root.empty();
        }

        /// <summary>
        /// Gets the path root
        /// </summary>
        std::string GetRoot() const
        {
            return _root;
        }

        /// <summary>
        /// Gets a value indicating whether the path has a file name
        /// </summary>
        bool HasFileName() const
        {
            return !_filename.empty();
        }

        /// <summary>
        /// Gets the file name
        /// </summary>
        std::string GetFileName() const
        {
            return _filename;
        }

        /// <summary>
        /// Gets a value indicating whether the file name has an stem
        /// </summary>
        bool HasFileStem() const
        {
            return !GetFileStem().empty();
        }

        /// <summary>
        /// Gets the file name minus the extension
        /// </summary>
        std::string GetFileStem() const
        {
            // Everything before the last period is the stem
            auto lastSeparator = _filename.find_last_of(FileExtensionSeparator);
            if (lastSeparator != std::string::npos)
            {
                return _filename.substr(0, lastSeparator);
            }
            else
            {
                // Return the entire filename if no extension
                return _filename;
            }
        }

        /// <summary>
        /// Gets a value indicating whether the file name has an extension
        /// </summary>
        bool HasFileExtension() const
        {
            return !GetFileExtension().empty();
        }

        /// <summary>
        /// Gets the file extension
        /// </summary>
        std::string GetFileExtension() const
        {
            // Everything after and including the last period is the extension
            auto lastSeparator = _filename.find_last_of(FileExtensionSeparator);
            if (lastSeparator != std::string::npos)
            {
                return _filename.substr(lastSeparator);
            }
            else
            {
                // Return the empty string
                return "";
            }
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const Path& rhs) const
        {
            return _root == rhs._root &&
                _directories == rhs._directories &&
                _filename == rhs._filename;
        }

        bool operator !=(const Path& rhs) const
        {
            return _root != rhs._root ||
                _directories != rhs._directories ||
                _filename != rhs._filename;
        }

        bool operator <(const Path& rhs) const
        {
            // TODO: Tests and better implementation
            return ToString() < rhs.ToString();
        }

        /// <summary>
        /// Concatenate paths
        /// </summary>
        Path operator +(const Path& rhs) const
        {
            if (rhs.HasRoot())
                throw std::runtime_error(
                    "Cannot combine a rooted path on the right hand side: " + rhs.ToString());

            auto result = Path();

            // Take the root from the left hand side
            result._root = _root;

            // Combine the directories
            result._directories.reserve(
                _directories.size() + rhs._directories.size());
            result._directories.insert(
                result._directories.end(),
                _directories.begin(),
                _directories.end());

            // Convert the left hand side filename to a directory
            if (HasFileName())
            {
                result._directories.push_back(_filename);
            }

            result._directories.insert(
                result._directories.end(),
                rhs._directories.begin(),
                rhs._directories.end());

            // Take the filename from the right hand side
            result._filename = rhs._filename;

            result.NormalizeDirectories();

            return result;
        }

        /// <summary>
        /// Convert to string
        /// </summary>
        std::string ToString() const
        {
            std::stringstream stringBuilder;

            if (HasRoot())
            {
                stringBuilder << _root << DirectorySeparator;
            }

            for (size_t i = 0; i < _directories.size(); i++)
            {
                stringBuilder << _directories[i] << DirectorySeparator;
            }

            if (HasFileName())
            {
                stringBuilder << _filename;
            }

            return stringBuilder.str();
        }

private:
    void ParsePath(std::string_view value)
    {
        size_t current = 0;
        size_t next = 0;
        bool isFirst = true;
        while ((next = value.find(DirectorySeparator, current)) != std::string::npos)
        {
            auto directory = value.substr(current, next - current);

            // Check if the first entry is a root
            if (isFirst)
            {
                if (IsRoot(directory))
                {
                    _root = std::string(directory);
                }
                else
                {
                    _directories.push_back(std::string(directory));
                }

                isFirst = false;
            }
            else
            {
                _directories.push_back(std::string(directory));
            }

            current = next + 1;
        }

        // Check if there are characters beyond the last separator
        if (current != value.size())
        {
            auto directory = value.substr(current, next - current);

            // Check if still on the first entry
            // Could be empty root or single filename
            if (isFirst)
            {
                if (IsRoot(directory))
                {
                    _root = std::move(directory);
                }
                else
                {
                    _filename = std::move(directory);
                }

                isFirst = false;
            }
            else
            {
                _filename = std::move(directory);
            }
        }
    }

    bool IsRoot(std::string_view value)
    {
        // Check for drive letter
        if (value.size() == 2)
        {
            if (std::isalpha(value[0]) && value[1] == LetterDriveSpecifier)
            {
                return true;
            }
        }

        return false;
    }

    void NormalizeDirectories()
    {
        // Remove as many up directories as we can
        for (size_t i = 0; i < _directories.size(); i++)
        {
            // Skip over the first directory entry
            if (i != 0)
            {
                // Remove a parent directory if possible
                if ( _directories.at(i) == ParentDirectory &&
                    _directories.at(i - 1) != ParentDirectory)
                {
                    // Remove the directories and move back
                    _directories.erase(
                        _directories.begin() + (i - 1),
                        _directories.begin() + (i + 1));
                    i -= 2;
                }
            }
        }
    }

    private:
        std::string _root;
        std::vector<std::string> _directories;
        std::string _filename;
    };
}
