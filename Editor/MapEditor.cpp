
#include "StormData/StormDataParent.h"

#include "Foundation/PropertyMeta/PropertyFieldMetaFuncs.h"

#include "Runtime/Map/MapTileJson.h"
#include "Runtime/Map/MapDef.refl.meta.h"

#include "EditorContainer.h"
#include "DocumentEditor.h"
#include "MapEditor.h"

#include "MapEditorToolManualTileLayerDraw.h"
#include "MapEditorToolManualTileLayerSelect.h"
#include "MapEditorToolEntityLayerDraw.h"
#include "MapEditorToolEntityLayerSelect.h"
#include "MapEditorToolEntityLayerDraw.h"
#include "MapEditorToolVolumeCreate.h"
#include "MapEditorToolVolumeEditor.h"
#include "MapEditorToolPathCreate.h"
#include "MapEditorToolPathEditor.h"


MapEditor::MapEditor(PropertyFieldDatabase & property_db, const std::string & root_path, MapDef & map, DocumentChangeLinkDelegate && change_link_callback,
  DocumentBeginTransactionDelegate && begin_transaction_callback, DocumentCommitChangesDelegate && commit_change_callback, QWidget *parent) :
  DocumentEditorWidgetBase(property_db, root_path, std::move(change_link_callback), std::move(begin_transaction_callback), std::move(commit_change_callback), parent),
  m_Map(map),
  m_ManualTileLayers(this, m_Map),
  m_EntityLayers(this, m_Map),
  m_ParalaxLayers(this, m_Map),
  m_Volumes(this, m_Map),
  m_Paths(this, m_Map),
  m_Layout(std::make_unique<QGridLayout>()),
  m_Properties(std::make_unique<GenericFrame>("Properties", this)),
  m_Selector(std::make_unique<MapEditorSelector>(this, m_Map)),
  m_LayerList(std::make_unique<MapEditorLayerList>(this, m_Map)),
  m_Viewer(std::make_unique<MapEditorViewer>(this, m_Map)),
  m_IgnoreSelectionChanges(false)
{
  m_Properties->setMinimumWidth(230);

  m_Layout->setColumnStretch(0, 1);
  m_Layout->setColumnMinimumWidth(0, 100);

  m_Layout->setColumnStretch(1, 4);
  m_Layout->setColumnMinimumWidth(0, 100);

  m_Layout->setColumnStretch(2, 1);
  m_Layout->setColumnMinimumWidth(1, 300);

  m_Layout->setRowStretch(0, 4);

  m_Layout->setRowStretch(1, 1);
  m_Layout->setRowMinimumHeight(1, 100);

  m_Layout->addWidget(m_LayerList.get(), 0, 0, 2, 1);
  m_Layout->addWidget(m_Viewer.get(), 0, 1);
  m_Layout->addWidget(m_Properties.get(), 0, 2);
  m_Layout->addWidget(m_Selector.get(), 1, 1, 1, 2);

  m_PropertyEditor = m_Properties->CreateWidget<PropertyEditor>();
  setLayout(m_Layout.get());
}

