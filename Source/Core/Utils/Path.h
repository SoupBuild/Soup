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
        static constexpr const char* ParentDirectory = "..";

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Path"/> class.
        /// </summary>
        Path() :
            m_root(),
            m_directories(),
            m_filename()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Path"/> class.
        /// </summary>
        Path(const std::string& value) :
            m_root(),
            m_directories(),
            m_filename()
        {
            ParsePath(value);
            NormalizeDirectories();
        }

        /// <summary>
        /// Gets a value indicating whether the path has a root
        /// </summary>
        bool HasRoot() const
        {
            return !m_root.empty();
        }

        /// <summary>
        /// Gets the path root
        /// </summary>
        std::string GetRoot() const
        {
            return m_root;
        }

        /// <summary>
        /// Gets a value indicating whether the path has a file name
        /// </summary>
        bool HasFileName() const
        {
            return !m_filename.empty();
        }

        /// <summary>
        /// Gets the file name
        /// </summary>
        std::string GetFileName() const
        {
            return m_filename;
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
            auto lastSeparator = m_filename.find_last_of(FileExtensionSeparator);
            if (lastSeparator != std::string::npos)
            {
                return m_filename.substr(0, lastSeparator);
            }
            else
            {
                // Return the entire filename if no extension
                return m_filename;
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
            auto lastSeparator = m_filename.find_last_of(FileExtensionSeparator);
            if (lastSeparator != std::string::npos)
            {
                return m_filename.substr(lastSeparator);
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
            return m_root == rhs.m_root &&
                m_directories == rhs.m_directories &&
                m_filename == rhs.m_filename;
        }

        bool operator !=(const Path& rhs) const
        {
            return m_root != rhs.m_root ||
                m_directories != rhs.m_directories ||
                m_filename != rhs.m_filename;
        }

        /// <summary>
        /// Concatenate paths
        /// </summary>
        Path operator +(const Path& rhs) const
        {
            if (rhs.HasRoot())
                throw std::runtime_error("Cannot combine a rooted path on the right hand side.");

            auto result = Path();

            // Take the root from the left hand side
            result.m_root = m_root;

            // Combine the directories
            result.m_directories.reserve(
                m_directories.size() + rhs.m_directories.size());
            result.m_directories.insert(
                result.m_directories.end(),
                m_directories.begin(),
                m_directories.end());

            // Convert the left hand side filename to a directory
            if (HasFileName())
            {
                result.m_directories.push_back(m_filename);
            }

            result.m_directories.insert(
                result.m_directories.end(),
                rhs.m_directories.begin(),
                rhs.m_directories.end());

            // Take the filename from the right hand side
            result.m_filename = rhs.m_filename;

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
                stringBuilder << m_root << DirectorySeparator;
            }

            for (size_t i = 0; i < m_directories.size(); i++)
            {
                stringBuilder << m_directories[i] << DirectorySeparator;
            }

            if (HasFileName())
            {
                stringBuilder << m_filename;
            }

            return stringBuilder.str();
        }

        void Print()
        {
            std::cout << 'R' << m_root << std::endl;
            for (auto& value : m_directories)
                std::cout << 'D' << value << std::endl;

            std::cout << 'F' << m_filename << std::endl;
        }

private:
    void ParsePath(const std::string& value)
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
                    m_root = std::move(directory);
                }
                else
                {
                    m_directories.push_back(std::move(directory));
                }

                isFirst = false;
            }
            else
            {
                m_directories.push_back(std::move(directory));
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
                    m_root = std::move(directory);
                }
                else
                {
                    m_filename = std::move(directory);
                }

                isFirst = false;
            }
            else
            {
                m_filename = std::move(directory);
            }
        }
    }

    bool IsRoot(const std::string& value)
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
        for (size_t i = 0; i < m_directories.size(); i++)
        {
            // Skip over the first directory entry
            if (i != 0)
            {
                // Remove a parent directory if possible
                if (m_directories.at(i) == ParentDirectory &&
                    m_directories.at(i - 1) != ParentDirectory)
                {
                    // Remove the directories and move back
                    m_directories.erase(
                        m_directories.begin() + (i - 1),
                        m_directories.begin() + (i + 1));
                    i -= 2;
                }
            }
        }
    }

    private:
        std::string m_root;
        std::vector<std::string> m_directories;
        std::string m_filename;
    };
}
