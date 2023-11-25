#include "../include/windows.hpp"
#define FMT_HEADER_ONLY
#include <fmt/format.h>
using namespace duskland::tui;
using namespace duskland;
windows::windows() : _root(nullptr) {
  _root = new node();
  _root->first = nullptr;
  _root->second = nullptr;
  _root->key = "";
  _root->identity = "";
  get_attribute().position = style::ABSOLUTE;
  get_attribute().xoverflow = style::FIXED;
  get_attribute().yoverflow = style::FIXED;
  get_attribute().size.width = getmaxx(stdscr);
  get_attribute().size.height = getmaxy(stdscr);
  get_attribute().border_left = true;
  get_attribute().border_right = true;
  get_attribute().border_top = true;
  get_attribute().border_bottom = true;
  get_attribute().offset.x = 0;
  get_attribute().offset.y = 0;
  request_update();
}
windows::node *windows::get_root() { return _root; }
void windows::process_node(
    node *node, util::rect rc,
    const std::unordered_map<std::string, core::auto_release<widget>> &cache) {
  if (node->first) {
    util::rect first_rc = {0, 0, 0, 0};
    util::rect second_rc = {0, 0, 0, 0};
    if (node->direction == node::VERTICAL) {
      auto width = node->offset;
      if (width < 0) {
        width += rc.width;
      }
      first_rc.x = rc.x;
      first_rc.y = rc.y;
      first_rc.width = width;
      first_rc.height = rc.height;
      second_rc.x = rc.x + first_rc.width + 1;
      second_rc.y = rc.y;
      second_rc.width = rc.width - first_rc.width - 1;
      second_rc.height = rc.height;
    } else {
      auto height = node->offset;
      if (height < 0) {
        height += rc.height;
      }
      first_rc.x = rc.x;
      first_rc.y = rc.y;
      first_rc.width = rc.width;
      first_rc.height = height;
      second_rc.x = rc.x;
      second_rc.y = rc.y + height + 1;
      second_rc.width = rc.width;
      second_rc.height = rc.height - first_rc.height - 1;
    }
    process_node(node->first, first_rc, cache);
    process_node(node->second, second_rc, cache);
  } else {
    widget *w = nullptr;
    if (!node->key.empty()) {
      if (cache.contains(node->key)) {
        w = cache.at(node->key).get();
      } else {
        w = new widget();
      }
      _keyed_windows[node->key] = w;
    }
    if (!w) {
      w = new widget();
    }
    w->get_attribute().offset.x = rc.x;
    w->get_attribute().offset.y = rc.y;
    w->get_attribute().size.width = rc.width;
    w->get_attribute().size.height = rc.height;
    add_child(w);
    if (!node->identity.empty()) {
      _named_windows[node->identity] = w;
    }
    util::point lt, lb, rt, rb;
    lt.x = rc.x - 1;
    lt.y = rc.y - 1;
    lb.x = rc.x - 1;
    lb.y = rc.y + rc.height;
    rt.x = rc.x + rc.width;
    rt.y = rc.y - 1;
    rb.x = rc.x + rc.width;
    rb.y = rc.y + rc.height;
    auto s_lt = fmt::format("{}.{}", lt.x, lt.y);
    auto s_lb = fmt::format("{}.{}", lb.x, lb.y);
    auto s_rt = fmt::format("{}.{}", rt.x, rt.y);
    auto s_rb = fmt::format("{}.{}", rb.x, rb.y);
    graph *g_lt = nullptr;
    graph *g_lb = nullptr;
    graph *g_rt = nullptr;
    graph *g_rb = nullptr;
    if (_nodes.contains(s_lt)) {
      g_lt = _nodes.at(s_lt);
    } else {
      g_lt = new graph();
      _nodes[s_lt] = g_lt;
      g_lt->pos = lt;
    }
    if (_nodes.contains(s_lb)) {
      g_lb = _nodes.at(s_lb);
    } else {
      g_lb = new graph();
      _nodes[s_lb] = g_lb;
      g_lb->pos = lb;
    }
    if (_nodes.contains(s_rt)) {
      g_rt = _nodes.at(s_rt);
    } else {
      g_rt = new graph();
      _nodes[s_rt] = g_rt;
      g_rt->pos = rt;
    }
    if (_nodes.contains(s_rb)) {
      g_rb = _nodes.at(s_rb);
    } else {
      g_rb = new graph();
      _nodes[s_rb] = g_rb;
      g_rb->pos = rb;
    }
    g_lt->right = g_rt;
    g_rt->left = g_lt;
    g_lt->bottom = g_lb;
    g_lb->top = g_lt;

    g_rt->bottom = g_rb;
    g_rb->top = g_rt;

    g_lb->right = g_rb;
    g_rb->left = g_lb;
  }
}
void windows::set_root(node *root) {
  if (_root && _root != root) {
    delete _root;
  }
  _root = root;
  relayout();
}