void MapEditor::ChangeLayerSelection(const MapEditorLayerSelection & layer, bool change_viewer_position)
{
  if (m_IgnoreSelectionChanges)
  {
    return;
  }

  m_Selector->GetTileSelector()->SetLayer(-1);
  m_Selector->GetEntitySelector()->SetLayer(-1);

  switch (layer.m_Type)
  {
  case MapEditorLayerItemType::kManualTileLayerParent:
  case MapEditorLayerItemType::kEntityLayerParent:
  case MapEditorLayerItemType::kVolumeParent:
  case MapEditorLayerItemType::kPathParent:
    ClearLayerSelection();
    break;
  case MapEditorLayerItemType::kManualTileLayer:
    m_PropertyEditor->LoadStruct(this, m_Map.m_ManualTileLayers[layer.m_Index],
      [this, index = layer.m_Index]() -> void * { return m_Map.m_ManualTileLayers.TryGet(index); }, true);

    m_Selector->GetTileSelector()->show();
    m_Selector->GetTileSelector()->SetLayer((int)layer.m_Index);
    m_Selector->GetEntitySelector()->Clear();
    m_Selector->GetEntitySelector()->hide();
    m_Selector->GetTileSelector()->LoadManualTileLayer(layer.m_Index);
    break;
  case MapEditorLayerItemType::kEntityLayer:

    m_Selector->GetTileSelector()->hide();
    m_Selector->GetTileSelector()->Clear();
    m_Selector->GetEntitySelector()->show();
    m_Selector->GetEntitySelector()->SetLayer((int)layer.m_Index);
    m_PropertyEditor->LoadStruct(this, m_Map.m_EntityLayers[layer.m_Index], 
      [this, index = layer.m_Index]() -> void * { return m_Map.m_EntityLayers.TryGet(index); }, true);
    break;
  case MapEditorLayerItemType::kEntity:

    m_Selector->GetTileSelector()->hide();
    m_Selector->GetTileSelector()->Clear();
    m_Selector->GetEntitySelector()->hide();
    m_Selector->GetEntitySelector()->Clear();
    
    m_PropertyEditor->LoadStruct(this, m_Map.m_EntityLayers[layer.m_Index].m_Entities[layer.m_SubIndex], 
      [this, index = layer.m_Index, subindex = layer.m_SubIndex]() -> void * 
      { 
        auto layer = m_Map.m_EntityLayers.TryGet(index); 
        auto entity = layer ? layer->m_Entities.TryGet(subindex) : nullptr; 
        return entity;
      }, true
    );

    if (change_viewer_position)
    {
      m_Viewer->ZoomToEntity(layer.m_Index, layer.m_SubIndex);
    }
    break;

  case MapEditorLayerItemType::kParalaxLayer:

    m_Selector->GetTileSelector()->hide();
    m_Selector->GetTileSelector()->Clear();
    m_Selector->GetEntitySelector()->hide();
    m_Selector->GetEntitySelector()->Clear();
    m_PropertyEditor->LoadStruct(this, m_Map.m_ParalaxLayers[layer.m_Index], 
      [this, index = layer.m_Index]() -> void * { return m_Map.m_ParalaxLayers.TryGet(index); }, true);
    break;

  case MapEditorLayerItemType::kCreateVolume:

    m_Selector->GetTileSelector()->hide();
    m_Selector->GetTileSelector()->Clear();
    m_Selector->GetEntitySelector()->hide();
    m_Selector->GetEntitySelector()->Clear();

    {
      auto property_data = GetProperyMetaData<RPolymorphic<VolumeDataBase, VolumeTypeDatabase, VolumeDataTypeInfo>>(GetPropertyFieldDatabase());
      m_PropertyEditor->LoadObject(this, property_data, false, [this]() -> void * { return &m_VolumeInitData; }, "");
    }

    break;

  case MapEditorLayerItemType::kVolume:

    m_Selector->GetTileSelector()->hide();
    m_Selector->GetTileSelector()->Clear();
    m_Selector->GetEntitySelector()->hide();
    m_Selector->GetEntitySelector()->Clear();

    m_PropertyEditor->LoadStruct(this, m_Map.m_Volumes[layer.m_Index],
      [this, index = layer.m_Index]() -> void * { return m_Map.m_Volumes.TryGet(index); }, true);

    if (change_viewer_position)
    {
      m_Viewer->ZoomToVolume(layer.m_Index);
    }
    break;

  case MapEditorLayerItemType::kCreatePath:

    m_Selector->GetTileSelector()->hide();
    m_Selector->GetTileSelector()->Clear();
    m_Selector->GetEntitySelector()->hide();
    m_Selector->GetEntitySelector()->Clear();

    {
      auto property_data = GetProperyMetaData<RPolymorphic<PathDataBase, PathTypeDatabase, PathDataTypeInfo>>(GetPropertyFieldDatabase());
      m_PropertyEditor->LoadObject(this, property_data, false, [this]() -> void * { return &m_PathInitData; }, "");
    }

    break;

  case MapEditorLayerItemType::kPath:

    m_Selector->GetTileSelector()->hide();
    m_Selector->GetTileSelector()->Clear();
    m_Selector->GetEntitySelector()->hide();
    m_Selector->GetEntitySelector()->Clear();

    m_PropertyEditor->LoadStruct(this, m_Map.m_Paths[layer.m_Index],
      [this, index = layer.m_Index]() -> void * { return m_Map.m_Paths.TryGet(index); }, true);

    if (change_viewer_position)
    {
      m_Viewer->ZoomToPath(layer.m_Index);
    }
    break;
  }

  m_Viewer->ChangeLayerSelection(layer);
  m_LayerList->ChangeLayerSelection(layer);

  switch (layer.m_Type)
  {
  case MapEditorLayerItemType::kManualTileLayer:
    m_Viewer->SetTool(MapEditorTool<MapEditorToolManualTileLayerSelect>{}, (int)layer.m_Index);
    break;
  case MapEditorLayerItemType::kEntity:
    m_EntityLayers.GetLayerManager(layer.m_Index)->SetSingleSelection(layer.m_SubIndex);
    m_Viewer->SetTool(MapEditorTool<MapEditorToolEntityLayerSelect>{}, (int)layer.m_Index);
    break;
  case MapEditorLayerItemType::kEntityLayer:
    m_Viewer->SetTool(MapEditorTool<MapEditorToolEntityLayerSelect>{}, (int)layer.m_Index);
    break;
  case MapEditorLayerItemType::kCreateVolume:
    m_Viewer->SetTool(MapEditorTool<MapEditorToolVolumeCreate>{});
    break;
  case MapEditorLayerItemType::kVolume:
    m_Viewer->SetTool(MapEditorTool<MapEditorToolVolumeEditor>{}, (int)layer.m_Index);
    break;
  case MapEditorLayerItemType::kCreatePath:
    m_Viewer->SetTool(MapEditorTool<MapEditorToolPathCreate>{});
    break;
  case MapEditorLayerItemType::kPath:
    m_Viewer->SetTool(MapEditorTool<MapEditorToolPathEditor>{}, (int)layer.m_Index);
    break;
  }
}

