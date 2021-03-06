#pragma once

#include <atomic>

#include "Foundation/Delegate/DelegateList.h"
#include "Foundation/Delegate/DelegateLink.h"
#include "Foundation/Any/Any.h"

#include "Runtime/RuntimeCommon.h"
#include "Runtime/DocumentResource/DocumentResourceReferenceBase.h"

class DocumentResourceManager;

class RUNTIME_EXPORT DocumentResourceBase
{
public:
  bool IsLoaded() const;
  bool IsError() const;
  czstr GetFileName() const;
  uint64_t GetFileNameHash() const;

  virtual ~DocumentResourceBase();

protected:
  friend class DocumentResourceReferenceBase;

  DocumentResourceBase(Any && load_data, uint32_t path_hash, czstr path);

  void IncRef();
  void DecRef();

protected:

  static NullOptPtr<DocumentResourceBase> FindDocumentResource(uint32_t file_path_hash);
  static NotNullPtr<DocumentResourceBase> LoadDocumentResource(czstr file_path,
          std::unique_ptr<DocumentResourceBase>(*ResourceCreator)(Any &&, uint32_t, czstr));

  virtual void OnDataLoadComplete(const std::string & resource_data);
  virtual void CallAssetLoadCallbacks();

protected:

  friend class DocumentResourceManager;

  Any m_LoadData;

  std::atomic_int m_RefCount;
  bool m_Loaded;
  bool m_Error;

  NotNullPtr<DocumentResourceManager> m_ResourceManager;
  std::string m_FileName;
  uint32_t m_FileNameHash;
};
