#include <cassert>
#include <fstream>
#include <functional>
#include <vector>

bool write_data(char const *const filename, char const *const data,
                size_t const size) {
  auto success = false;
  std::ofstream ofile(filename, std::ios::binary);

  if (ofile.is_open()) {
    try {
      ofile.write(data, size);
      success = true;
    } catch (std::ios_base::failure &) {
    }

    ofile.close();
  }

  return success;
}

size_t read_data(char const *const filename,
                 std::function<char *(size_t const)> allocator) {
  size_t readbytes = 0;
  std::ifstream ifile(filename, std::ios::ate | std::ios::binary);

  if (ifile.is_open()) {
    auto length = static_cast<size_t>(ifile.tellg());
    ifile.seekg(0, std::ios_base::beg);

    auto buffer = allocator(length);

    try {
      ifile.read(buffer, length);
      readbytes = static_cast<size_t>(ifile.gcount());
    } catch (std::ios_base::failure &) {
    }

    ifile.close();
  }

  return readbytes;
}

int main() {
  std::vector<unsigned char> output{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  write_data("sample.bin", reinterpret_cast<char *>(output.data()),
             output.size());
  // std::ofstream ofile("sample.bin", std::ios::binary);

  // if (ofile.is_open()) {
  //  ofile.write(reinterpret_cast<char *>(output.data()), output.size());
  //  ofile.close();
  //}

  std::vector<unsigned char> input;
  read_data("sample.bin", [&input](size_t const length) {
    input.resize(length);
    return reinterpret_cast<char *>(input.data());
  });
  // std::ifstream ifile("sample.bin", std::ios::binary);

  // if (ifile.is_open()) {
  //  // TODO understand the behavior of this in case of file larger than
  //  memory. ifile.seekg(0, std::ios_base::end); auto length = ifile.tellg();
  //  ifile.seekg(0, std::ios_base::beg);

  //  std::vector<unsigned char> input;
  //  input.resize(static_cast<size_t>(length));

  //  ifile.read(reinterpret_cast<char *>(input.data()), length);
  //  auto success = !ifile.fail() && length == ifile.gcount() && output ==
  //  input; assert(success);

  //  ifile.close();
  //}

  assert(output == input);

  return 0;
}