void MapEditor::ClearLayerSelection()
{
  if (m_IgnoreSelectionChanges)
  {
    return;
  }

  m_PropertyEditor->Unload();
  m_Viewer->ClearLayerSelection();
  m_LayerList->ClearLayerSelection();

  m_Selector->GetTileSelector()->Clear();
  m_Selector->GetTileSelector()->hide();
  m_Selector->GetEntitySelector()->Clear();
  m_Selector->GetEntitySelector()->hide();
}

MapEditorTileLayerManager & MapEditor::GetManualTileManager()
{
  return m_ManualTileLayers;
}

MapEditorEntityLayerManager & MapEditor::GetEntityManager()
{
  return m_EntityLayers;
}

MapEditorParalaxLayerManager & MapEditor::GetParalaxManager()
{
  return m_ParalaxLayers;
}

MapEditorVolumeManager & MapEditor::GetVolumeManager()
{
  return m_Volumes;
}

MapEditorPathManager & MapEditor::GetPathManager()
{
  return m_Paths;
}

MapEditorSelector & MapEditor::GetSelector()
{
  return *m_Selector.get();
}

MapEditorLayerList & MapEditor::GetLayerList()
{
  return *m_LayerList.get();
}

MapEditorViewer & MapEditor::GetViewer()
{
  return *m_Viewer.get();
}

void MapEditor::SelectManualTile(int layer_index, uint64_t frame_id)
{
  m_Selector->GetTileSelector()->SetSelectedTile(frame_id);
  m_Viewer->SetTool(MapEditorTool<MapEditorToolManualTileLayerDraw>{}, layer_index, frame_id);
}

void MapEditor::SetSelectedEntity(int layer_index, czstr entity_file)
{
  m_Selector->GetEntitySelector()->SetSelectEntity(entity_file);
  m_Viewer->SetTool(MapEditorTool<MapEditorToolEntityLayerDraw>{}, layer_index, entity_file);
}

void MapEditor::ClearPropertyPanel()
{
  m_PropertyEditor->Unload();
}

const RPolymorphic<VolumeDataBase, VolumeTypeDatabase, VolumeDataTypeInfo> & MapEditor::GetVolumeInitData() const
{
  return m_VolumeInitData;
}

void MapEditor::CreateNewVolume(const Box & box)
{
  MapVolume vol;
  vol.m_XStart = box.m_Start.x;
  vol.m_YStart = box.m_Start.y;
  vol.m_XEnd = box.m_End.x;
  vol.m_YEnd = box.m_End.y;
  vol.m_VolumeData = m_VolumeInitData;
  
  auto type_info = m_VolumeInitData.GetTypeInfo();
  if (type_info)
  {
    vol.m_Name = type_info->m_Name;
  }
  else
  {
    vol.m_Name = "Volume";
  }

  m_Map.m_Volumes.EmplaceBack(vol);
}

const RPolymorphic<PathDataBase, PathTypeDatabase, PathDataTypeInfo> & MapEditor::GetPathInitData() const
{
  return m_PathInitData;
}

void MapEditor::CreateNewPath(const Line & line)
{
  MapPath path;
  path.m_PathData = m_PathInitData;
 
  MapPathPoint start;
  start.m_X = line.m_Start.x;
  start.m_Y = line.m_Start.y;

  MapPathPoint end;
  end.m_X = line.m_End.x;
  end.m_Y = line.m_End.y;

  path.m_Points.EmplaceBack(start);
  path.m_Points.EmplaceBack(end);

  auto type_info = m_PathInitData.GetTypeInfo();
  if (type_info)
  {
    path.m_Name = type_info->m_Name;
  }
  else
  {
    path.m_Name = "Path";
  }

  m_Map.m_Paths.EmplaceBack(path);
}

void MapEditor::AboutToClose()
{
  m_IgnoreSelectionChanges = true;
}

REGISTER_EDITOR("Map", MapEditor, MapDef, ".map", "Maps");