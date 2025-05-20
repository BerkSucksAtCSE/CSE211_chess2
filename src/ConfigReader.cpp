#include "ConfigReader.hpp"
#include <fstream>
#include <iostream>

ConfigReader::ConfigReader() {}

bool ConfigReader::loadFromFile(const std::string &filePath) {
  try {
    std::ifstream file(filePath);
    if (!file.is_open()) {
      std::cerr << "Failed to open config file: " << filePath << std::endl;
      return false;
    }

    nlohmann::json jsonData;
    file >> jsonData;

    parseGameSettings(jsonData);
    parsePieces(jsonData);
    parseCustomPieces(jsonData);
    parsePortals(jsonData);

    return validateConfig();
  } catch (const std::exception &e) {
    std::cerr << "Error parsing config file: " << e.what() << std::endl;
    return false;
  }
}

bool ConfigReader::loadFromString(const std::string &jsonString) {
  try {
    nlohmann::json jsonData = nlohmann::json::parse(jsonString);

    parseGameSettings(jsonData);
    parsePieces(jsonData);
    parseCustomPieces(jsonData);
    parsePortals(jsonData);

    return validateConfig();
  } catch (const std::exception &e) {
    std::cerr << "Error parsing config string: " << e.what() << std::endl;
    return false;
  }
}

const GameConfig &ConfigReader::getConfig() const { return m_config; }

bool ConfigReader::validateConfig() {
  // Basic validation
  if (m_config.game_settings.name.empty()) {
    std::cerr << "Game name is missing" << std::endl;
    return false;
  }

  if (m_config.game_settings.board_size <= 0) {
    std::cerr << "Invalid board size" << std::endl;
    return false;
  }

  if (m_config.game_settings.turn_limit <= 0) {
    std::cerr << "Invalid turn limit" << std::endl;
    return false;
  }

  if (m_config.pieces.empty()) {
    std::cerr << "No pieces defined" << std::endl;
    return false;
  }

  // Check that each piece has a valid type and position
  for (const auto &piece : m_config.pieces) {
    if (piece.type.empty()) {
      std::cerr << "Piece is missing type" << std::endl;
      return false;
    }

    if (piece.positions.empty()) {
      std::cerr << "Piece " << piece.type << " has no positions" << std::endl;
      return false;
    }
  }

  // Validate custom pieces if any exist
  for (const auto &piece : m_config.custom_pieces) {
    if (piece.type.empty()) {
      std::cerr << "Custom piece is missing type" << std::endl;
      return false;
    }

    if (piece.positions.empty()) {
      std::cerr << "Custom piece " << piece.type << " has no positions"
                << std::endl;
      return false;
    }
  }

  // Validate portal positions are within board bounds
  for (const auto &portal : m_config.portals) {
    if (portal.id.empty()) {
      std::cerr << "Portal is missing ID" << std::endl;
      return false;
    }

    if (portal.positions.entry.x < 0 ||
        portal.positions.entry.x >= m_config.game_settings.board_size ||
        portal.positions.entry.y < 0 ||
        portal.positions.entry.y >= m_config.game_settings.board_size) {
      std::cerr << "Portal " << portal.id
                << " entry position is outside board bounds" << std::endl;
      return false;
    }

    if (portal.positions.exit.x < 0 ||
        portal.positions.exit.x >= m_config.game_settings.board_size ||
        portal.positions.exit.y < 0 ||
        portal.positions.exit.y >= m_config.game_settings.board_size) {
      std::cerr << "Portal " << portal.id
                << " exit position is outside board bounds" << std::endl;
      return false;
    }
  }

  return true;
}

void ConfigReader::parseGameSettings(const nlohmann::json &json) {
  if (json.contains("game_settings")) {
    const auto &settings = json["game_settings"];

    m_config.game_settings.name = settings.value("name", "Custom Chess");
    m_config.game_settings.board_size = settings.value("board_size", 8);
    m_config.game_settings.turn_limit = settings.value("turn_limit", 100);
  } else {
    // Default values if no settings specified
    m_config.game_settings.name = "Custom Chess";
    m_config.game_settings.board_size = 8;
    m_config.game_settings.turn_limit = 100;
  }
}

void ConfigReader::parseSpecialAbilities(const nlohmann::json &abilities,
                                         SpecialAbilities &specialAbilities) {
  if (abilities.is_object()) {
    // Parse standard abilities
    specialAbilities.castling = abilities.value("castling", false);
    specialAbilities.royal = abilities.value("royal", false);
    specialAbilities.jump_over = abilities.value("jump_over", false);
    specialAbilities.promotion = abilities.value("promotion", false);
    specialAbilities.en_passant = abilities.value("en_passant", false);

    // Parse any additional custom abilities
    for (auto it = abilities.begin(); it != abilities.end(); ++it) {
      const std::string &key = it.key();

      // Skip standard abilities
      if (key != "castling" && key != "royal" && key != "jump_over" &&
          key != "promotion" && key != "en_passant") {

        // Add to custom abilities map if it's a boolean value
        if (it.value().is_boolean()) {
          specialAbilities.custom_abilities[key] = it.value().get<bool>();
        }
      }
    }
  }
}

