#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "graphCollection.hpp"

namespace smirnov
{

// Функция для вывода ошибки
void printError(std::ostream & err);

// Функции обработки конкретных команд
void command_graphs(const GraphCollection & graphs, std::ostream & out);
void vertexes(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err);
void outbound(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err);
void inbound(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err);
void bind(GraphCollection & graphs, std::istream & in, std::ostream & err);
void cut(GraphCollection & graphs, std::istream & in, std::ostream & err);
void create(GraphCollection & graphs, std::istream & in, std::ostream & err);
void merge(GraphCollection & graphs, std::istream & in, std::ostream & err);
void extract(GraphCollection & graphs, std::istream & in, std::ostream & err);

// Основная функция обработки команд
void processCommands(GraphCollection & graphs,
                     std::istream & in,
                     std::ostream & out,
                     std::ostream & err);

}  // namespace smirnov

#endif  // COMMANDS_HPP
