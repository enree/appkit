/** @file
 * @brief File archive.
 * Provide save/load capabilities for underlying storage
 *
 * $Id: $
 */

#pragma once

#include "ProxyStorage.h"

#include "ValueAttribute.h"

#include "exception/FileError.h"
#include "serialization/Serializer.h"

#include "gsl/gsl"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

namespace storage
{

/**
 * @brief save_string_file
 * These functions are taken from boost 1.60
 */
inline void
saveStringFile(const boost::filesystem::path& p, const std::string& str)
{
    boost::filesystem::ofstream file;
    file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    file.open(p, std::ios_base::binary);
    file.write(str.c_str(), str.size());
}

inline void loadStringFile(const boost::filesystem::path& p, std::string& str)
{
    boost::filesystem::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(p, std::ios_base::binary);
    std::size_t sz = static_cast<std::size_t>(file_size(p));
    str.resize(sz, '\0');
    file.read(&str[0], sz);
}

/**
 * File archive storage. Store objects in separate files on disk
 */
template <typename StorageType>
class FileArchive : public ProxyStorage<StorageType>, private boost::noncopyable
{
    friend class FileArchiveMutator;

public:
    /**
     * Typedef of template types
     */
    typedef StorageType InnerStorage;
    typedef ProxyStorage<StorageType> BaseType;
    typedef typename BaseType::Type Type;
    typedef typename BaseType::Key Key;
    typedef typename BaseType::Edit Edit;

    typedef serialization::Serializer<Type> Serializer;
    typedef ValueAttribute<Type, Key, std::string> FileArchiveValueAttribute;

protected:
    /**
     * FileArchiveCursor class. Flush changes to disk when editing is complete
     */
    class FileArchiveMutator : public BaseType::ProxyStoragePrivateMutator
    {
    public:
        FileArchiveMutator(
            const Mutator<Edit>& inner, FileArchive<StorageType>* archive)
            : BaseType::ProxyStoragePrivateMutator(inner), m_archive(archive)
        {
        }

        void unlock() override
        {
            flush();
            BaseType::ProxyStoragePrivateMutator::unlock();
        }

        virtual ~FileArchiveMutator()
        {
            flush();
        }

    private:
        void flush()
        {
            if (BaseType::ProxyStoragePrivateMutator::isLocked()
                && BaseType::ProxyStoragePrivateMutator::isInitialized())
            {
                m_archive->save(BaseType::ProxyStoragePrivateMutator::lock());
            }
        }

    private:
        FileArchive<StorageType>* m_archive;
    };

public:
    /**
     * Create file archive.
     * @a path is a path where Archive should put files to
     */
    FileArchive(
        std::shared_ptr<InnerStorage> storage,
        std::string path,
        std::unique_ptr<Serializer> serializer,
        std::unique_ptr<FileArchiveValueAttribute> valueAttribute)
        : BaseType(std::move(storage))
        , m_path(std::move(path))
        , m_serializer(std::move(serializer))
        , m_valueAttribute(std::move(valueAttribute))
    {
        Expects(m_serializer);
        if (!boost::filesystem::exists(m_path))
        {
            boost::filesystem::create_directories(m_path);
        }

        FileArchive::sync();
    }

    /**
     * Insert new object. New file will be created
     */
    void insert(const Type& object) override
    {
        BaseType::insert(object);
        save(object);
    }
    /**
     * Insert objects. New files will be created
     */
    void insertMulti(const std::vector<Type>& objects) override
    {
        BaseType::insertMulti(objects);
        for (const auto& object: objects)
        {
            save(object);
        }
    }

    /**
     * Return Mutator for FileArchive
     */
    Mutator<Edit> get(const Key& address) override
    {
        return Mutator<Edit>(
            std::make_shared<FileArchiveMutator>(BaseType::get(address), this));
    }

    /**
     * Remove object from storage by it's address. This function removes file
     */
    void remove(const Key& key) override
    {
        BaseType::remove(key);
        erase(key);
    }

    /**
     * Remove objects from storage by their keys. This function removes files
     */
    void removeMulti(const std::vector<Key>& keys) override
    {
        BaseType::removeMulti(keys);
        for (const auto& key: keys)
        {
            erase(key);
        }
    }

    /**
     * Remove object from storage by it's address. This function removes file
     */
    boost::optional<Type> take(const Key& key) override
    {
        boost::optional<Type> object = BaseType::take(key);
        erase(key);
        return object;
    }

    /**
     * Remove all objects from storage. This function removes all files in
     * folder
     */
    void clear() override
    {
        boost::filesystem::remove_all(m_path);
        boost::filesystem::create_directories(m_path);

        BaseType::clear();
    }

    /**
     * Reload files from disk
     */
    void sync() override
    {
        auto entities = loadAll();
        BaseType::clear();
        BaseType::insertMulti(entities);
        BaseType::sync();
    }

private:
    /**
     * Load all objects to inner storage. Skip corrupted files
     */
    std::vector<Type> loadAll()
    {
        std::vector<Type> entities;

        using namespace boost::filesystem;
        if (boost::filesystem::exists(m_path))
        {
            directory_iterator endIterator;

            // Iterate through the directory
            for (directory_iterator it(m_path); it != endIterator; ++it)
            {
                // If it's not a directory, load file
                if (is_regular_file(it->path()))
                {

                    try
                    {
                        std::string str;
                        loadStringFile(it->path(), str);
                        entities.push_back(m_serializer->deserialize(str));
                    }
                    catch (const std::exception&)
                    {
                        // Skip file
                    }
                }
            }
        }
        return entities;
    }

    void save(const Type& object)
    {
        boost::filesystem::path file(m_path);
        file /= m_valueAttribute->id(object);
        try
        {
            saveStringFile(file, m_serializer->serialize(object));
        }
        catch (const std::exception& ex)
        {
            BOOST_THROW_EXCEPTION(
                exception::FileError()
                << exception::FileInfo(m_path.native())
                << exception::toForeignExceptionInfo(ex));
        }
    }

    void erase(const Key& key)
    {
        boost::filesystem::path file(m_path);
        file /= m_valueAttribute->id(key);
        boost::filesystem::remove(file);
    }

private:
    boost::filesystem::path m_path;
    std::unique_ptr<Serializer> m_serializer;
    std::unique_ptr<FileArchiveValueAttribute> m_valueAttribute;
};

} // namespace storage
