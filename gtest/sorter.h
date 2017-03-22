#include <vector>
#include <algorithm>
#include <stack>


class Sorter {
public:
  Sorter(const std::vector<int>& numbers);

  const std::vector<int>& GetNumbers() const;

protected:
  std::vector<int> numbers_;
};

class MySorter: public Sorter {
public:
  MySorter(const std::vector<int>& numbers);

  void DoSort();

private:
  int Partition(int l, int r);

};

class StdSorter: public Sorter {
public:
  StdSorter(const std::vector<int>& numbers);

  void DoSort();
};
