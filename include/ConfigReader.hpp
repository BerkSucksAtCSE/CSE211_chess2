#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

// Forward declarations
struct Position;
struct Movement;
struct SpecialAbilities;
struct PieceConfig;
struct PortalProperties;
struct PortalConfig;
struct GameConfig;

// Position on the chess board
struct Position {
  int x;
  int y;
};

// Movement capabilities for chess pieces
struct Movement {
  int forward = 0;
  int sideways = 0;
  int diagonal = 0;
  bool l_shape = false;
  int diagonal_capture = 0;
  int first_move_forward = 0;
};

// Special abilities for chess pieces
struct SpecialAbilities {
  bool castling = false;
  bool royal = false;
  bool jump_over = false;
  bool promotion = false;
  bool en_passant = false;
  // Additional custom abilities are also welcome
  std::unordered_map<std::string, bool> custom_abilities;
};

// Configuration for a chess piece
struct PieceConfig {
  std::string type;
  std::unordered_map<std::string, std::vector<Position>> positions;
  Movement movement;
  SpecialAbilities special_abilities;
  int count;
};

// Properties for portals
struct PortalProperties {
  bool preserve_direction;
  std::vector<std::string> allowed_colors;
  int cooldown;
};

// Configuration for a portal
struct PortalConfig {
  std::string type;
  std::string id;
  struct {
    Position entry;
    Position exit;
  } positions;
  PortalProperties properties;
};

// Game configuration
struct GameConfig {
  struct {
    std::string name;
    int board_size;
    int turn_limit;
  } game_settings;

  std::vector<PieceConfig> pieces;
  std::vector<PieceConfig> custom_pieces;
  std::vector<PortalConfig> portals;
};

class ConfigReader {
public:
  // Constructor
  ConfigReader();

  // Load configuration from a file
  bool loadFromFile(const std::string &filePath);

  // Load configuration from a JSON string
  bool loadFromString(const std::string &jsonString);

  // Get the parsed configuration
  const GameConfig &getConfig() const;

  // Validate the configuration
  bool validateConfig();

private:
  GameConfig m_config;

  // Parse game settings from JSON
  void parseGameSettings(const nlohmann::json &json);

  // Parse pieces from JSON
  void parsePieces(const nlohmann::json &json);

  // Parse custom pieces from JSON
  void parseCustomPieces(const nlohmann::json &json);

  // Parse portals from JSON
  void parsePortals(const nlohmann::json &json);

  // Parse special abilities from JSON
  void parseSpecialAbilities(const nlohmann::json &abilities,
                             SpecialAbilities &specialAbilities);
};
