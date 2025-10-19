#include <map>
#include <limits>
#include <functional>
#include <iostream>
#include "commands.hpp"
#include <tree.hpp>

int main()
{
  using namespace kushekbaev;
  using dictionary_system = HashTable< std::string, HashTable< std::string, Vector< std::string > > >;
  dictionary_system curr_ds;
  Tree< std::string, std::function< void(std::ostream&, std::istream&, dictionary_system&) > > commands;
  commands["insert"] = insert;
  commands["insert_without_translation"] = insert_without_translation;
  commands["print"] = print;
  commands["remove"] = kushekbaev::remove;
  commands["save"] = save;
  commands["import_dictionary"] = import_dictionary;
  commands["search"] = search;
  commands["clear_dictionary"] = clear_dictionary;
  commands["reverse_search"] = reverse_search;
  commands["remove_translation"] = remove_translation;
  commands["remove_translation_at_all"] = remove_translation_at_all;
  commands["delete_all_translations"] = delete_all_translations;
  commands["prefix_search"] = prefix_search;
  commands["no_prefix_search"] = no_prefix_search;
  commands["suffix_search"] = suffix_search;
  commands["no_suffix_search"] = no_suffix_search;
  commands["merge"] = merge;
  commands["split"] = split;
  commands["find_words_without_translation"] = find_words_without_translations;
  commands["complement"] = complement;
  commands["intersect"] = intersect;
  commands["unification"] = unification;

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)(std::cout, std::cin, curr_ds);
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << "\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
