#pragma once

#include <chrono>

using DocumentLoadCallback = Delegate<void, uint32_t, Optional<Buffer> &&, std::chrono::system_clock::time_point>;

class DocumentLoader
{
public:
  virtual void LoadDocument(czstr path, uint32_t file_hash, DocumentLoadCallback callback) = 0;
};

