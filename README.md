# CSE211_chess2 ♟️🚪

Custom Chess Game with Portals – Term Project for **Data Structures (CSE211)**  
Made by Berk with caffeine, long hours, and just a little help from ChatGPT (pls don't be mad at me).

---

## 📦 Features

- ♜ Fully functional standard chess pieces
- 🌀 Portal system with entry/exit points, cooldowns, and directional logic
- 🧠 Graph-based move validation using BFS/DFS
- 🗃️ Efficient HashMap-based board representation (`"x,y"` → Piece)
- ⏳ Stack-based undo system, queue-based portal cooldown tracking
- ⚙️ Completely configurable pieces and rules via JSON

---

## 🚀 Build & Run

```bash
make deps
make
./bin/chess_game ./data/chess_pieces.json
```

> `make run` also works if the config path is pre-defined.

---

## 📁 Project Structure

- `src/` → C++ source files
- `include/` → Header files
- `data/` → JSON configs (pieces, settings, portals)
- `bin/` → Compiled game binary
- `third_party/` → External dependencies (e.g., nlohmann/json)

---

## 🤝 Credits

Developed by [**Berk**](https://github.com/BerkSucksAtCSE)  
With emotional support from caffeine and the silent judgment of the terminal window.

---

## 🧪 Notes

This project does not play chess. It *lives* chess.
Handle with care and a sense of humor.
