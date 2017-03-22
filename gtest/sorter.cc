#include "sorter.h"
#include <algorithm>
#include <stack>

Sorter::Sorter(const std::vector<int>& numbers): numbers_(numbers) {
}

const std::vector<int>& Sorter::GetNumbers() const {
  return numbers_;
}

MySorter::MySorter(const std::vector<int>& numbers): Sorter(numbers) {
}

void MySorter::DoSort() {
  if (numbers_.empty()) {
    return ;
  }
  std::stack<std::pair<int, int> > s;
  s.push(std::make_pair(0, numbers_.size() - 1)); 
  while (!s.empty()) {
    std::pair<int, int> p = s.top();
    s.pop();
    int m = Partition(p.first, p.second);
    if (p.first < m - 1) {
      s.push(std::make_pair(p.first, m - 1));
    } 
    if (m + 1 < p.second) {
      s.push(std::make_pair(m + 1, p.second));
    }
  }
}

int MySorter::Partition(int l, int r) {
  int i = l - 1;
  for (int j = l; j < r; ++j) {
  if (numbers_[j] <= numbers_[r]) {
      std::swap(numbers_[++i], numbers_[j]);
    }
  }
  std::swap(numbers_[++i], numbers_[r]);
  return i;
} 

StdSorter::StdSorter(const std::vector<int>& numbers): Sorter(numbers) {
}

void StdSorter::DoSort() {
  std::sort(numbers_.begin(), numbers_.end());
}

