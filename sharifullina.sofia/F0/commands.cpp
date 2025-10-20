#include "commands.hpp"
#include <fstream>
#include <numeric>
#include <iterator>
#include <iomanip>

namespace
{
    bool dictExists(const std::string & name, const sharifullina::DictCollection & dicts)
    {
    for (auto it = dicts.begin(); it != dicts.end(); ++it)
    {
      if ((*it).first == name)
      {
        return true;
      }
    }
    return false;
  }

  bool wordExists(const std::string & dictName, const std::string & word, const sharifullina::DictCollection & dicts)
  {
  for (auto dictIt = dicts.begin(); dictIt != dicts.end(); ++dictIt)
  {
    if ((*dictIt).first == dictName)
    {
      const auto & dictionary = (*dictIt).second;
      for (auto wordIt = dictionary.begin(); wordIt != dictionary.end(); ++wordIt)
      {
        if ((*wordIt).first == word)
        {
          return true;
        }
      }
      return false;
    }
  }
  return false;
  }

  struct DictExists
  {
    const sharifullina::DictCollection & dicts;
    bool operator()(const std::string & rhs)
    {
      return dictExists(rhs, dicts);
    }
  };

  struct WordEntry
  {
    const std::pair< const std::string, sharifullina::AVLtree< std::string, bool > > & data;
    WordEntry(const std::pair< const std::string, sharifullina::HashTable< std::string, int > > & pair):
      data(pair)
    {}
  };

  std::ostream & operator<<(std::ostream & out, const WordEntry & entry)
  {
    out << entry.data.first << ' ';
    for (auto it = entry.data.second.begin(); it != entry.data.second.end(); ++it)
    {
        out << it->first << ' ';
    }
    return out;
  }

  struct DictNameView
  {
    const std::pair< const std::string, sharifullina::Dictionary > & data;
    DictNameView(const std::pair< const std::string, sharifullina::Dictionary > & pair):
      data(pair)
    {}
  };

  std::ostream & operator<<(std::ostream & out, const DictNameView & view)
  {
    out << view.data.first;
    return out;
  }

  void readTranslations(std::istream & in, sharifullina::AVLtree< std::string, bool > & translations)
  {
    std::istream_iterator< std::string > it(in);
    std::istream_iterator< std::string > end;
    std::copy(it, end, std::inserter(translations, translations.begin()));
  }

