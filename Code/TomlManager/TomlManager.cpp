#include "TomlManager.h"

#include <CrySystem/ISystem.h>
#if defined(WIN32)
#include <ShlObj.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#elif __linux__
#include <unistd.h>
#include <sys/types.h>
#include <cstddef>
#include <cstdlib>
#endif

int CTomlManager::NewDocument()
{
	std::scoped_lock guard(m_mtxTomlDocuments);

	// Get new document ID.
	int newDocumentId = m_nextTomlDocumentId;
	m_nextTomlDocumentId += 1;

	// Create a fresh TOML object.
	m_tomlDocuments[newDocumentId] = toml::value();

	return newDocumentId;
}

bool CTomlManager::IsDocumentRegistered(int documentId)
{
	std::scoped_lock guard(m_mtxTomlDocuments);

	const auto it = m_tomlDocuments.find(documentId);
	return it != m_tomlDocuments.end();
}

std::variant<std::vector<std::string>, CTomlManager::GetAllDocumentsError> CTomlManager::GetAllDocuments(const std::string& directoryName)
{
	// Check that directory name is not empty.
	if (directoryName.empty())
	{
		return CTomlManager::GetAllDocumentsError::DirectoryNameEmpty;
	}

	// Get base directory to store configs.
	const auto optionalBasePath = GetDirectoryForConfigs();
	if (!optionalBasePath.has_value())
	{
		return CTomlManager::GetAllDocumentsError::FailedToGetBasePath;
	}

	// Construct directory path.
	const auto directoryPath = optionalBasePath.value() / std::string(directoryName);
	if (!std::filesystem::exists(directoryPath))
	{
		return std::vector<std::string>();
	}

	// Scan directory for documents.
	std::vector<std::string> foundDocumentNames;
	const auto directoryIterator = std::filesystem::directory_iterator(directoryPath);
	for (const auto& entry : directoryIterator)
	{
		if (!entry.is_regular_file()) continue;

		if (entry.path().extension().string() == m_backupFileExtension)
		{
			// Backup file. See if original file exists.
			const auto originalFilePath = directoryPath / entry.path().stem().string();
			if (!std::filesystem::exists(originalFilePath))
			{
				// Backup file exists, but not the original file.
				// Copy backup file as the original.
				std::filesystem::copy_file(entry, originalFilePath);
			}

			// Check if we already added the original file.
			bool bFound = false;
			std::string originalFileName = originalFilePath.stem().string();
			for (const auto& fileName : foundDocumentNames)
			{
				if (originalFileName == fileName)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				foundDocumentNames.push_back(originalFileName);
			}
		}
		else
		{
			// Check if we already added this file.
			bool bFound = false;
			std::string entryFileName = entry.path().stem().string();
			for (const auto& fileName : foundDocumentNames)
			{
				if (entryFileName == fileName)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				foundDocumentNames.push_back(entryFileName);
			}
		}
	}

	return foundDocumentNames;
}

std::optional<std::filesystem::path> CTomlManager::GetDirectoryPathForDocuments(const std::string& directoryName)
{
	// Check that directory name is not empty.
	if (directoryName.empty())
	{
		return {};
	}

	// Get base directory to store configs.
	const auto optionalBasePath = GetDirectoryForConfigs();
	if (!optionalBasePath.has_value())
	{
		return {};
	}

	// Construct directory path.
	const auto directoryPath = optionalBasePath.value() / std::string(directoryName);

	return directoryPath;
}

toml::value* CTomlManager::GetTomlData(int documentId)
{
	std::scoped_lock guard(m_mtxTomlDocuments);

	const auto it = m_tomlDocuments.find(documentId);
	if (it == m_tomlDocuments.end())
	{
		return nullptr;
	}

	return &it->second;
}

