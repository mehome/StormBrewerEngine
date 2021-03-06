#pragma once

#include "Runtime/GenericResource/GenericResource.h"

template <typename ConfigDataType>
class ConfigPtr : public DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>
{
public:

  ConfigPtr() = default;
  ConfigPtr(const ConfigPtr<ConfigDataType> & rhs) = default;
  ConfigPtr(ConfigPtr<ConfigDataType> && rhs) = default;

  ConfigPtr(DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>> && ref) :
    DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>(std::move(ref))
  {

  }

  ConfigPtr(const DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>> & rhs) :
    DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>(rhs)
  {

  }

  ConfigPtr<ConfigDataType> & operator = (const DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>> & rhs)
  {
    DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>::operator =(rhs);
    return *this;
  }

  ConfigPtr<ConfigDataType> & operator = (DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>> && rhs)
  {
    DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>::operator =(std::move(rhs));
    return *this;
  }

  NotNullPtr<ConfigDataType> operator *()
  {
    return DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>::GetData();
  }

  NotNullPtr<const ConfigDataType> operator *() const
  {
    return DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>::GetData();
  }

  NotNullPtr<ConfigDataType> operator ->()
  {
    return DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>::GetData();
  }

  NotNullPtr<const ConfigDataType> operator ->() const
  {
    return DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>::GetData();
  }

  bool operator == (const ConfigPtr<ConfigDataType> & rhs) const
  {
    return DocumentResourcePtr<ConfigDataType, GenericResource<ConfigDataType>>::m_Resource == rhs.m_Resource;
  }
};
