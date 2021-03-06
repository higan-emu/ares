#if defined(Hiro_TreeView)

namespace hiro {

struct pTreeView : pWidget {
  Declare(TreeView, Widget)

  auto append(sTreeViewItem item) -> void;
  auto remove(sTreeViewItem item) -> void;
  auto setActivation(Mouse::Click activation) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setFocused() -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setGeometry(Geometry geometry) -> void override;
};

}

#endif