  sharifullina::Dictionary readDictionary(std::istream & in)
  {
    sharifullina::Dictionary dict;
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }
      std::string word;
      sharifullina::AVLtree< std::string, bool > translations;
      std::stringstream lineStream(line);
      if (lineStream >> word)
      {
        readTranslations(lineStream, translations);
      }
      if (!translations.empty())
      {
        dict[word] = translations;
      }
    }
    return dict;
  }

  struct MergeCondition
  {
    const sharifullina::List< std::pair< std::string, int > > & names;

    bool operator()(const std::pair< std::string, sharifullina::Dictionary > & pair)
    {
        return std::find(names.cbegin(), names.cend(), pair.first) != names.cend();
    }
  };

  struct MergePusher
  {
    using value_type = std::pair< std::string, sharifullina::AVLtree< std::string, bool > >;

    sharifullina::Dictionary & output;

    void push_back(const std::pair< std::string, sharifullina::AVLtree< std::string, bool > > & pair)
    {
        output[pair.first].insert(pair.second.cbegin(), pair.second.cend());
    }
  };

  struct MergeWrapper
  {
    using value_type = std::pair< std::string, sharifullina::Dictionary >;

    sharifullina::Dictionary output;

    void push_back(const std::pair< std::string, sharifullina::Dictionary > & pair)
    {
        MergePusher pusher{output};
        std::copy(pair.second.cbegin(), pair.second.cend(), std::back_inserter(pusher));
    }
  };

  struct TranslationCounter
  {
    size_t operator()(size_t sum, const std::pair< const std::string, sharifullina::AVLtree< std::string, bool > > & rhs)
    {
      return sum + rhs.second.size();
    }
  };

  struct SubstractCondition
  {
    const sharifullina::Dictionary & discard;
    bool operator()(const std::pair< std::string, sharifullina::AVLtree< std::string, bool > > & rhs)
    {
        return discard.find(rhs.first) == discard.cend();
    }
  };

  struct WordChecker
  {
    const std::pair< std::string, sharifullina::AVLtree< std::string, bool > > & src;
    bool operator()(const std::pair< std::string, sharifullina::Dictionary > & rhs)
    {
      return rhs.second.find(src.first) != rhs.second.end();
    }
  };

  struct SymDiffCondition
  {
    const sharifullina::DictCollection & all;
    bool operator()(const std::pair< std::string, sharifullina::AVLtree< std::string, bool > > & rhs)
    {
      return std::count_if(all.cbegin(), all.cend(), WordChecker{rhs}) == 1;
    }
  };

  struct WordInside
  {
    const sharifullina::List< std::pair< std::string, int > > & src;
    bool operator()(const std::pair< std::string, std::set< std::string > > & rhs)
    {
      return std::find(src.cbegin(), src.cend(), rhs.first) != src.cend();
    }
  };

  struct TranslationInside
  {
    const std::string & src;
    bool operator()(const std::pair< std::string, sharifullina::AVLtree< std::string, bool > > & rhs)
    {
      return std::find(rhs.second.cbegin(), rhs.second.cend(), src) != rhs.second.cend();
    }
  };

  struct SetWrapper
  {
    using value_type = std::pair< std::string, sharifullina::AVLtree< std::string, bool > >;
    sharifullina::AVLtree< std::string, bool > output;
    void push_back(const std::pair< std::string, sharifullina::AVLtree< std::string, bool > > & rhs)
    {
      output.insert(rhs.second.cbegin(), rhs.second.cend());
    }
  };

  struct CommonCondition
  {
    const sharifullina::Dictionary & src;
    bool operator()(const std::string & rhs)
    {
      return std::all_of(src.cbegin(), src.cend(), TranslationInside{rhs});
    }
  };
}

void sharifullina::createDict(std::istream & in, DictCollection & dicts)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("invalid arguments for createdict");
  }

  if (dictExists(name, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }

  dicts[name] = Dictionary{};
}

void sharifullina::deleteDict(std::istream & in, DictCollection & dicts)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("invalid arguments for deletedict");
  }

  auto it = dicts.find(name);
  if (it == dicts.end())
  {
    throw std::runtime_error("dictionary not found");
  }
  dicts.erase(it);
}

void sharifullina::listDicts(std::istream &, const DictCollection & dicts, std::ostream & out)
{
  if (dicts.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  std::copy(dicts.cbegin(), dicts.cend(), std::ostream_iterator< DictNameView >(out, "\n"));
}

void sharifullina::addWord(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  if (!(in >> dictName >> word))
  {
    throw std::runtime_error("invalid arguments for addword");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found");
  }
  auto & dict = dicts[dictName];
  if (dict.find(word) != dict.end())
  {
    throw std::runtime_error("word already exists");
  }
  sharifullina::AVLtree< std::string, bool > translations;
  readTranslations(in, translations);
  if (translations.empty())
  {
    throw std::runtime_error("no translations provided");
  }
  dict[word] = translations;
}

void sharifullina::addTranslation(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  std::string translation;
  if (!(in >> dictName >> word >> translation))
  {
    throw std::runtime_error("invalid arguments for addtranslation");
  }
  if (!wordExists(dictName, word, dicts))
  {
    throw std::runtime_error("dictionary or word not found");
  }
  auto & dict = dicts[dictName];
  auto wordIt = dict.find(word);
  wordIt->second.insert(translation);
}

void sharifullina::removeTranslation(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  std::string translation;
  if (!(in >> dictName >> word >> translation))
  {
    throw std::runtime_error("invalid arguments for removetranslation");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary, word or translation not found");
  }
  auto & dict = dicts[dictName];
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    throw std::runtime_error("dictionary, word or translation not found");
  }
  auto & translations = wordIt->second;
  auto transIt = translations.find(translation);
  if (transIt == translations.end())
  {
    throw std::runtime_error("dictionary, word or translation not found");
  }
  translations.erase(transIt);
  if (translations.empty())
  {
    dict.erase(wordIt);
  }
}

