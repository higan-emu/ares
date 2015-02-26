#ifndef NALL_WINDOWS_REGISTRY_HPP
#define NALL_WINDOWS_REGISTRY_HPP

#include <nall/platform.hpp>
#include <nall/string.hpp>

#include <shlwapi.h>
#undef interface
#ifndef KEY_WOW64_64KEY
  #define KEY_WOW64_64KEY 0x0100
#endif
#ifndef KEY_WOW64_32KEY
  #define KEY_WOW64_32KEY 0x0200
#endif

#ifndef NWR_FLAGS
  #define NWR_FLAGS KEY_WOW64_64KEY
#endif

#ifndef NWR_SIZE
  #define NWR_SIZE 4096
#endif

namespace nall {

struct registry {
  static bool exists(const string& name) {
    lstring part = name.split("/");
    HKEY handle, rootKey = root(part.take(0));
    string node = part.take();
    string path = part.merge("\\");
    if(RegOpenKeyExW(rootKey, utf16_t(path), 0, NWR_FLAGS | KEY_READ, &handle) == ERROR_SUCCESS) {
      wchar_t data[NWR_SIZE] = L"";
      DWORD size = NWR_SIZE * sizeof(wchar_t);
      LONG result = RegQueryValueExW(handle, utf16_t(node), nullptr, nullptr, (LPBYTE)&data, (LPDWORD)&size);
      RegCloseKey(handle);
      if(result == ERROR_SUCCESS) return true;
    }
    return false;
  }

  static string read(const string& name) {
    lstring part = name.split("/");
    HKEY handle, rootKey = root(part.take(0));
    string node = part.take();
    string path = part.merge("\\");
    if(RegOpenKeyExW(rootKey, utf16_t(path), 0, NWR_FLAGS | KEY_READ, &handle) == ERROR_SUCCESS) {
      wchar_t data[NWR_SIZE] = L"";
      DWORD size = NWR_SIZE * sizeof(wchar_t);
      LONG result = RegQueryValueExW(handle, utf16_t(node), nullptr, nullptr, (LPBYTE)&data, (LPDWORD)&size);
      RegCloseKey(handle);
      if(result == ERROR_SUCCESS) return (const char*)utf8_t(data);
    }
    return "";
  }

  static void write(const string& name, const string& data = "") {
    lstring part = name.split("/");
    HKEY handle, rootKey = root(part.take(0));
    string node = part.take(), path;
    DWORD disposition;
    for(unsigned n = 0; n < part.size(); n++) {
      path.append(part[n]);
      if(RegCreateKeyExW(rootKey, utf16_t(path), 0, nullptr, 0, NWR_FLAGS | KEY_ALL_ACCESS, nullptr, &handle, &disposition) == ERROR_SUCCESS) {
        if(n == part.size() - 1) {
          RegSetValueExW(handle, utf16_t(node), 0, REG_SZ, (BYTE*)(wchar_t*)utf16_t(data), (data.length() + 1) * sizeof(wchar_t));
        }
        RegCloseKey(handle);
      }
      path.append("\\");
    }
  }

  static bool remove(const string& name) {
    lstring part = name.split("/");
    HKEY rootKey = root(part.take(0));
    string node = part.take();
    string path = part.merge("\\");
    if(node.empty()) return SHDeleteKeyW(rootKey, utf16_t(path)) == ERROR_SUCCESS;
    return SHDeleteValueW(rootKey, utf16_t(path), utf16_t(node)) == ERROR_SUCCESS;
  }

  static lstring contents(const string& name) {
    lstring part = name.split("/"), result;
    HKEY handle, rootKey = root(part.take(0));
    part.remove();
    string path = part.merge("\\");
    if(RegOpenKeyExW(rootKey, utf16_t(path), 0, NWR_FLAGS | KEY_READ, &handle) == ERROR_SUCCESS) {
      DWORD folders, nodes;
      RegQueryInfoKey(handle, nullptr, nullptr, nullptr, &folders, nullptr, nullptr, &nodes, nullptr, nullptr, nullptr, nullptr);
      for(unsigned n = 0; n < folders; n++) {
        wchar_t name[NWR_SIZE] = L"";
        DWORD size = NWR_SIZE * sizeof(wchar_t);
        RegEnumKeyEx(handle, n, (wchar_t*)&name, &size, nullptr, nullptr, nullptr, nullptr);
        result.append(string{(const char*)utf8_t(name), "/"});
      }
      for(unsigned n = 0; n < nodes; n++) {
        wchar_t name[NWR_SIZE] = L"";
        DWORD size = NWR_SIZE * sizeof(wchar_t);
        RegEnumValueW(handle, n, (wchar_t*)&name, &size, nullptr, nullptr, nullptr, nullptr);
        result.append((const char*)utf8_t(name));
      }
      RegCloseKey(handle);
    }
    return result;
  }

private:
  static HKEY root(const string& name) {
    if(name == "HKCR") return HKEY_CLASSES_ROOT;
    if(name == "HKCC") return HKEY_CURRENT_CONFIG;
    if(name == "HKCU") return HKEY_CURRENT_USER;
    if(name == "HKLM") return HKEY_LOCAL_MACHINE;
    if(name == "HKU" ) return HKEY_USERS;
    return nullptr;
  }
};

}

#endif
