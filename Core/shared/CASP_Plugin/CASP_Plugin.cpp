#include "CASP_Plugin.h"

unordered_map<string, CASP_Plugin*> plugins;

string RegisterPlugin(string id, CASP_Plugin* plugin) {
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    plugins[id] = plugin;

    return id;
}

CASP_Plugin* GetModule(string id) {
    id = Helpers::toLower(id);
    CASP_Plugin* plugin = NULL;
    if (ModuleExists(id)) {
        plugin = plugins[id];
    } else {
        throw "Module '" + id + "' does not exist.";
    }
    return plugin;
}

bool ModuleExists(string id) {
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    CASP_Plugin* p = plugins[id];
    if (p == NULL) {
        plugins.erase(id);
        return false;
    }
    return true;
}