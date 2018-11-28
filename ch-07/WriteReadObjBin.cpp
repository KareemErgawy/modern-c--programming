#include <cassert>
#include <fstream>
#include <string>
#include <vector>

class foo {
  int i;
  char c;
  std::string s;

public:
  foo(int const i = 0, char const c = 0, std::string const &s = {})
      : i(i), c(c), s(s) {}

  foo(foo const &) = default;

  foo &operator=(foo const &) = default;

  bool operator==(foo const &rhv) const {
    return i == rhv.i && c == rhv.c && s == rhv.s;
  }

  bool operator!=(foo const &rhv) const { return !(*this == rhv); }

  bool write(std::ofstream &ofile) const {
    ofile.write(reinterpret_cast<const char *>(&i), sizeof(i));
    ofile.write(&c, sizeof(c));
    auto size = static_cast<size_t>(s.size());
    ofile.write(reinterpret_cast<const char *>(&size), sizeof(size));
    ofile.write(reinterpret_cast<const char *>(s.data()), s.size());

    return !ofile.fail();
  }

  bool read(std::ifstream &ifile) {
    ifile.read(reinterpret_cast<char *>(&i), sizeof(i));
    ifile.read(&c, sizeof(c));
    size_t size;
    ifile.read(reinterpret_cast<char *>(&size), sizeof(size));
    s.resize(size);
    ifile.read(reinterpret_cast<char *>(&s.front()), s.size());

    return ifile.fail();
  }
};

struct foopod {
  bool a;
  char b;
  int c[2];
};

bool operator==(foopod const &f1, foopod const &f2) {
  return f1.a == f2.a && f1.b == f2.b && f1.c[0] == f2.c[0] &&
         f1.c[1] == f2.c[1];
}

int main() {
  std::vector<foopod> output{
      {true, '1', {1, 2}}, {true, '2', {3, 4}}, {false, '3', {5, 6}}};

  std::ofstream ofile("sample.bin", std::ios::binary);

  if (ofile.is_open()) {
    ofile.write(reinterpret_cast<char *>(output.data()),
                output.size() * sizeof(foopod));

    ofile.close();
  }

  std::ifstream ifile("sample.bin", std::ios::binary);
  std::vector<foopod> input;

  if (ifile.is_open()) {
    while (true) {
      foopod value;
      ifile.read(reinterpret_cast<char *>(&value), sizeof(value));

      if (ifile.fail() || ifile.eof()) {
        break;
      }

      input.push_back(value);
    }

    ifile.close();
  }

  assert(output == input);

  foo fooOutput(1, 'a', "Hello, world");
  std::ofstream ofile2("sample2.bin", std::ios::binary);
  fooOutput.write(ofile2);
  ofile2.close();

  foo fooInput;
  std::ifstream ifile2("sample2.bin", std::ios::binary);
  fooInput.read(ifile2);
  ifile2.close();

  assert(fooOutput == fooInput);

  return 0;
}
