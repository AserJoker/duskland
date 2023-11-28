#pragma once
#include "core/include/auto_release.hpp"
#include "core/include/object.hpp"
#include "entity_base.hpp"
#include "tui/include/graphic.hpp"
#include "tui/include/widget.hpp"
namespace duskland::game {
class system_entity : public core::object {
public:
  class renderer : public tui::widget {
  protected:
    void on_render(core::auto_release<tui::graphic> &g);
  };

private:
  std::vector<core::auto_release<entity_base>> _entites;
  core::auto_release<renderer> _renderer;

public:
  void initialize();
  void uninitialize();
  const std::vector<core::auto_release<entity_base>> &get_entities();
  const core::auto_release<renderer> &get_renderer();
  template <class T> core::auto_release<T> create_entity(auto... args) {
    core::auto_release<T> entity = new T(args...);
    _entites.push_back(entity);
    return entity;
  }
  void delete_entity(const core::auto_release<entity_base> &entity);
};
} // namespace duskland::game