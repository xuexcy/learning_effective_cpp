/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/24 17:25:05
# Desc   : 条款 23：宁以 non-member、non-friend 替换 member 函数(Prefer non-member non-friend functions to member functions)
########################################################################
*/

namespace browser {
class WebBrowser {
public:
  void clearCache() {
    // ...
  }
  void clearHistory() {
    // ...
  }
  void removeCookies() {
    // ...
  }
  void clearEverything() {
    clearCache();
    clearHistory();
    removeCookies();
  }
};  // class WebBrowser

// part-1
// 越少的代码可以看到数据（可以访问它），我们就能越自由地改变数据
// clearEverything() 可以访问 class WebBrowser 的私有成员（虽然函数里面没有访问私有成员，但并不代表未来更改代码时不会访问）
//   这样，class WebBrowser 中有 4 个函数可以访问（看到）私有成员
// clearBrowser() 不会访问 class WebBrowser 的私有成员
//   这样，class WebBrowser 中有 3 个函数可以访问（看到）私有成员
void clearBrowser(WebBrowser& browser) {
  browser.clearCache();
  browser.clearHistory();
  browser.removeCookies();
}
}  // namespace browser

// part-2
// 可以通过将 clearBrowser 和 class WebBrowser 放在同一个 namespace，方便管理
// 也可以放到一个功能类的 static 函数中
class BrowserUtils {
public:
  static void clearBrowser(browser::WebBrowser& browser) {
    browser.clearCache();
    browser.clearHistory();
    browser.removeCookies();
  }
};  // class BrowserUtil

// part-3
// 将所有便利函数放在多个头文件内但隶属同一个 namespace，意味着客户可以轻松扩展这一组便利函数
// 对于非核心的功能（不是每个用户都要使用的功能），比如书签、cookies 相关的便利函数，可以分类将其放入不同的 header 中
// bookmark.h，cookies.h，这样用户在使用其中一个功能时，可以减少编译依赖

// part-4
// 用户也可以在这个 namespace 下添加便利函数


void run() {
  browser::WebBrowser browser;
  browser.clearEverything();
  browser::clearBrowser(browser);
  BrowserUtils::clearBrowser(browser);
}
int main() {

  return 0;
}