void ConfigReader::parsePieces(const nlohmann::json &json) {
  if (!json.contains("pieces") || !json["pieces"].is_array()) {
    return;
  }

  for (const auto &pieceJson : json["pieces"]) {
    PieceConfig piece;

    // Parse basic properties
    piece.type = pieceJson.value("type", "");
    piece.count = pieceJson.value("count", 0);

    // Parse positions
    if (pieceJson.contains("positions")) {
      const auto &positions = pieceJson["positions"];

      // Parse white piece positions
      if (positions.contains("white") && positions["white"].is_array()) {
        for (const auto &posJson : positions["white"]) {
          Position pos;
          pos.x = posJson.value("x", 0);
          pos.y = posJson.value("y", 0);
          piece.positions["white"].push_back(pos);
        }
      }

      // Parse black piece positions
      if (positions.contains("black") && positions["black"].is_array()) {
        for (const auto &posJson : positions["black"]) {
          Position pos;
          pos.x = posJson.value("x", 0);
          pos.y = posJson.value("y", 0);
          piece.positions["black"].push_back(pos);
        }
      }
    }

    // Parse movement
    if (pieceJson.contains("movement")) {
      const auto &movement = pieceJson["movement"];

      // Set all movement properties with default 0 if not specified
      piece.movement.forward = movement.value("forward", 0);
      piece.movement.sideways = movement.value("sideways", 0);
      piece.movement.diagonal = movement.value("diagonal", 0);
      piece.movement.l_shape = movement.value("l_shape", false);
      piece.movement.diagonal_capture = movement.value("diagonal_capture", 0);
      piece.movement.first_move_forward =
          movement.value("first_move_forward", 0);
    }

    // Parse special abilities
    if (pieceJson.contains("special_abilities")) {
      parseSpecialAbilities(pieceJson["special_abilities"],
                            piece.special_abilities);
    }

    m_config.pieces.push_back(piece);
  }
}

void ConfigReader::parseCustomPieces(const nlohmann::json &json) {
  if (!json.contains("custom_pieces") || !json["custom_pieces"].is_array()) {
    return;
  }

  for (const auto &pieceJson : json["custom_pieces"]) {
    PieceConfig piece;

    // Parse basic properties
    piece.type = pieceJson.value("type", "");
    piece.count = pieceJson.value("count", 0);

    // Parse positions
    if (pieceJson.contains("positions")) {
      const auto &positions = pieceJson["positions"];

      // Parse white piece positions
      if (positions.contains("white") && positions["white"].is_array()) {
        for (const auto &posJson : positions["white"]) {
          Position pos;
          pos.x = posJson.value("x", 0);
          pos.y = posJson.value("y", 0);
          piece.positions["white"].push_back(pos);
        }
      }

      // Parse black piece positions
      if (positions.contains("black") && positions["black"].is_array()) {
        for (const auto &posJson : positions["black"]) {
          Position pos;
          pos.x = posJson.value("x", 0);
          pos.y = posJson.value("y", 0);
          piece.positions["black"].push_back(pos);
        }
      }
    }

    // Parse movement
    if (pieceJson.contains("movement")) {
      const auto &movement = pieceJson["movement"];

      // Set all movement properties with default 0 if not specified
      piece.movement.forward = movement.value("forward", 0);
      piece.movement.sideways = movement.value("sideways", 0);
      piece.movement.diagonal = movement.value("diagonal", 0);
      piece.movement.l_shape = movement.value("l_shape", false);
      piece.movement.diagonal_capture = movement.value("diagonal_capture", 0);
      piece.movement.first_move_forward =
          movement.value("first_move_forward", 0);
    }

    // Parse special abilities
    if (pieceJson.contains("special_abilities")) {
      parseSpecialAbilities(pieceJson["special_abilities"],
                            piece.special_abilities);
    }

    m_config.custom_pieces.push_back(piece);
  }
}

void ConfigReader::parsePortals(const nlohmann::json &json) {
  if (!json.contains("portals") || !json["portals"].is_array()) {
    return;
  }

  for (const auto &portalJson : json["portals"]) {
    PortalConfig portal;

    // Parse basic properties
    portal.type = portalJson.value("type", "Portal");
    portal.id = portalJson.value("id", "");

    // Parse positions
    if (portalJson.contains("positions")) {
      const auto &positions = portalJson["positions"];

      // Parse entry position
      if (positions.contains("entry")) {
        portal.positions.entry.x = positions["entry"].value("x", 0);
        portal.positions.entry.y = positions["entry"].value("y", 0);
      }

      // Parse exit position
      if (positions.contains("exit")) {
        portal.positions.exit.x = positions["exit"].value("x", 0);
        portal.positions.exit.y = positions["exit"].value("y", 0);
      }
    }

    // Parse properties
    if (portalJson.contains("properties")) {
      const auto &properties = portalJson["properties"];

      portal.properties.preserve_direction =
          properties.value("preserve_direction", true);
      portal.properties.cooldown = properties.value("cooldown", 0);

      // Parse allowed colors
      if (properties.contains("allowed_colors") &&
          properties["allowed_colors"].is_array()) {
        for (const auto &color : properties["allowed_colors"]) {
          portal.properties.allowed_colors.push_back(color);
        }
      } else {
        // Default to allowing both colors if not specified
        portal.properties.allowed_colors = {"white", "black"};
      }
    }

    m_config.portals.push_back(portal);
  }
}
