
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010 Francois Beaune
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// Interface header.
#include "surfaceshadercollectionitem.h"

// appleseed.studio headers.
#include "mainwindow/project/entitybrowser.h"
#include "mainwindow/project/entityeditorwindow.h"
#include "mainwindow/project/multimodelentityeditorformfactory.h"
#include "mainwindow/project/multimodelentityitem.h"
#include "mainwindow/project/projectbuilder.h"
#include "mainwindow/project/tools.h"

// appleseed.foundation headers.
#include "foundation/utility/uid.h"

// Qt headers.
#include <QMenu>

// Standard headers.
#include <memory>

using namespace foundation;
using namespace renderer;
using namespace std;

namespace appleseed {
namespace studio {

namespace
{
    const UniqueID g_class_uid = new_guid();
}

SurfaceShaderCollectionItem::SurfaceShaderCollectionItem(
    Assembly&               assembly,
    SurfaceShaderContainer& surface_shaders,
    ProjectBuilder&         project_builder)
  : CollectionItemBase(g_class_uid, "Surface Shaders")
  , m_assembly(assembly)
  , m_project_builder(project_builder)
{
    add_items(surface_shaders);
}

QMenu* SurfaceShaderCollectionItem::get_single_item_context_menu() const
{
    QMenu* menu = new QMenu(treeWidget());
    menu->addAction("Create Surface Shader...", this, SLOT(slot_create_surface_shader()));
    return menu;
}

void SurfaceShaderCollectionItem::add_item(SurfaceShader& surface_shader)
{
    addChild(
        new MultiModelEntityItem<SurfaceShader, Assembly, SurfaceShaderFactoryRegistrar>(
            m_assembly,
            surface_shader,
            m_registrar,
            m_project_builder));
}

void SurfaceShaderCollectionItem::slot_create_surface_shader()
{
    auto_ptr<EntityEditorWindow::IFormFactory> form_factory(
        new MultiModelEntityEditorFormFactory<SurfaceShaderFactoryRegistrar>(
            m_registrar,
            get_name_suggestion("surface_shader", m_assembly.surface_shaders())));

    auto_ptr<EntityEditorWindow::IEntityBrowser> entity_browser(
        new EntityBrowser<Assembly>(m_assembly));

    open_entity_editor(
        treeWidget(),
        "Create Surface Shader",
        form_factory,
        entity_browser,
        this,
        SLOT(slot_create_surface_shader_accepted(foundation::Dictionary)));
}

void SurfaceShaderCollectionItem::slot_create_surface_shader_accepted(Dictionary values)
{
    catch_entity_creation_errors(
        &SurfaceShaderCollectionItem::create_surface_shader,
        values,
        "Surface Shader");
}

void SurfaceShaderCollectionItem::create_surface_shader(const Dictionary& values)
{
    m_project_builder.insert_surface_shader(m_assembly, values);

    qobject_cast<QWidget*>(sender())->close();
}

}   // namespace studio
}   // namespace appleseed
