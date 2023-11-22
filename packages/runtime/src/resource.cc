#include "../include/resource.hpp"
#include <filesystem>
#include <fstream>
using namespace duskland::runtime;
void resource::load(const std::string &root) {
  if (std::filesystem::exists(root)) {
    for (auto &item : std::filesystem::directory_iterator(root)) {
      load_resource(std::filesystem::absolute(item.path()).string(),
                    {item.path().filename().string()});
    }
  }
}
void resource::load_resource(const std::string &path,
                             const std::vector<std::string> &parts) {
  for (auto &item : std::filesystem::directory_iterator(path)) {
    if (item.is_directory()) {
      auto current = parts;
      current.push_back(item.path().filename().string());
      load_resource(item.path().string(), current);
    } else {
      std::string name;
      for (auto &part : parts) {
        name += part + ".";
      }
      auto filename = item.path().filename().string();
      if (item.path().has_extension()) {
        auto extension = item.path().extension();
        filename =
            filename.substr(0, filename.length() - extension.string().length());
      }
      name += filename;
      _resources[name] = item.path();
    }
  }
}
std::vector<char> resource::query(const std::string &name) {
  if (_resources.contains(name)) {
    std::ifstream file(_resources.at(name));
    if (file.is_open()) {
      std::vector<char> result((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
      file.close();
      return result;
    }
  }
  return {};
}