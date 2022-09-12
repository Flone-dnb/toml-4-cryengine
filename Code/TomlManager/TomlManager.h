#pragma once

#include <unordered_map>
#include <mutex>
#include <optional>
#include <filesystem>
#include "External/toml11/toml.hpp"

//! Allows working with TOML files.
class CTomlManager
{
public:
	//! Describes TOML manager's operation error.
	enum class SetValueError {
		DocumentNotFound, //!< Document ID is not registered or this document was saved (and ID is no longer valid).
		KeyEmpty,         //!< Key parameter is empty.
	};

	//! Describes TOML manager's operation error.
	enum class SaveDocumentError {
		DocumentNotFound,    //!< Document ID is not registered or this document was saved (and ID is no longer valid).
		DocumentIsEmpty,     //!< Document has no data to save (no value was set before).
		FileNameEmpty,       //!< File name parameter is empty.
		DirectoryNameEmpty,  //!< Directory name parameter is empty.
		FailedToGetBasePath, //!< Failed to get base path for storing your document (see logs for details).
		UnableToCreateFile,  //!< Unable to create/open file.
	};

	//! Constructor.
	CTomlManager() = default;

	//! Registers a new TOML document and returns this document's unique ID.
	//! 
	//! \return New document ID.
	int RegisterNewTomlDocument();

	//! Checks if the specified document was registered before.
	//! 
	//! \param documentId Document to check.
	//! 
	//! \return 'true' if registered, 'false' if not.
	bool IsDocumentRegistered(int documentId);

	//! Sets a value into a TOML documents.
	//! 
	//! \param documentId  Document to write value to.
	//! \param key         Name of the key for the value.
	//! \param value       Value to write.
	//! \param sectionName Optional. Section name for the value.
	//! 
	//! \return Error if something went wrong.
	std::optional<SetValueError> SetValue(int documentId, const std::string& key, const std::string& value, const std::string& sectionName = "");

	//! Saves document to file and invalidates document ID.
	//! 
	//! \param documentId    Document to write value to.
	//! \param fileName      Name of the file without ".toml" extension for the document.
	//! \param directoryName Usually your game name. Directory for file (will be appended to the base path).
	//! \param bOverwrite    Whether to overwrite already existing file or not.
	//! 
	//! \remark If the document was saved successfully the document's ID is no longer valid.
	std::optional<SaveDocumentError> SaveDocument(int documentId, const std::string& fileName, const std::string& directoryName, bool bOverwrite = true);

	//! Returns directory path to store config files.
	//! 
	//! \return Empty if something went wrong (see logs), otherwise directory path,
	//! "%localappdata%" on Windows, "%HOME%/.config" on Linux.
	std::optional<std::filesystem::path> GetDirectoryForConfigs();

private:

	//! Returns document's TOML data.
	//! 
	//! \param documentId Document for which to get TOML data.
	//! 
	//! \return nullptr if no document was registered for the specified ID, valid pointer otherwise.
	toml::value* GetTomlData(int documentId);

	//! Text that we add before log text.
	static inline const auto m_logCategory = "TomlManager";

	//! Created but not saved yet TOML documents.
	std::unordered_map<size_t, toml::value> m_tomlDocuments;

	//! ID for the next created TOML document.
	int m_nextTomlDocumentId = 0;

	//! Mutex for read/write operations on TOML documents and IDs.
	std::recursive_mutex m_mtxTomlDocuments;
};

