#ifndef GRID_H
#define GRID_H

#include <cassert>
#include <vector>
class grid {
public:
    grid(const int width, const int height)
      : m_v(height,std::vector<double>(width,0.0))
  {
      assert(width == get_width());
      assert(height == get_height());
  }
  int get_height() const noexcept { return static_cast<int>(m_v.size()); }
  int get_width() const {
    assert(!m_v.empty());
    return static_cast<int>(m_v[0].size());
  }
  double get(const int x, const int y) const {
    assert(x >= 0);
    assert(x < get_height());
    assert(y >= 0);
    assert(y < get_width());
    return m_v[x][y];
  }
  void set(const int x, const int y, const double value) {
    assert(x >= 0);
    assert(x < get_height());
    assert(y >= 0);
    assert(y < get_width());
    m_v[x][y] = value;
    assert(get(x, y) == value);
  }

  private:
  //an x-y-ordered 2D vector
  std::vector<std::vector<double>> m_v;
};


#endif // GRID_H
