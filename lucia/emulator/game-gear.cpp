struct GameGear : Emulator {
  GameGear();
  auto load() -> bool override;
  auto save() -> bool override;
  auto pak(ares::Node::Object) -> shared_pointer<vfs::directory> override;
  auto input(ares::Node::Input::Input) -> void override;
};

GameGear::GameGear() {
  manufacturer = "Sega";
  name = "Game Gear";

//note: disabled because the BIOS is not yet working for Game Gear
//firmware.append({"BIOS", "World"});

  { InputPort port{"Hardware"};

    InputDevice device{"Gamepad"};
    device.button("Up",    virtualPads[0].up);
    device.button("Down",  virtualPads[0].down);
    device.button("Left",  virtualPads[0].left);
    device.button("Right", virtualPads[0].right);
    device.button("1",     virtualPads[0].a);
    device.button("2",     virtualPads[0].b);
    device.button("Start", virtualPads[0].start);
    port.append(device);

    ports.append(port);
  }
}

auto GameGear::load() -> bool {
  game = mia::Medium::create("Game Gear");
  if(!game->load(Emulator::load(game, configuration.game))) return false;

  system = mia::System::create("Game Gear");
  if(!system->load()) return false;
//if(!system->load(firmware[0].location)) return false;

  if(!ares::MasterSystem::load(root, "[Sega] Game Gear")) return false;

  if(auto port = root->find<ares::Node::Port>("Cartridge Slot")) {
    port->allocate();
    port->connect();
  }

  return true;
}

auto GameGear::save() -> bool {
  root->save();
  system->save(system->location);
  game->save(game->location);
  return true;
}

auto GameGear::pak(ares::Node::Object node) -> shared_pointer<vfs::directory> {
  if(node->name() == "Game Gear") return system->pak;
  if(node->name() == "Game Gear Cartridge") return game->pak;
  return {};
}

auto GameGear::input(ares::Node::Input::Input input) -> void {
  auto name = input->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"   ) mapping = virtualPads[0].up;
  if(name == "Down" ) mapping = virtualPads[0].down;
  if(name == "Left" ) mapping = virtualPads[0].left;
  if(name == "Right") mapping = virtualPads[0].right;
  if(name == "1"    ) mapping = virtualPads[0].a;
  if(name == "2"    ) mapping = virtualPads[0].b;
  if(name == "Start") mapping = virtualPads[0].start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = input->cast<ares::Node::Input::Button>()) {
      button->setValue(value);
    }
  }
}
