#include <iostream>
#include <string>
#include <fstream>
#include <vector>

bool TrimString(const std::string& input, const std::string& trim_chars, std::string* output) {
  const size_t last_char = input.length() - 1;
  const size_t first_good_char = input.find_first_not_of(trim_chars);
  const size_t last_good_char = input.find_last_not_of(trim_chars);
  
  if (input.empty() || (first_good_char == std::string::npos) || (last_good_char == std::string::npos)) {
    output->clear();
    return false;
  }
  *output = input.substr(first_good_char, last_good_char - first_good_char + 1);
  return true;
}

size_t Tokenize(const std::string& str, const std::string& delimiters, std::vector<std::string>* tokens) {
  tokens->clear();
  size_t start = str.find_first_not_of(delimiters);
  while (start != std::string::npos) {
    size_t end = str.find_first_of(delimiters, start + 1);
    if (end == std::string::npos) {
      tokens->push_back(str.substr(start));
      break;
    } else {
      tokens->push_back(str.substr(start, end - start));
      start = str.find_first_not_of(delimiters, end + 1);
    }
  }
  return tokens->size();
}

struct CidPair {
  std::string cid;
  std::vector<float>* embedding;
};

bool Load(const std::string & file_path) {
  std::ifstream input_f(file_path.c_str(), std::ios_base::in);
  if (!input_f.is_open()) {
    std::cout << "Failed to open file [" << file_path << "] for inputs.";
    return false;
  }
  std::string line;
  const static std::string kTrimChar = " \t\r\n'!\"#$%&()*+,-./:;<=>?@[\\]^_`{|}~";
  int num_line = 0;
  std::vector<CidPair*> cid_pair;
  while (std::getline(input_f, line)) {
    ++num_line;
    TrimString(line, kTrimChar, &line);
    std::vector<std::string> cid3_vector;
    Tokenize(line, "=", &cid3_vector);
    if (cid3_vector.size() != 2) {
      std::cout << "the size of line <" << num_line << "> is wrong!" << std::endl;
      return false;
    }
    std::vector<float>* embedding = new std::vector<float>;
    CidPair *p = new CidPair{cid3_vector[0], embedding};
    std::vector<std::string> tmp_vs;
    Tokenize(cid3_vector[1], ",", &tmp_vs);
    for (auto iter = tmp_vs.begin(); iter != tmp_vs.end(); ++iter) {
      embedding->push_back(std::stof(*iter));
    }
    cid_pair.push_back(p);
  }
}

int main() {
  Load("./cid.txt");
  return 0;
}