std::optional<CTomlManager::SaveDocumentError> CTomlManager::SaveDocument(int documentId, const std::string& fileName, const std::string& directoryName, bool bEnableBackup)
{
	std::scoped_lock guard(m_mtxTomlDocuments);

	// Check that file name is not empty.
	if (fileName.empty())
	{
		CloseDocument(documentId);
		return CTomlManager::SaveDocumentError::FileNameEmpty;
	}

	// Check that directory name is not empty.
	if (fileName.empty())
	{
		CloseDocument(documentId);
		return CTomlManager::SaveDocumentError::DirectoryNameEmpty;
	}

	// Check that document exists.
	if (!IsDocumentRegistered(documentId))
	{
		CloseDocument(documentId);
		return CTomlManager::SaveDocumentError::DocumentNotFound;
	}

	// Get TOML data.
	auto pTomlData = GetTomlData(documentId);

	// See if document has something.
	if (pTomlData->is_uninitialized())
	{
		CloseDocument(documentId);
		return CTomlManager::SaveDocumentError::DocumentIsEmpty;
	}

	// Get base directory to store configs.
	const auto optionalBasePath = GetDirectoryForConfigs();
	if (!optionalBasePath.has_value())
	{
		CloseDocument(documentId);
		return CTomlManager::SaveDocumentError::FailedToGetBasePath;
	}

	// Construct directory path.
	const auto directoryPath = optionalBasePath.value() / std::string(directoryName);
	if (!std::filesystem::exists(directoryPath)) {
		std::filesystem::create_directories(directoryPath);
	}

	// Construct file path.
	const auto filePath = directoryPath / (std::string(fileName) + ".toml");

	// Handle backup.
	std::filesystem::path backupFile = filePath;
	backupFile += m_backupFileExtension;
	if (bEnableBackup)
	{
		// Check if we already had this file saved.
		if (std::filesystem::exists(filePath))
		{
			if (std::filesystem::exists(backupFile))
			{
				std::filesystem::remove(backupFile);
			}
			std::filesystem::rename(filePath, backupFile);
		}
	}

	// Save document.
	std::ofstream outFile(filePath, std::ios::binary);
	if (!outFile.is_open())
	{
		CloseDocument(documentId);
		return CTomlManager::SaveDocumentError::UnableToCreateFile;
	}
	outFile << *pTomlData;
	outFile.close();

	CryLogAlways("[%s]: saved TOML document at \"%s\" (document %i)", m_logCategory, filePath.string().c_str(), documentId);

	CloseDocument(documentId);

	if (bEnableBackup)
	{
		// Create backup file if it does not exist.
		if (!std::filesystem::exists(backupFile))
		{
			std::filesystem::copy_file(filePath, backupFile);
		}
	}

	return {};
}

std::variant<int, CTomlManager::OpenDocumentError> CTomlManager::OpenDocument(const std::string& fileName, const std::string& directoryName)
{
	std::scoped_lock guard(m_mtxTomlDocuments);

	// Check that file name is not empty.
	if (fileName.empty())
	{
		return CTomlManager::OpenDocumentError::FileNameEmpty;
	}

	// Check that directory name is not empty.
	if (fileName.empty())
	{
		return CTomlManager::OpenDocumentError::DirectoryNameEmpty;
	}

	// Get base directory to store configs.
	const auto optionalBasePath = GetDirectoryForConfigs();
	if (!optionalBasePath.has_value())
	{
		return CTomlManager::OpenDocumentError::FailedToGetBasePath;
	}

	// Construct directory path.
	const auto directoryPath = optionalBasePath.value() / std::string(directoryName);
	if (!std::filesystem::exists(directoryPath)) {
		std::filesystem::create_directories(directoryPath);
	}

	// Construct file path.
	const auto filePath = directoryPath / (std::string(fileName) + ".toml");

	// Handle backup.
	std::filesystem::path backupFile = filePath;
	backupFile += m_backupFileExtension;
	if (!std::filesystem::exists(filePath))
	{
		// Check if backup file exists.
		if (std::filesystem::exists(backupFile))
		{
			std::filesystem::copy_file(backupFile, filePath);
		}
		else
		{
			return CTomlManager::OpenDocumentError::FileNotFound;
		}
	}

	// Register new document.
	const auto documentId = NewDocument();

	// Get TOML data.
	auto pTomlData = GetTomlData(documentId);

	// Try parsing file.
	try
	{
		*pTomlData = toml::parse(filePath);
	}
	catch (std::exception& exception)
	{
		CryLogAlways("[%s]: failed to parse file at \"%s\", error: %s", m_logCategory, filePath.string().c_str(), exception.what());
		return CTomlManager::OpenDocumentError::ParsingFailed;
	}

	return documentId;
}

bool CTomlManager::CloseDocument(int documentId)
{
	std::scoped_lock guard(m_mtxTomlDocuments);

	// Find document with this ID.
	const auto it = m_tomlDocuments.find(documentId);
	if (it == m_tomlDocuments.end())
	{
		return false;
	}

	// Remove document ID.
	m_tomlDocuments.erase(it);

	return true;
}

std::optional<std::filesystem::path> CTomlManager::GetDirectoryForConfigs()
{
	std::filesystem::path directoryPath;

#if defined(WIN32)

	// Try to get AppData folder.
	PWSTR pathTmp;
	const HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &pathTmp);
	if (result != S_OK) {
		CoTaskMemFree(pathTmp);

		CryLogAlways("[%s]: failed to get directory for configs, error code: %ld", m_logCategory, result);
		return {};
	}

	directoryPath = pathTmp;

	CoTaskMemFree(pathTmp);

#elif __linux__

	const auto sHomePath = std::string(getenv("HOME"));
	if (sHomePath.empty()) {
		CryLogAlways("[%s]: environment variable HOME is not set", GetName());
		return {};
	}

	directoryPath = sHomePath + "/.config/";

#else

	static_assert(false, "not implemented");

#endif

	if (!std::filesystem::exists(directoryPath)) {
		std::filesystem::create_directories(directoryPath);
	}

	return directoryPath;
}
