#pragma once
#include "widget.hpp"
#include <unordered_map>
namespace duskland::tui {
class windows : public widget {
public:
  struct node {
    node *first;
    node *second;
    enum { VERTICAL, HORIZONTAL } direction;
    int32_t offset;
    std::string identity;
    std::string key;
    node() : first(nullptr), second(nullptr), direction(VERTICAL), offset(0) {}
    ~node() {
      if (first) {
        delete first;
      }
      if (second) {
        delete second;
      }
    }
  };

private:
  node *_root;
  std::unordered_map<std::string, core::auto_release<widget>> _named_windows;
  std::unordered_map<std::string, core::auto_release<widget>> _keyed_windows;
  struct graph {
    graph *left;
    graph *top;
    graph *right;
    graph *bottom;
    util::point pos;
    graph() : left(nullptr), top(nullptr), right(nullptr), bottom(nullptr) {}
  };
  std::unordered_map<std::string, graph *> _nodes;

private:
  void process_node(
      node *node, util::rect rc,
      const std::unordered_map<std::string, core::auto_release<widget>> &cache);
  void process_edge(node *node);
  void relayout();

protected:
  void on_render(core::auto_release<graphic> &g) override;
  bool on_input(const util::key &key) override;

public:
  windows();
  ~windows() override;
  void set_root(node *root);
  node *get_root();
  core::auto_release<widget> get_window(const std::string &identity);
};
} // namespace duskland::tui