bool windows::on_input(const util::key &key) {
  if (key.name == "<resize>") {
    get_attribute().position = style::ABSOLUTE;
    get_attribute().xoverflow = style::FIXED;
    get_attribute().yoverflow = style::FIXED;
    get_attribute().size.width = getmaxx(stdscr);
    get_attribute().size.height = getmaxy(stdscr);
    get_attribute().border_left = true;
    get_attribute().border_right = true;
    get_attribute().border_top = true;
    get_attribute().border_bottom = true;
    get_attribute().offset.x = 0;
    get_attribute().offset.y = 0;
    auto active = get_active();
    relayout();
    set_active(active);
  }
  return widget::on_input(key);
}
core::auto_release<widget> windows::get_window(const std::string &identity) {
  if (_named_windows.contains(identity)) {
    return _named_windows.at(identity);
  }
  return nullptr;
}
void windows::relayout() {
  _nodes.clear();
  set_active(nullptr);
  get_children().clear();
  auto &attr = get_attribute();
  _named_windows.clear();
  auto cache = _keyed_windows;
  _keyed_windows.clear();
  process_node(_root, {0, 0, attr.size.width, attr.size.height}, cache);
  request_update();
}
void windows::on_render(core::auto_release<graphic> &g) {
  g->set_attr("active");
  for (auto &[_, node] : _nodes) {
    if (node->right) {
      for (auto x = node->pos.x + 1; x < node->right->pos.x; x++) {
        g->draw(x, node->pos.y, g->get_brush()->query_symbol("box.top"));
      }
    }
    if (node->bottom) {
      for (auto y = node->pos.y + 1; y < node->bottom->pos.y; y++) {
        g->draw(node->pos.x, y, g->get_brush()->query_symbol("box.left"));
      }
    }
    for (auto &[_, node] : _nodes) {
      if (node->left && node->right && node->top && node->bottom) {
        g->draw(node->pos.x, node->pos.y,
                g->get_brush()->query_symbol("box.left_right_top_bottom"));
      }
      if (!node->left && node->right && node->top && node->bottom) {
        g->draw(node->pos.x, node->pos.y,
                g->get_brush()->query_symbol("box.right_top_bottom"));
      }
      if (node->left && !node->right && node->top && node->bottom) {
        g->draw(node->pos.x, node->pos.y,
                g->get_brush()->query_symbol("box.left_top_bottom"));
      }
      if (node->left && node->right && !node->top && node->bottom) {
        g->draw(node->pos.x, node->pos.y,
                g->get_brush()->query_symbol("box.left_right_bottom"));
      }
      if (node->left && node->right && node->top && !node->bottom) {
        g->draw(node->pos.x, node->pos.y,
                g->get_brush()->query_symbol("box.left_right_top"));
      }
      if (!node->left && node->right && !node->top && node->bottom) {
        g->draw(node->pos.x, node->pos.y,
                g->get_brush()->query_symbol("box.right_bottom"));
      }
      if (!node->left && node->right && node->top && !node->bottom) {
        g->draw(node->pos.x, node->pos.y,
                g->get_brush()->query_symbol("box.right_top"));
      }
      if (node->left && !node->right && !node->top && node->bottom) {
        g->draw(node->pos.x, node->pos.y,
                g->get_brush()->query_symbol("box.left_bottom"));
      }
      if (node->left && !node->right && node->top && !node->bottom) {
        g->draw(node->pos.x, node->pos.y,
                g->get_brush()->query_symbol("box.left_top"));
      }
    }
  }
}

void windows::process_edge(windows::node *node) {}
windows::~windows() {
  if (_root) {
    delete _root;
    _root = nullptr;
  }
  for (auto &[_, n] : _nodes) {
    delete n;
  }
}