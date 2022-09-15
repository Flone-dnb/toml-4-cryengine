#pragma once

#include <unordered_map>
#include <mutex>
#include <optional>
#include <filesystem>
#include <variant>
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
	enum class GetValueError {
		DocumentNotFound,  //!< Document ID is not registered or this document was saved (and ID is no longer valid).
		KeyEmpty,          //!< Key parameter is empty.
		ValueNotFound,     //!< Value for the specified key/section is not found.
		ValueTypeNotString //!< Value for the specified key/section is not string.
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

	//! Describes TOML manager's operation error.
	enum class OpenDocumentError {
		FileNotFound,        //!< The specified file/directory does not exist.
		FileNameEmpty,       //!< File name parameter is empty.
		DirectoryNameEmpty,  //!< Directory name parameter is empty.
		FailedToGetBasePath, //!< Failed to get base path for storing your document (see logs for details).
		ParsingFailed,       //!< Failed to parse given TOML file (see logs for details).
	};

	//! Constructor.
	CTomlManager() = default;

	//! Returns path where we store documents.
	//! 
	//! \param directoryName Usually your game name. Directory for documents (will be appended to the base path).
	//! 
	//! \return Empty if the directory name is empty or something went wrong (see logs),
	//! otherwise path to the directory where the documents are stored (the directory might not exist).
	static std::optional<std::filesystem::path> GetDirectoryPathForDocuments(const std::string& directoryName);

	//! Initializes a fresh new TOML document and returns this document's unique ID.
	//! 
	//! \return New document ID.
	int NewDocument();

	//! Checks if the specified document was registered before.
	//! 
	//! \param documentId Document to check.
	//! 
	//! \return 'true' if registered, 'false' if not.
	bool IsDocumentRegistered(int documentId);

	//! Sets a value into a TOML documents.
	//! 
	//! \param documentId  Document to write value to.
	//! \param keyName     Name of the key for the value.
	//! \param value       Value to write.
	//! \param sectionName Optional. Section name for the value.
	//! 
	//! \return Error if something went wrong.
	std::optional<SetValueError> SetValue(int documentId, const std::string& keyName, const std::string& value, const std::string& sectionName = "");

	//! Returns a string value from TOML document.
	//! 
	//! \param documentId  Document to get value from.
	//! \param keyName     Name of the key of the value.
	//! \param sectionName Optional. Section name of the value.
	//! 
	//! \warning If the value has type other than std::string this function will return error.
	//! 
	//! \return Error if something went wrong, otherwise found value.
	std::variant<std::string, GetValueError> GetValue(int documentId, const std::string& keyName, const std::string& sectionName = "");

	//! Saves document to file and closes the document (so you don't need to call \ref CloseDocument).
	//! 
	//! \param documentId    Document to write value to.
	//! \param fileName      Name of the file without ".toml" extension for the document.
	//! \param directoryName Usually your game name. Directory for file (will be appended to the base path).
	//! \param bOverwrite    Whether to overwrite already existing file or not.
	//! 
	//! \return Error if something went wrong.
	std::optional<SaveDocumentError> SaveDocument(int documentId, const std::string& fileName, const std::string& directoryName, bool bOverwrite = true);

	//! Opens a document file and returns its new ID.
	//! 
	//! \param fileName      Name of the file without ".toml" extension for the document.
	//! \param directoryName Usually your game name. Directory for file (will be appended to the base path).
	//! 
	//! \return ID of the opened document if successful, otherwise error.
	std::variant<int, OpenDocumentError> OpenDocument(const std::string& fileName, const std::string& directoryName);

	//! Invalidates document ID and clears internal data related to it.
	//! 
	//! \param documentId Document to invalidate.
	//! 
	//! \remark Usually you don't need to call this function explicitly as \ref SaveDocument will do that for you,
	//! but if you opened a document, read some values from it, then call this function or
	//! if you created a new document, added some values to it and decided to not save this document, call this function
	//! to clear internal data related to this document.
	//! 
	//! \return 'true' if the document with this ID was found and closed, 'false' otherwise.
	bool CloseDocument(int documentId);

private:

	//! Returns directory path to store config files.
	//! 
	//! \return Empty if something went wrong (see logs), otherwise directory path,
	//! "%localappdata%" on Windows, "%HOME%/.config" on Linux.
	static std::optional<std::filesystem::path> GetDirectoryForConfigs();

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