void sharifullina::deleteWord(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string word;
  if (!(in >> dictName >> word))
  {
    throw std::runtime_error("invalid arguments for deleteword");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary or word not found");
  }
  auto & dict = dicts[dictName];
  if (dict.erase(word) == 0)
  {
    throw std::runtime_error("dictionary or word not found");
  }
}

void sharifullina::findTranslations(std::istream & in, const DictCollection & dicts, std::ostream & out)
{
  std::string dictName;
  std::string word;
  if (!(in >> dictName >> word))
  {
    throw std::runtime_error("invalid arguments for findtranslations");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary or word not found");
  }
  const auto & dict = dicts.at(dictName);
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    throw std::runtime_error("dictionary or word not found");
  }
  std::copy(wordIt->second.cbegin(), wordIt->second.cend(), std::ostream_iterator< std::string >(out, " "));
  out << '\n';
}

void sharifullina::listWords(std::istream & in, const DictCollection & dicts, std::ostream & out)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    throw std::runtime_error("invalid arguments for listwords");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found");
  }
  const auto & dict = dicts.at(dictName);
  if (dict.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  std::copy(dict.cbegin(), dict.cend(), std::ostream_iterator< WordEntry >(out, "\n"));
}

void sharifullina::mergeDicts(std::istream & in, DictCollection & dicts)
{
  std::string newDictName;
  size_t count = 0;
  if (!(in >> newDictName >> count))
  {
    throw std::runtime_error("invalid arguments for merge");
  }
  if (count < 2)
  {
    throw std::runtime_error("invalid count");
  }
  sharifullina::List< std::pair< std::string, int > > dictNames;
  std::copy(std::istream_iterator< std::string >(in), std::istream_iterator< std::string >(), std::back_inserter(dictNames));
  if (dictNames.size() != count)
  {
    throw std::runtime_error("invalid count");
  }
  if (!std::all_of(dictNames.cbegin(), dictNames.cend(), DictExists{dicts}))
  {
    throw std::runtime_error("dictionary not found");
  }
  if (dictExists(newDictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  MergeWrapper wrapper;
  std::copy_if(dicts.cbegin(), dicts.cend(), std::back_inserter(wrapper), MergeCondition{dictNames});
  dicts[newDictName] = wrapper.output;
}

void sharifullina::findCommon(std::istream & in, const DictCollection & dicts, std::ostream & out)
{
  std::string dictName;
  size_t count = 0;
  if (!(in >> dictName >> count))
  {
    throw std::runtime_error("invalid arguments for findcommon");
  }
  if (count < 1)
  {
    throw std::runtime_error("invalid count");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary or word(s) not found");
  }
  sharifullina::List< std::pair< std::string, int > > words;
  std::copy(std::istream_iterator< std::string >(in), std::istream_iterator< std::string >(), std::back_inserter(words));
  if (words.size() != count)
  {
    throw std::runtime_error("invalid count");
  }
  const auto & dict = dicts.at(dictName);
  if (!std::all_of(dict.cbegin(), dict.cend(), WordInside{words}))
  {
    throw std::runtime_error("dictionary or word(s) not found");
  }
  sharifullina::Dictionary temp;
  std::copy_if(dict.cbegin(), dict.cend(), std::inserter(temp, temp.end()), WordInside{words});
  SetWrapper wrapper;
  std::copy_if(dict.cbegin(), dict.cend(), std::back_inserter(wrapper), WordInside{words});
  sharifullina::AVLtree< std::string, bool > commonTs;
  std::copy_if(wrapper.output.cbegin(), wrapper.output.cend(), std::inserter(commonTs, commonTs.end()), CommonCondition{temp});

  if (commonTs.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  std::copy(commonTs.cbegin(), commonTs.cend(), std::ostream_iterator< std::string >(out, " "));
  out << '\n';
}

void sharifullina::saveDict(std::istream & in, const DictCollection & dicts, std::ostream & out)
{
  std::string dictName;
  std::string filename;
  if (!(in >> dictName >> filename))
  {
    throw std::runtime_error("invalid arguments for save");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found or file error");
  }
  std::ofstream file(filename);
  if (!file)
  {
    throw std::runtime_error("dictionary not found or file error");
  }
  const auto & dict = dicts.at(dictName);
  std::copy(dict.cbegin(), dict.cend(), std::ostream_iterator< WordEntry >(out, "\n"));
}

void sharifullina::loadDict(std::istream & in, DictCollection & dicts)
{
  std::string dictName;
  std::string filename;
  if (!(in >> dictName >> filename))
  {
    throw std::runtime_error("invalid arguments for load");
  }
  if (dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("file not found or invalid format");
  }
  Dictionary newDict = readDictionary(file);
  if (newDict.empty())
  {
    throw std::runtime_error("file not found or invalid format");
  }
  dicts[dictName] = newDict;
}

void sharifullina::statDict(std::istream & in, const DictCollection & dicts, std::ostream & out)
{
  std::string dictName;
  if (!(in >> dictName))
  {
    throw std::runtime_error("invalid arguments for stat");
  }
  if (!dictExists(dictName, dicts))
  {
    throw std::runtime_error("dictionary not found");
  }
  const auto & dict = dicts.at(dictName);
  size_t totalWords = dict.size();
  size_t totalTranslations = std::accumulate(dict.cbegin(), dict.cend(), 0, TranslationCounter{});
  double avgTranslations = totalWords > 0 ? static_cast< double >(totalTranslations) / totalWords : 0.0;
  out << "Words: " << totalWords << "\n";
  out << "Translations: " << totalTranslations << "\n";
  out << "Average translations per word: " << avgTranslations << "\n";
}

void sharifullina::subtractDicts(std::istream & in, DictCollection & dicts)
{
  std::string newDictName;
  size_t count = 0;
  if (!(in >> newDictName >> count))
  {
    throw std::runtime_error("invalid arguments for subtract");
  }
  if (count < 2)
  {
    throw std::runtime_error("invalid count");
  }
  sharifullina::List< std::pair< std::string, int > > dictNames;
  std::copy(std::istream_iterator< std::string >(in), std::istream_iterator< std::string >(), std::back_inserter(dictNames));
  if (dictNames.size() != count)
  {
    throw std::runtime_error("invalid count");
  }
  if (!std::all_of(dictNames.cbegin(), dictNames.cend(), DictExists{dicts}))
  {
    throw std::runtime_error("dictionary not found");
  }
  if (dictExists(newDictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  auto target = dicts.find(dictNames[0]);

  MergeWrapper wrapper;
  std::copy_if(dicts.begin(), target, std::back_inserter(wrapper), MergeCondition{dictNames});
  std::copy_if(std::next(target), dicts.end(), std::back_inserter(wrapper), MergeCondition{dictNames});

  sharifullina::Dictionary newDict;
  auto & temp = target->second;
  std::copy_if(temp.cbegin(), temp.cend(), std::inserter(newDict, newDict.end()), SubstractCondition{wrapper.output});
  dicts[newDictName] = newDict;
}

void sharifullina::symdiffDicts(std::istream & in, DictCollection & dicts)
{
  std::string newDictName;
  size_t count = 0;
  if (!(in >> newDictName >> count))
  {
    throw std::runtime_error("invalid arguments for symdiff");
  }
  if (count < 2)
  {
    throw std::runtime_error("invalid count");
  }
  sharifullina::List< std::pair< std::string, int > > dictNames;
  std::copy(std::istream_iterator< std::string >(in), std::istream_iterator< std::string >(), std::back_inserter(dictNames));
  if (dictNames.size() != count)
  {
    throw std::runtime_error("invalid count");
  }
  if (!std::all_of(dictNames.cbegin(), dictNames.cend(), DictExists{dicts}))
  {
    throw std::runtime_error("dictionary not found");
  }
  if (dictExists(newDictName, dicts))
  {
    throw std::runtime_error("dictionary already exists");
  }
  MergeWrapper wrapper;
  std::copy_if(dicts.cbegin(), dicts.cend(), std::back_inserter(wrapper), MergeCondition{dictNames});

  sharifullina::Dictionary newDict;
  auto & temp = wrapper.output;
  std::copy_if(temp.cbegin(), temp.cend(), std::inserter(newDict, newDict.end()), SymDiffCondition{dicts});
  dicts[newDictName] = newDict;
}

void sharifullina::loadFile(const std::string & filename, DictCollection & dicts)
{
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("file not found or invalid format");
  }
  Dictionary newDict = readDictionary(file);
  if (!newDict.empty())
  {
    dicts["default"] = newDict;
  }
}

void sharifullina::printHelp(std::ostream & out)
{
  out << std::left;
  out << "Available commands:\n" << '\n';
  constexpr size_t cmdWidth = 30;
  constexpr size_t numWidth = 3;
  out << std::setw(numWidth) << "1." << std::setw(cmdWidth);
  out << "createdict <name>" << "create a new dictionary\n";

  out << std::setw(numWidth) << "2." << std::setw(cmdWidth);
  out << "deletedict <name>" << "delete a dictionary\n";

  out << std::setw(numWidth) << "3." << std::setw(cmdWidth);
  out << "listdicts" << "list all dictionaries\n";

  out << std::setw(numWidth) << "4." << std::setw(cmdWidth);
  out << "addword <dict> <word> <trans...>" << "add word with translations\n";

  out << std::setw(numWidth) << "5." << std::setw(cmdWidth);
  out << "addtranslation <dict> <word> <trans>" << "add translation to word\n";

  out << std::setw(numWidth) << "6." << std::setw(cmdWidth);
  out << "removetranslation <dict> <word> <trans>" << "remove translation\n";

  out << std::setw(numWidth) << "7." << std::setw(cmdWidth);
  out << "deleteword <dict> <word>" << "delete word\n";

  out << std::setw(numWidth) << "8." << std::setw(cmdWidth);
  out << "findtranslations <dict> <word>" << "find word translations\n";

  out << std::setw(numWidth) << "9." << std::setw(cmdWidth);
  out << "listwords <dict>" << "list all words in dictionary\n";

  out << std::setw(numWidth) << "10." << std::setw(cmdWidth);
  out << "merge <new> <count> <dicts...>" << "merge dictionaries\n";

  out << std::setw(numWidth) << "11." << std::setw(cmdWidth);
  out << "findcommon <dict> <count> <words...>" << "find common translations\n";

  out << std::setw(numWidth) << "12." << std::setw(cmdWidth);
  out << "save <dict> <file>" << "save dictionary to file\n";

  out << std::setw(numWidth) << "13." << std::setw(cmdWidth);
  out << "load <dict> <file>" << "load dictionary from file\n";

  out << std::setw(numWidth) << "14." << std::setw(cmdWidth);
  out << "stat <dict>" << "show dictionary statistics\n";

  out << std::setw(numWidth) << "15." << std::setw(cmdWidth);
  out << "subtract <new> <count> <dicts...>" << "dictionary subtraction\n";

  out << std::setw(numWidth) << "16." << std::setw(cmdWidth);
  out << "symdiff <new> <count> <dicts...>" << "symmetric difference\n";
}

void sharifullina::printError(const std::string & message, std::ostream & out)
{
  out << "<ERROR: " << message << ">\n";
}
