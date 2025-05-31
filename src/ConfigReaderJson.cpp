//burayi chatgpt yapti cunku kodu linklemede sorun yasiyorum
#include "ConfigReader.hpp"

// Position
void from_json(const nlohmann::json& j, Position& p) {
    p.x = j.at("x").get<int>();
    p.y = j.at("y").get<int>();
}

// Movement
void from_json(const nlohmann::json& j, Movement& m) {
    m.forward = j.value("forward", 0);
    m.sideways = j.value("sideways", 0);
    m.diagonal = j.value("diagonal", 0);
    m.l_shape = j.value("l_shape", false);
    m.diagonal_capture = j.value("diagonal_capture", 0);
    m.first_move_forward = j.value("first_move_forward", 0);
}

// SpecialAbilities
void from_json(const nlohmann::json& j, SpecialAbilities& s) {
    s.castling = j.value("castling", false);
    s.royal = j.value("royal", false);
    s.jump_over = j.value("jump_over", false);
    s.promotion = j.value("promotion", false);
    s.en_passant = j.value("en_passant", false);
    s.custom_abilities = j.value("custom_abilities", std::unordered_map<std::string, bool>{});
}

// PieceConfig
void from_json(const nlohmann::json& j, PieceConfig& p) {
    p.type = j.at("type").get<std::string>();
    p.positions = j.value("positions", std::unordered_map<std::string, std::vector<Position>>{});
    p.movement = j.at("movement").get<Movement>();
    p.special_abilities = j.at("special_abilities").get<SpecialAbilities>();
    p.count = j.value("count", 1);
}

// PortalProperties
void from_json(const nlohmann::json& j, PortalProperties& p) {
    p.preserve_direction = j.value("preserve_direction", false);
    p.allowed_colors = j.value("allowed_colors", std::vector<std::string>{});
    p.cooldown = j.value("cooldown", 0);
}

// PortalConfig
void from_json(const nlohmann::json& j, PortalConfig& p) {
    p.type = j.at("type").get<std::string>();
    p.id = j.at("id").get<std::string>();
    p.positions.entry = j.at("positions").at("entry").get<Position>();
    p.positions.exit = j.at("positions").at("exit").get<Position>();
    p.properties = j.at("properties").get<PortalProperties>();
}

// GameConfig
void from_json(const nlohmann::json& j, GameConfig& g) {
    auto settings = j.at("game_settings");
    g.game_settings.name = settings.value("name", "");
    g.game_settings.board_size = settings.value("board_size", 8);
    g.game_settings.turn_limit = settings.value("turn_limit", 100);
    
    g.pieces = j.value("pieces", std::vector<PieceConfig>{});
    g.custom_pieces = j.value("custom_pieces", std::vector<PieceConfig>{});
    g.portals = j.value("portals", std::vector<PortalConfig>{});
}
