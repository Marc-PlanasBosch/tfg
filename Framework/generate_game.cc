#include "Core/GameDefinition.hh"
#include <iostream>
#include <fstream>
#include <string>

/**
 * Generador d'executables de joc basat en configuració
 * Llegeix un game_def.cnf i genera un executable específic del joc
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <game_def.cnf>" << std::endl;
        return 1;
    }
    
    std::string config_file = argv[1];
    
    // 1. Llegir configuració del joc
    auto game_def = std::make_shared<GameDefinition>();
    if (!game_def->loadFromFile(config_file)) {
        std::cerr << "Error carregant la configuració: " << config_file << std::endl;
        return 1;
    }
    
    std::cout << "Generant executable per: " << game_def->getGameName() << " v" << game_def->getVersion() << std::endl;
    
    // 2. Generar Main.cc específic del joc
    std::string game_name = game_def->getGameName();
    std::string main_file = game_name + "_main.cc";
    
    std::ofstream main_out(main_file);
    if (!main_out.is_open()) {
        std::cerr << "Error creant " << main_file << std::endl;
        return 1;
    }
    
    // Generar codi C++ per al joc específic
    main_out << "#include \"../Core/Game.hh\"" << std::endl;
    main_out << "#include \"../Core/Player.hh\"" << std::endl;
    main_out << "#include \"../Core/Board.hh\"" << std::endl;
    main_out << "#include \"../Core/Action.hh\"" << std::endl;
    main_out << "#include <iostream>" << std::endl;
    main_out << "#include <memory>" << std::endl;
    main_out << "#include <vector>" << std::endl;
    main_out << "#include <string>" << std::endl;
    main_out << std::endl;
    
    main_out << "/**" << std::endl;
    main_out << " * Executable generat automàticament per " << game_name << std::endl;
    main_out << " * Basat en " << config_file << std::endl;
    main_out << " */" << std::endl;
    main_out << "int main(int argc, char* argv[]) {" << std::endl;
    main_out << "    if (argc < 2) {" << std::endl;
    main_out << "        std::cerr << \"Usage: \" << argv[0] << \" <map_file> [player1] [player2] [player3] [player4]\" << std::endl;" << std::endl;
    main_out << "        return 1;" << std::endl;
    main_out << "    }" << std::endl;
    main_out << std::endl;
    
    main_out << "    std::string map_file = argv[1];" << std::endl;
    main_out << "    std::vector<std::string> player_names;" << std::endl;
    main_out << "    for (int i = 2; i < argc && i < 6; ++i) {" << std::endl;
    main_out << "        player_names.push_back(argv[i]);" << std::endl;
    main_out << "    }" << std::endl;
    main_out << "    while (player_names.size() < " << game_def->getMaxPlayers() << ") {" << std::endl;
    main_out << "        player_names.push_back(\"dummy\");" << std::endl;
    main_out << "    }" << std::endl;
    main_out << std::endl;
    
    main_out << "    std::cout << \"=== " << game_name << " - Framework Genèric ===\" << std::endl;" << std::endl;
    main_out << "    std::cout << \"Mapa: \" << map_file << std::endl;" << std::endl;
    main_out << "    std::cout << \"Jugadors: \";" << std::endl;
    main_out << "    for (const auto& name : player_names) {" << std::endl;
    main_out << "        std::cout << name << \" \";" << std::endl;
    main_out << "    }" << std::endl;
    main_out << "    std::cout << std::endl;" << std::endl;
    
    main_out << "    // Carregar definició del joc" << std::endl;
    main_out << "    auto game_def = std::make_shared<GameDefinition>();" << std::endl;
    main_out << "    if (!game_def->loadFromFile(\"" << config_file << "\")) {" << std::endl;
    main_out << "        std::cerr << \"Error carregant la definició del joc\" << std::endl;" << std::endl;
    main_out << "        return 1;" << std::endl;
    main_out << "    }" << std::endl;
    main_out << std::endl;
    
    main_out << "    // Crear el joc" << std::endl;
    main_out << "    Game game(game_def);" << std::endl;
    main_out << std::endl;
    
    main_out << "    // Carregar mapa" << std::endl;
    main_out << "    if (!game.loadMap(map_file)) {" << std::endl;
    main_out << "        std::cerr << \"Error carregant el mapa: \" << map_file << std::endl;" << std::endl;
    main_out << "        return 1;" << std::endl;
    main_out << "    }" << std::endl;
    main_out << std::endl;
    
    main_out << "    // Afegir jugadors dummy" << std::endl;
    main_out << "    for (int i = 0; i < " << game_def->getMaxPlayers() << "; ++i) {" << std::endl;
    main_out << "        game.addPlayer(std::make_shared<DummyPlayer>());" << std::endl;
    main_out << "    }" << std::endl;
    main_out << std::endl;
    
    main_out << "    // Executar partida" << std::endl;
    main_out << "    std::cout << \"\\n=== Iniciant Partida ===\" << std::endl;" << std::endl;
    main_out << "    game.run(std::cout);" << std::endl;
    main_out << std::endl;
    
    main_out << "    std::cout << \"\\n=== Partida Completada ===\" << std::endl;" << std::endl;
    main_out << "    return 0;" << std::endl;
    main_out << "}" << std::endl;
    main_out << std::endl;
    
    // Generar classe DummyPlayer
    main_out << "class DummyPlayer : public Player {" << std::endl;
    main_out << "private:" << std::endl;
    main_out << "    std::mt19937 rng;" << std::endl;
    main_out << "    " << std::endl;
    main_out << "public:" << std::endl;
    main_out << "    DummyPlayer() : rng(std::random_device{}()) {}" << std::endl;
    main_out << "    " << std::endl;
    main_out << "    void play() override {" << std::endl;
    main_out << "        auto units = board->getUnitsByPlayer(player_id);" << std::endl;
    main_out << "        if (units.empty()) return;" << std::endl;
    main_out << "        " << std::endl;
    main_out << "        std::uniform_int_distribution<int> action_dist(0, 1);" << std::endl;
    main_out << "        std::uniform_int_distribution<int> dir_dist(0, 7);" << std::endl;
    main_out << "        " << std::endl;
    main_out << "        int action_type = action_dist(rng);" << std::endl;
    main_out << "        int direction = dir_dist(rng);" << std::endl;
    main_out << "        " << std::endl;
    main_out << "        if (action_type == 0) {" << std::endl;
    main_out << "            action->move(direction);" << std::endl;
    main_out << "        } else {" << std::endl;
    main_out << "            action->throw_kamehame(direction);" << std::endl;
    main_out << "        }" << std::endl;
    main_out << "    }" << std::endl;
    main_out << "};" << std::endl;
    
    main_out.close();
    
    // 3. Generar Makefile específic
    std::string makefile_name = "Makefile_" + game_name;
    std::ofstream makefile_out(makefile_name);
    if (!makefile_out.is_open()) {
        std::cerr << "Error creant " << makefile_name << std::endl;
        return 1;
    }
    
    makefile_out << "# Makefile per " << game_name << std::endl;
    makefile_out << "CXX = g++" << std::endl;
    makefile_out << "CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g" << std::endl;
    makefile_out << "INCLUDES = -I./Core" << std::endl;
    makefile_out << "LIBS = " << std::endl;
    makefile_out << std::endl;
    makefile_out << "CORE_OBJECTS = GameDefinition.o Action.o Board.o Game.o" << std::endl;
    makefile_out << "MAIN_SOURCE = " << main_file << std::endl;
    makefile_out << "EXECUTABLE = " << game_name << "_game" << std::endl;
    makefile_out << std::endl;
    makefile_out << "all: $(EXECUTABLE)" << std::endl;
    makefile_out << std::endl;
    makefile_out << "$(EXECUTABLE): $(MAIN_SOURCE) $(CORE_OBJECTS)" << std::endl;
    makefile_out << "\t$(CXX) $(CXXFLAGS) $(INCLUDES) $(MAIN_SOURCE) $(CORE_OBJECTS) -o $(EXECUTABLE) $(LIBS)" << std::endl;
    makefile_out << std::endl;
    makefile_out << "clean:" << std::endl;
    makefile_out << "\trm -f $(EXECUTABLE) $(MAIN_SOURCE)" << std::endl;
    makefile_out << std::endl;
    makefile_out << ".PHONY: all clean" << std::endl;
    
    makefile_out.close();
    
    std::cout << "Fitxers generats:" << std::endl;
    std::cout << "  - " << main_file << std::endl;
    std::cout << "  - " << makefile_name << std::endl;
    std::cout << std::endl;
    std::cout << "Per compilar:" << std::endl;
    std::cout << "  make -f " << makefile_name << std::endl;
    std::cout << std::endl;
    std::cout << "Per executar:" << std::endl;
    std::cout << "  ./" << game_name << "_game <map_file> [player1] [player2] [player3] [player4]" << std::endl;
    
    return 0;
}
