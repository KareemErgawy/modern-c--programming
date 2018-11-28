#include <iomanip>
#include <iostream>
#include <vector>

struct Chapter {
  int Number;
  std::string Title;
  int Page;
};

struct BookPart {
  std::string Title;
  std::vector<Chapter> Chapters;
};

struct Book {
  std::string Title;
  std::vector<BookPart> Parts;
};

std::string to_roman(unsigned int value) {
  struct roman_t {
    unsigned int value;
    char const *numeral;
  };
  const static roman_t rarr[13] = {
      {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"},
      {90, "XC"},  {50, "L"},   {40, "XL"}, {10, "X"},   {9, "IX"},
      {5, "V"},    {4, "IV"},   {1, "I"}};

  std::string result;
  for (auto const &number : rarr) {
    while (value >= number.value) {
      result += number.numeral;
      value -= number.value;
    }
  }

  return result;
}

void print_toc(Book const &book) {
  std::cout << book.Title << std::endl;

  for (auto const &part : book.Parts) {
    std::cout << std::left << std::setw(15) << std::setfill(' ') << part.Title
              << std::endl;
    std::cout << std::left << std::setw(15) << std::setfill('-') << "-"
              << std::endl;

    for (auto const &chapter : part.Chapters) {
      std::cout << std::right << std::setw(4) << std::setfill(' ')
                << to_roman(chapter.Number) << ' ';
      std::cout << std::left << std::setw(35) << std::setfill('.')
                << chapter.Title;
      std::cout << std::right << std::setw(3) << std::setfill('.')
                << chapter.Page << std::endl;
    }
  }
}

int main() {
  std::cout << std::boolalpha << true << std::endl;
  std::cout << false << std::endl;
  std::cout << std::noboolalpha << false << std::endl;

  std::cout << std::right << std::setw(10) << "right" << std::endl;
  std::cout << std::setw(10) << "text" << std::endl;
  std::cout << std::left << std::setw(10) << "left" << std::endl;

  std::cout << std::fixed << 0.25 << std::endl;
  std::cout << std::scientific << 0.25 << std::endl;
  std::cout << std::hexfloat << 0.25 << std::endl;
  std::cout << std::defaultfloat << 0.25 << std::endl;

  std::cout << std::oct << 42 << std::endl;
  std::cout << std::hex << 42 << std::endl;
  std::cout << std::dec << 42 << std::endl;

  std::cout << std::right << std::setfill('.') << std::setw(10) << "right"
            << std::endl;

  std::cout << std::fixed << std::setprecision(2) << 12.345 << std::endl;

  auto book = Book{"THE FELLOWSHIP OF THE RING",
                   {{"BOOK ONE", {{1, "A Long-expected Party", 21}}}}};

  print_toc(book);

  return 0;
}
